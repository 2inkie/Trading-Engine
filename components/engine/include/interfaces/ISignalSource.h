// include/interfaces/ISignalSource.h

#pragma once

#include <map>
#include <string>
#include <nlohmann/json.hpp>

class ISignalSource {
public:
    virtual ~ISignalSource() = default;

    /**
     * @brief Updates the source with the latest market data.
     * The EventLoop calls this first to provide the necessary data.
     * @param market_data A JSON object containing the latest market data.
     */
    virtual void update_market_data(const nlohmann::json& market_data) = 0;

    /**
     * @brief Triggers communication and returns the aggregated portfolio.
     * The EventLoop calls this second, after updating the market data.
     * @return A map defining the final, aggregated target portfolio.
     */
    virtual std::map<std::string, double> get_target_portfolio() = 0;
};