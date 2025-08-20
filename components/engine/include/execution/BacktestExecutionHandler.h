// include/execution/BacktestExecutionHandler.h

#pragma once
#include "interfaces/IExecutionHandler.h"

class BacktestExecutionHandler : public IExecutionHandler {
public:
    /**
     * @brief Constructs the handler with specific backtest parameters.
     * @param commission The cost per trade.
     * @param slippage The percentage of slippage to apply to the execution price.
     */
    BacktestExecutionHandler(double commission, double slippage);

    ~BacktestExecutionHandler() override = default;

    void execute_trades(
        Portfolio& portfolio,
        const std::map<std::string, double>& approved_target,
        const std::map<std::string, double>& latest_prices
    ) override;

private:
    double m_commission_per_trade;
    double m_slippage_percentage;
};