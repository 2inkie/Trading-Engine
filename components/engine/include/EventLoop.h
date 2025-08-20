// include/EventLoop.h

#pragma once

#include "interfaces/IDataProvider.h"
#include "interfaces/ISignalSource.h"
#include "interfaces/IRiskManager.h"
#include "interfaces/IExecutionHandler.h"
#include "core/Portfolio.h"
#include <memory>

/**
 * @class EventLoop
 * @brief Orchestrates the entire backtesting process.
 *
 * This class owns all the core components of the trading system (via their
 * interfaces) and drives the simulation forward, one data bar at a time.
 */
class EventLoop {
public:
    /**
     * @brief Constructs the EventLoop, taking ownership of all components.
     * This uses dependency injection to decouple the loop from concrete implementations.
     */
    EventLoop(
        std::unique_ptr<IDataProvider> data_provider,
        std::unique_ptr<ISignalSource> signal_source,
        std::unique_ptr<IRiskManager> risk_manager,
        std::unique_ptr<IExecutionHandler> execution_handler,
        std::unique_ptr<Portfolio> portfolio
    );

    // The main entry point to start the simulation.
    void run_backtest();

private:
    // --- Core Components ---
    std::unique_ptr<IDataProvider> m_data_provider;
    std::unique_ptr<ISignalSource> m_signal_source;
    std::unique_ptr<IRiskManager> m_risk_manager;
    std::unique_ptr<IExecutionHandler> m_execution_handler;
    std::unique_ptr<Portfolio> m_portfolio;

    // --- Backtest State ---
    double m_peak_portfolio_value;
    std::map<std::string, double> m_latest_prices;
};