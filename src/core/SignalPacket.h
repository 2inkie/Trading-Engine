// SignalPacket.h

#pragma once
#include <string>

enum class SignalType { Long, Short, Flat };

struct SignalPacket {
    std::string symbol;      // Asset identifier (eg AAPL)
    SignalType signal_type; // "long", "short" or "flat"
    double target_weight;    // Desired allocation of the portfolio for this asset
    double confidence;       // Confidence level of the signal (0.0 to 1.0)

    SignalPacket(const std::string& sym, SignalType type, double weight, double conf)
        : symbol(sym), signal_type(type), target_weight(weight), confidence(conf) {}
};