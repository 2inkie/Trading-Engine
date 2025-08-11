// src/execution/BacktestExecutionHandler.cpp

#include "execution/BacktestExecutionHandler.h"
#include "core/Portfolio.h"
#include "core/TradeOrder.h"
#include <set>
#include <cmath>

BacktestExecutionHandler::BacktestExecutionHandler(double commission, double slippage)
    : m_commission_per_trade(commission), m_slippage_percentage(slippage) {}

void BacktestExecutionHandler::execute_trades(
    Portfolio& portfolio,
    const std::map<std::string, double>& approved_target,
    const std::map<std::string, double>& latest_prices
) {
    // Get the total market value of the portfolio before any trades
    double total_value = portfolio.get_total_value();

    // Establish the universe of assets to consider
    std::set<std::string> asset_universe;
    for (const auto& pair : portfolio.get_holdings()) {
        asset_universe.insert(pair.first);
    }
    for (const auto& pair : approved_target) {
        asset_universe.insert(pair.first);
    }

    // Iterate through each asset and reconcile positions
    for (const std::string& symbol : asset_universe) {
        // Find the latest price for this asset
        auto price_it = latest_prices.find(symbol);
        if (price_it == latest_prices.end()) {
            continue; // Cannot trade without a price
        }
        double latest_price = price_it->second;

        // Target position in shares
        double target_weight = approved_target.count(symbol) ? approved_target.at(symbol) : 0.0;
        double target_dollar_value = total_value * target_weight;

        if (latest_price <= 0.0) {
            continue;
        }
        long long target_shares = static_cast<long long>(std::round(target_dollar_value / latest_price));

        // Current position in shares
        long long current_shares = portfolio.get_position(symbol);

        // Trade delta
        long long shares_to_trade = target_shares - current_shares;

        if (shares_to_trade == 0) {
            continue; // No trade needed
        }

        OrderSide side = (shares_to_trade > 0) ? OrderSide::Buy : OrderSide::Sell;
        long long trade_quantity = std::abs(shares_to_trade);
        
        double execution_price = 0.0;
        if (side == OrderSide::Buy) {
            // Slippage (higher)
            execution_price = latest_price * (1.0 + m_slippage_percentage);
        } else { // Sell
            // Slippage (lower)
            execution_price = latest_price * (1.0 - m_slippage_percentage);
        }

        double trade_value = trade_quantity * execution_price;

        portfolio.update_holding(symbol, shares_to_trade);

        if (side == OrderSide::Buy) {
            portfolio.update_cash(-trade_value - m_commission_per_trade);
        } else { // Sell
            portfolio.update_cash(trade_value - m_commission_per_trade);
        }
    }
}