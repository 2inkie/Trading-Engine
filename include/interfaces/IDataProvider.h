// include/interfaces/IDataProvider.h

#pragma once
#include "core/DataBar.h"
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
    virtual std::optional<DataBar> get_next_bar() = 0;
};