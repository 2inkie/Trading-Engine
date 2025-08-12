// src/main.cpp

#include "EventLoop.h"
#include "core/Portfolio.h"
#include "data/BinFileReader.h"
#include "signals/AggregatedIPCSource.h"
#include "risk/PortfolioRiskManager.h"
#include "execution/BacktestExecutionHandler.h"
#include <iostream>
#include <vector>
#include <string>
#include <memory>

int main() {
    // --- 1. Configuration ---
    // This section would is be loaded from a config file (eg JSON)
    const double initial_cash = 100000.0;
    const std::string data_file_path = "data/AAPL.bin";

    // IPC configuration for Python models
    const std::vector<std::string> model_endpoints = {"tcp://localhost:5555", "tcp://localhost:5556"};
    const std::chrono::milliseconds reply_timeout(100); // 100ms timeout

    // Risk and Execution parameters
    const double max_position_weight = 0.25;
    const double max_leverage = 1.0;
    const double max_drawdown = 0.20;
    const double commission_per_trade = 1.00;
    const double slippage_percentage = 0.0005; // 0.05% slippage

    // --- 2. Component Assembly (Dependency Injection) ---
    auto portfolio = std::make_unique<Portfolio>(initial_cash);

    auto data_provider = std::make_unique<BinFileReader>(data_file_path);

    auto signal_source = std::make_unique<AggregatedIPCSource>(model_endpoints, reply_timeout);

    auto risk_manager = std::make_unique<PortfolioRiskManager>(
        max_position_weight, max_leverage, max_drawdown
    );

    auto execution_handler = std::make_unique<BacktestExecutionHandler>(
        commission_per_trade, slippage_percentage
    );

    // --- 3. Create and Run the Event Loop ---
    // We pass ownership of all our components to the EventLoop
    EventLoop event_loop(
        std::move(data_provider),
        std::move(signal_source),
        std::move(risk_manager),
        std::move(execution_handler),
        std::move(portfolio)
    );

    try {
        event_loop.run_backtest();
    } catch (const std::exception& e) {
        std::cerr << "An unhandled exception occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown exception occurred." << std::endl;
        return 1;
    }

    return 0;
}