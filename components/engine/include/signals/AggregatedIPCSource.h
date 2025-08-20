// include/signals/AggregatedIPCSource.h

#pragma once

#include "interfaces/ISignalSource.h"
#include "core/SignalPacket.h"
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <vector>
#include <string>

class AggregatedIPCSource : public ISignalSource {
public:
    AggregatedIPCSource(const std::vector<std::string>& model_endpoints,
                        std::chrono::milliseconds reply_timeout);

    ~AggregatedIPCSource() override = default;

    // Implements a two-step interface contract
    void update_market_data(const nlohmann::json& market_data) override;
    std::map<std::string, double> get_target_portfolio() override;

    // --- Safety: Disallow copy/move ---
    AggregatedIPCSource(const AggregatedIPCSource&) = delete;
    AggregatedIPCSource& operator=(const AggregatedIPCSource&) = delete;
    AggregatedIPCSource(AggregatedIPCSource&&) = delete;
    AggregatedIPCSource& operator=(AggregatedIPCSource&&) = delete;

private:
    zmq::context_t m_context;
    std::vector<zmq::socket_t> m_sockets;
    std::chrono::milliseconds m_reply_timeout;

    // Holds the data passed in from the update_market_data call
    nlohmann::json m_latest_market_data;
};