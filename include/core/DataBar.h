// include/core/DataBar.h

#pragma once

#include <string>
#include <cstdint>
#include <chrono>

struct DataBar {
    std::string symbol;                                              // Asset identifier (eg AAPL)
    std::chrono::time_point<std::chrono::system_clock> timestamp;    // Timestamp of the bar
    double open;
    double high;
    double low;
    double close;
    uint64_t volume;                                                 // Volume

    DataBar(const std::string& sym, std::chrono::time_point<std::chrono::system_clock> ts, double o, double h, double l, double c, uint64_t vol)
        : symbol(sym), timestamp(ts), open(o), high(h), low(l), close(c), volume(vol) {}

};