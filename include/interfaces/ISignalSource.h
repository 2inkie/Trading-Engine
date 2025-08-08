// include/interfaces/ISignalSource.h

#pragma once
#include "core/SignalPacket.h"

#include <map>
#include <string>

/**
 * @class ISignalSource
 * @brief The "Job Description" for any component that generates trading signals.
 *
 * This interface defines the contract for a signal generation module. Its sole
 * responsibility is to receive market data and produce a complete target portfolio.
 */
class ISignalSource {
public:
    virtual ~ISignalSource() = default;

    /**
     * @brief The core function of the Signal Source role.
     * @param latest_bar The most recent market data to analyze.
     * @return A map defining the ideal portfolio (asset symbol -> target weight).
     */
    virtual std::map<std::string, double> get_target_portfolio(const SignalPacket& signal) = 0;
};