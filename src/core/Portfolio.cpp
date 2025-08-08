// src/core/Portfolio.cpp

#include "core/Portfolio.h"

Portfolio::Portfolio(double initial_cash)
    : m_cash(initial_cash), m_total_value(initial_cash) {
    // No lock needed here, as the object hasn't been shared with other threads yet.
}

double Portfolio::get_cash() const {
    std::lock_guard<std::mutex> lock(m_mutex); // Lock is acquired
    return m_cash;
} // Lock is automatically released

double Portfolio::get_total_value() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_total_value;
}

std::map<std::string, long long> Portfolio::get_holdings() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_holdings; // A copy is created and returned
}

long long Portfolio::get_position(const std::string& symbol) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_holdings.find(symbol);
    if (it != m_holdings.end()) {
        return it->second;
    }
    return 0;
}

void Portfolio::update_cash(double amount) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_cash += amount;
}

void Portfolio::update_holding(const std::string& symbol, long long quantity) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_holdings[symbol] += quantity;
    if (m_holdings[symbol] == 0) {
        m_holdings.erase(symbol);
    }
}

void Portfolio::recalculate_total_value(const std::map<std::string, double>& latest_prices) {
    std::lock_guard<std::mutex> lock(m_mutex);
    double market_value = 0.0;
    for (const auto& pair : m_holdings) {
        const std::string& symbol = pair.first;
        const long long quantity = pair.second;
        auto price_it = latest_prices.find(symbol);
        if (price_it != latest_prices.end()) {
            market_value += quantity * price_it->second;
        }
    }
    m_total_value = m_cash + market_value;
}