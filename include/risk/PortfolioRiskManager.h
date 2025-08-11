// include/risk/PortfolioRiskManager.h

#pragma once

#include "interfaces/IRiskManager.h"

class PortfolioRiskManager : public IRiskManager {
    public:
        /**
         * @brief Constructs the risk manager with specific risk parameters.
         * @param max_pos_weight The max allocation to any single asset (e.g., 0.25 for 25%).
         * @param max_leverage The max total portfolio weight (e.g., 1.0 for no leverage).
         * @param max_drawdown The max allowed loss from the peak portfolio value (e.g., 0.15 for 15%).
         */

        PortfolioRiskManager(double max_pos_weight, double max_leverage, double max_drawdown);
        std::map<std::string, double> validate_target(
            const Portfolio& current_portfolio,
            double peak_portfolio_value,
            const std::map<std::string, double>& target_portfolio
        ) override;

    private:
        double m_max_pos_weight;
        double m_max_leverage;
        double m_max_drawdown;
};