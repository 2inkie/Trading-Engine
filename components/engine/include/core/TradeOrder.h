// include/core/TradeOrder.h

#pragma once
#include <string>

enum class OrderSide { Buy, Sell };

class TradeOrder {
    public:
        std::string symbol;
        long long quantity;
        OrderSide side;

        TradeOrder(const std::string& sym, long long qty, OrderSide s)
            : symbol(sym), quantity(qty), side(s) {}
};