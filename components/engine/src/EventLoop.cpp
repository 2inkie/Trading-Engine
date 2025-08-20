// src/EventLoop.cpp

#include "EventLoop.h"
#include "core/DataBar.h"
#include <iostream>
#include <nlohmann/json.hpp>

EventLoop::EventLoop(
    std::unique_ptr<IDataProvider> data_provider,
    std::unique_ptr<ISignalSource> signal_source,
    std::unique_ptr<IRiskManager> risk_manager,
    std::unique_ptr<IExecutionHandler> execution_handler,
    std::unique_ptr<Portfolio> portfolio)
    : m_data_provider(std::move(data_provider)),
      m_signal_source(std::move(signal_source)),
      m_risk_manager(std::move(risk_manager)),
      m_execution_handler(std::move(execution_handler)),
      m_portfolio(std::move(portfolio)),
      m_peak_portfolio_value(m_portfolio->get_total_value()) // Initialize peak value
{}

void EventLoop::run_backtest() {
    std::cout << "--- Backtest Starting ---" << std::endl;
    std::cout << "Initial Portfolio Value: $" << m_portfolio->get_total_value() << std::endl;

    while (auto optional_bar = m_data_provider->get_next_bar()) {
        // 1. Get the latest data bar
        DataBar bar = *optional_bar;
        m_latest_prices[bar.symbol] = bar.close; // Update the latest known price

        // 2. Update Portfolio Value (Mark-to-Market)
        m_portfolio->recalculate_total_value(m_latest_prices);
        double current_value = m_portfolio->get_total_value();

        // 3. Update Historical Peak Value for Drawdown Calculation
        if (current_value > m_peak_portfolio_value) {
            m_peak_portfolio_value = current_value;
        }

        // 4. Get Signals
        // Convert bar to JSON for the signal source
        nlohmann::json market_data_json = {
            {"symbol", bar.symbol},
            {"close", bar.close}
            // Add other data points if models need them
        };
        m_signal_source->update_market_data(market_data_json);
        auto target_portfolio = m_signal_source->get_target_portfolio();

        // 5. Manage Risk
        auto approved_portfolio = m_risk_manager->validate_target(
            *m_portfolio,
            m_peak_portfolio_value,
            target_portfolio
        );

        // 6. Execute Trades
        m_execution_handler->execute_trades(
            *m_portfolio,
            approved_portfolio,
            m_latest_prices
        );

        // Log portfolio value at each step
        // std::cout << "Timestamp: " << ... << " Value: $" << current_value << std::endl;
    }

    // Final mark-to-market to get the final value
    m_portfolio->recalculate_total_value(m_latest_prices);

    std::cout << "--- Backtest Finished ---" << std::endl;
    std::cout << "Final Portfolio Value: $" << m_portfolio->get_total_value() << std::endl;
}