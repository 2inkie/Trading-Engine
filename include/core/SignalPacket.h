// include/core/SignalPacket.h

#pragma once
#include <string>
#include <nlohmann/json.hpp>

enum class SignalType { Long, Short, Flat };

struct SignalPacket {
    std::string symbol;      // Asset identifier (eg AAPL)
    SignalType signal_type; // "long", "short" or "flat"
    double target_weight;    // Desired allocation of the portfolio for this asset
    double confidence;       // Confidence level of the signal (0.0 to 1.0)

    SignalPacket() = default;

    SignalPacket(const std::string& sym, SignalType type, double weight, double conf)
        : symbol(sym), signal_type(type), target_weight(weight), confidence(conf) {}
};

// This macro tells the nlohmann::json library how to convert strings to SignalType enum
NLOHMANN_JSON_SERIALIZE_ENUM(SignalType, {
    {SignalType::Long, "LONG"},
    {SignalType::Short, "SHORT"},
    {SignalType::Flat, "FLAT"},
})

inline void from_json(const nlohmann::json& j, SignalPacket& p) {
    j.at("symbol").get_to(p.symbol);
    j.at("signal_type").get_to(p.signal_type);
    j.at("target_weight").get_to(p.target_weight);
    j.at("confidence").get_to(p.confidence);
}