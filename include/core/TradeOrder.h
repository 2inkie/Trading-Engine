// include/core/TradeOrder.h

#pragma once
#include <string>

enum class OrderSide { Buy, Sell };

struct TradeOrder {
    std::string symbol;      // Asset identifier (eg AAPL)
    double quantity;         // Number of shares/contracts to trade
    OrderSide side;          // Buy or Sell

    TradeOrder(const std::string& sym, double qty, OrderSide side)
        : symbol(sym), quantity(qty), side(side) {}
};