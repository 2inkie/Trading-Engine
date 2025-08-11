// include/risk/PortfolioRiskManager.h

#pragma once

#include "interfaces/IRiskManager.h"

class PortfolioRiskManager : public IRiskManager {
    public:
        /**
         * @brief Validates a target portfolio against risk rules.
         * This function takes the current portfolio and the proposed target, and
         * returns a final, risk-approved target portfolio. This allows the risk
         * manager to scale down positions rather than just rejecting a plan outright.
         * @param current_portfolio The current state of the portfolio.
         * @param target_portfolio The proposed target portfolio from the signal source.
         * @return A risk-approved target portfolio.
         */
        std::map<std::string, double> validate_target(
            const Portfolio& current_portfolio,
            const std::map<std::string, double>& target_portfolio) override;
};