// include/core/Portfolio.h

#pragma once

#include <map>
#include <string>
#include <mutex>

class Portfolio {
public:
    explicit Portfolio(double initial_cash);

    double get_cash() const;
    double get_total_value() const;
    std::map<std::string, long long> get_holdings() const;
    long long get_position(const std::string& symbol) const;

    void update_cash(double amount);
    void update_holding(const std::string& symbol, long long quantity);
    void recalculate_total_value(const std::map<std::string, double>& latest_prices);

private:
    mutable std::mutex m_mutex;

    double m_cash;
    double m_total_value;
    std::map<std::string, long long> m_holdings;
};