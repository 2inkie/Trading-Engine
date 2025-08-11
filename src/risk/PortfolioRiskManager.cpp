// src/risk/PortfolioRiskManager.cpp

#include "risk/PortfolioRiskManager.h"
#include "core/Portfolio.h"
#include <numeric>
#include <iostream>

std::map<std::string, double> PortfolioRiskManager::validate_target(
    const Portfolio& current_portfolio,
    const std::map<std::string, double>& target_portfolio) {
    
    // --- Configuration ---
    const double MAX_POSITION_WEIGHT = 0.25; // Max 25% allocation to any single asset.
    const double MAX_TOTAL_LEVERAGE = 1.0;   // No leverage, portfolio weights must sum to <= 1.0.

    auto approved_portfolio = target_portfolio;

    std::cout << "[RiskManager] Starting validation of target portfolio..." << std::endl;

    // --- Concentration Risk ---
    for (auto& position : approved_portfolio) {
        // 'position' is a pair: {asset_symbol, target_weight}
        if (position.second > MAX_POSITION_WEIGHT) {
            std::cout << "[RiskManager] WARNING: Position " << position.first 
                      << " target weight (" << position.second * 100 << "%)"
                      << " exceeds max allowed (" << MAX_POSITION_WEIGHT * 100 << "%). Scaling down." 
                      << std::endl;
            // Scale down the single oversized position to the maximum allowed.
            position.second = MAX_POSITION_WEIGHT;
        }
    }

    // --- Leverage and Total Exposure Risk ---
    double total_weight = std::accumulate(
        approved_portfolio.begin(), 
        approved_portfolio.end(),
        0.0,
        [](double sum, const auto& map_pair) {
            return sum + map_pair.second;
        }
    );

    if (total_weight > MAX_TOTAL_LEVERAGE) {
        std::cout << "[RiskManager] WARNING: Total target weight (" << total_weight * 100 << "%)"
                  << " exceeds max leverage (" << MAX_TOTAL_LEVERAGE * 100 << "%). Scaling all positions."
                  << std::endl;
        
        // Calculate the factor needed to scale the entire portfolio back to the max leverage
        double scaling_factor = MAX_TOTAL_LEVERAGE / total_weight;
        for (auto& position : approved_portfolio) {
            position.second *= scaling_factor;
        }
    }

    // `Approved_portfolio` is compliant with our rules.
    std::cout << "[RiskManager] Validation complete. Portfolio is compliant." << std::endl;

    return approved_portfolio;
}