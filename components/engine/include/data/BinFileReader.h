// include/data/BinFileReader.h

#pragma once

#include "interfaces/IDataProvider.h"
#include <string>
#include <fstream>

class BinFileReader : public IDataProvider {
    public:
        explicit BinFileReader(const std::string& file_path);
        ~BinFileReader() override;

        std::optional<DataBar> get_next_bar() override;
    
    private:
        std::ifstream m_file_stream;
};