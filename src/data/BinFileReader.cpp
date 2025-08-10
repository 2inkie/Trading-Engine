// src/data/BinFileReader.cpp

#include "data/BinFileReader.h"
#include <stdexcept>
#include <chrono>

struct DataBarRecord {
    char symbol[16];
    uint64_t timestamp_epoch_ns;
    double open;
    double high;
    double low;
    double close;
    uint64_t volume;
};

BinFileReader::BinFileReader(const std::string& file_path)
    : m_file_stream(file_path, std::ios::binary) {
    if (!m_file_stream.is_open()) {
        throw std::runtime_error("Failed to open file: " + file_path);
    }
}

BinFileReader::~BinFileReader() {
    if (m_file_stream.is_open()) {
        m_file_stream.close();
    }
}

std::optional<DataBar> BinFileReader::get_next_bar() {
    DataBarRecord record;

    if (!m_file_stream.read(reinterpret_cast<char*>(&record), sizeof(DataBarRecord))) {
        if (m_file_stream.eof()) {
            return std::nullopt; // Clean end of file
        } else {
            throw std::runtime_error("Error reading from binary file");
        }
    }

    std::string symbol_str(record.symbol);
    auto timestamp = std::chrono::system_clock::time_point{} + 
                    std::chrono::nanoseconds(record.timestamp_epoch_ns);

    return DataBar{
        .symbol = symbol_str,
        .timestamp = timestamp,
        .open = record.open,
        .high = record.high,
        .low = record.low,
        .close = record.close,
        .volume = record.volume
    };
}