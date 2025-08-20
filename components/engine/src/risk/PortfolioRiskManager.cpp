// src/risk/PortfolioRiskManager.cpp

#include "risk/PortfolioRiskManager.h"
#include "core/Portfolio.h"
#include <numeric>
#include <iostream>

PortfolioRiskManager::PortfolioRiskManager(double max_pos_weight, double max_leverage, double max_drawdown)
    : m_max_pos_weight(max_pos_weight), m_max_leverage(max_leverage), m_max_drawdown(max_drawdown) {}

std::map<std::string, double> PortfolioRiskManager::validate_target(
    const Portfolio& current_portfolio,
    double peak_portfolio_value,
    const std::map<std::string, double>& target_portfolio) {

    // --- Max Drawdown Rule ---
    double current_value = current_portfolio.get_total_value();
    if (peak_portfolio_value > 0) {
        double drawdown = (peak_portfolio_value - current_value) / peak_portfolio_value;
        if (drawdown > m_max_drawdown) {
            std::cout << "[RiskManager] CRITICAL: Max drawdown of " << m_max_drawdown * 100
                      << "% exceeded. Current drawdown: " << drawdown * 100
                      << "%. FLATTENING ALL POSITIONS." << std::endl;
            // Return "sell everything".
            return {};
        }
    }

    auto approved_portfolio = target_portfolio;
    std::cout << "[RiskManager] Starting validation of target portfolio..." << std::endl;

    // --- Concentration Risk ---
    for (auto& position : approved_portfolio) {
        if (position.second > m_max_pos_weight) {
            std::cout << "[RiskManager] WARNING: Position " << position.first
                      << " target weight (" << position.second * 100 << "%)"
                      << " exceeds max allowed (" << m_max_pos_weight * 100 << "%). Scaling down."
                      << std::endl;
            position.second = m_max_pos_weight;
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

    if (total_weight > m_max_leverage) {
        std::cout << "[RiskManager] WARNING: Total target weight (" << total_weight * 100 << "%)"
                  << " exceeds max leverage (" << m_max_leverage * 100 << "%). Scaling all positions."
                  << std::endl;

        double scaling_factor = m_max_leverage / total_weight;
        for (auto& position : approved_portfolio) {
            position.second *= scaling_factor;
        }
    }

    std::cout << "[RiskManager] Validation complete. Portfolio is compliant." << std::endl;
    return approved_portfolio;
}