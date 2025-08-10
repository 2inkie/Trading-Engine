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

    auto timestamp_ns = std::chrono::system_clock::time_point{} + 
                        std::chrono::nanoseconds(record.timestamp_epoch_ns);
    auto timestamp_for_databar = std::chrono::time_point_cast<std::chrono::system_clock::duration>(timestamp_ns);

    return DataBar{
        symbol_str,
        timestamp_for_databar,
        record.open,
        record.high,
        record.low,
        record.close,
        record.volume
    };
}