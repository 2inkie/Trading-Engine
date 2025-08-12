// include/interfaces/ISignalSource.h

#pragma once
#include <map>
#include <string>

// Forward-declare to avoid including the full JSON header in an interface file
namespace nlohmann {
    template<typename, typename, typename, bool, typename, typename>
    class basic_json;
    using json = basic_json<std::map, std::vector, std::string, bool, std::int64_t, std::uint64_t>;
}

class ISignalSource {
public:
    virtual ~ISignalSource() = default;

    /**
     * @brief Updates the source with the latest market data.
     * The EventLoop calls this first to provide the necessary data.
     * @param market_data A JSON object containing the latest market data.
     */
    virtual void update_market_data(const nlohmann::json& market_data) = 0;

    /**
     * @brief Triggers communication and returns the aggregated portfolio.
     * The EventLoop calls this second, after updating the market data.
     * @return A map defining the final, aggregated target portfolio.
     */
    virtual std::map<std::string, double> get_target_portfolio() = 0;
};