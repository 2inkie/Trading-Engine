// src/signals/AggregatedIPCSource.cpp

#include "core/SignalPacket.h"
#include "signals/AggregatedIPCSource.h"
#include <numeric>
#include <iostream>
#include <cmath>

// Private helper function for the aggregation logic
namespace {
    std::map<std::string, double> aggregate_signals(const std::vector<SignalPacket>& packets) {
        if (packets.empty()) {
            return {};
        }

        std::map<std::string, double> weighted_sums;
        std::map<std::string, double> total_confidences;

        // 1. Accumulate weighted sums and total confidences for each asset
        for (const auto& packet : packets) {
            weighted_sums[packet.symbol] += packet.target_weight * packet.confidence;
            total_confidences[packet.symbol] += packet.confidence;
        }

        // 2. Calculate the final weighted average for each asset
        std::map<std::string, double> aggregated_portfolio;
        for (const auto& pair : weighted_sums) {
            const std::string& asset = pair.first;
            if (total_confidences[asset] > 1e-9) { // Avoid division by zero
                aggregated_portfolio[asset] = weighted_sums[asset] / total_confidences[asset];
            }
        }

        std::cout << "[IPCSource] Aggregation complete on " << packets.size() << " signals." << std::endl;
        return aggregated_portfolio;
    }
}

AggregatedIPCSource::AggregatedIPCSource(const std::vector<std::string>& model_endpoints,
                                       std::chrono::milliseconds reply_timeout)
    : m_context(1),
      m_reply_timeout(reply_timeout)
{
    m_sockets.reserve(model_endpoints.size());
    for (const auto& endpoint : model_endpoints) {
        std::cout << "[IPCSource] Connecting REQ socket to " << endpoint << std::endl;
        m_sockets.emplace_back(m_context, zmq::socket_type::req);
        m_sockets.back().connect(endpoint);
    }
}

void AggregatedIPCSource::update_market_data(const nlohmann::json& market_data) {
    m_latest_market_data = market_data;
}

std::map<std::string, double> AggregatedIPCSource::get_target_portfolio() {
    if (m_latest_market_data.is_null()) {
        std::cerr << "[IPCSource] ERROR: get_target_portfolio() called before update_market_data()." << std::endl;
        return {};
    }

    // --- 1. Send Requests ---
    std::string request_str = m_latest_market_data.dump();
    for (auto& socket : m_sockets) {
        socket.send(zmq::buffer(request_str), zmq::send_flags::dontwait);
    }

    // --- 2. Poll for Replies ---
    std::vector<zmq::pollitem_t> poll_items;
    poll_items.reserve(m_sockets.size());
    for (auto& socket : m_sockets) {
        poll_items.push_back({socket, 0, ZMQ_POLLIN, 0});
    }

    zmq::poll(poll_items, m_reply_timeout);

    // --- 3. Collect and Parse Replies ---
    std::vector<SignalPacket> received_packets;
    for (size_t i = 0; i < poll_items.size(); ++i) {
        if (poll_items[i].revents & ZMQ_POLLIN) {
            zmq::message_t reply;
            if (m_sockets[i].recv(reply, zmq::recv_flags::dontwait)) {
                try {
                    nlohmann::json j = nlohmann::json::parse(reply.to_string());
                    received_packets.push_back(j.get<SignalPacket>());
                } catch (const nlohmann::json::exception& e) {
                    std::cerr << "[IPCSource] ERROR parsing JSON reply: " << e.what() << std::endl;
                }
            }
        }
    }

    std::cout << "[IPCSource] Polling complete. Received "
              << received_packets.size() << "/" << m_sockets.size() << " replies." << std::endl;

    // --- 4. Aggregate and Return ---
    return aggregate_signals(received_packets);
}