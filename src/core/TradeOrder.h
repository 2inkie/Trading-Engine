// TradeOrder.h

#pragma once
#include <string>

enum class OrderSide { Buy, Sell };

struct TradeOrder {
    std::string symbol;      // Asset identifier (eg AAPL)
    double quantity;         // Number of shares/contracts to trade
    double price;            // Price at which to execute the trade
    OrderSide side;          // Buy or Sell

    TradeOrder(const std::string& sym, double qty, double prc, OrderSide side)
        : symbol(sym), quantity(qty), price(prc), side(side) {}
};