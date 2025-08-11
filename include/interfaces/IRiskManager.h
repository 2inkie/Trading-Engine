// include/interfaces/IRiskManager.h

#pragma once

#include <map>
#include <string>

// Forward-declaration of the Portfolio class.
class Portfolio;

/**
 * @class IRiskManager
 * @brief The "Job Description" for any component that manages portfolio risk.
 *
 * This interface defines the contract for a risk management module. Its primary
 * role is to inspect a proposed target portfolio, check it against a set of
 * rules, and either approve it or modify it to be compliant.
 */
class IRiskManager {
public:
    virtual ~IRiskManager() = default;

    /**
     * @brief Validates a target portfolio against risk rules.
     * This function takes the current portfolio and the proposed target, and
     * returns a final, risk-approved target portfolio. This allows the risk
     * manager to scale down positions rather than just rejecting a plan outright.
     * @param current_portfolio The current state of the portfolio.
     * @param target_portfolio The proposed target portfolio from the signal source.
     * @return A risk-approved target portfolio.
     */
    virtual std::map<std::string, double> validate_target(
        const Portfolio& current_portfolio,
        double peak_portfolio_value,
        const std::map<std::string, double>& target_portfolio) = 0;
};