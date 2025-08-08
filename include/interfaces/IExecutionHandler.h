// include/interfaces/IExecutionHandler.h

#pragma once

#include "core/TradeOrder.h"
#include <vector>
#include <map>
#include <string>

class Portfolio;

/**
 * @class IExecutionHandler
 * @brief The "Job Description" for any component that executes trades.
 *
 * This interface defines the contract for an execution handler. Its job is to
 * compare the current portfolio to the final, approved target portfolio and
 * generate the specific list of trades required to rebalance.
 */
class IExecutionHandler {
public:
    virtual ~IExecutionHandler() = default;

    /**
     * @brief Generates a list of trades to align the current portfolio with the target.
     * @param current_portfolio The portfolio's current state (holdings, cash, etc.).
     * @param approved_target The final, risk-approved target portfolio.
     * @return A vector of TradeOrder objects to be sent to the broker.
     */
    virtual std::vector<TradeOrder> execute_trades(
        Portfolio& current_portfolio,
        const std::map<std::string, double>& approved_target) = 0;
};