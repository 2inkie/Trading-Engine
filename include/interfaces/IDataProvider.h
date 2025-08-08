// include/interfaces/IDataProvider.h

#pragma once
#include <optional>
#include <map>
#include <string>

/**
 * @class IDataProvider
 * @brief An abstract interface for any class that provides sequential market data.
 *
 * This class defines a "contract".
 */


class IDataProvider {
public:
    // A virtual destructor.
    virtual ~IDataProvider() = default;

    // The core function of the interface.
    virtual std::map<std::string, double> get_target_portfolio() = 0;
};