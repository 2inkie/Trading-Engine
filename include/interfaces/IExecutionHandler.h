// include/interfaces/IExecutionHandler.h

#pragma once

#include <vector>
#include <map>
#include <string>

class Portfolio;
class TradeOrder;

class IExecutionHandler {
public:
    virtual ~IExecutionHandler() = default;

    /**
     * @brief Executes trades to align the portfolio with the target.
     * This method directly modifies the portfolio object to reflect the
     * executed trades, including simulated costs.
     * @param portfolio The portfolio object to be modified.
     * @param approved_target The final, risk-approved target portfolio.
     * @param latest_prices A map of symbols to their latest prices, needed for cost calculation.
     */
    virtual void execute_trades(
        Portfolio& portfolio,
        const std::map<std::string, double>& approved_target,
        const std::map<std::string, double>& latest_prices
    ) = 0;
};