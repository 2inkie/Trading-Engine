// IDataProvider.h

#pragma once
#include "core/DataBar.h"
#include <optional>

/**
 * @class IDataProvider
 * @brief An abstract interface for any class that provides sequential market data.
 *
 * This class defines a "contract". Any class that implements it must provide a way
 * to get the next DataBar. This decouples the trading logic from the data source.
 */

 
class IDataProvider {
public:
    // A virtual destructor is essential for any base class.
    virtual ~IDataProvider() = default;

    // The core function of the interface.
    virtual std::optional<DataBar> get_next_bar() = 0;
};