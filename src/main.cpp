#include <iostream>
#include <zmq.hpp>
#include <nlohmann/json.hpp>
#include <sqlite3.h>

int main() {
    nlohmann::json j;
    j["test"] = "success";
    std::cout << "JSON library linked: " << j.dump() << std::endl;

    std::cout << "SQLite3 library linked: version " << sqlite3_libversion() << std::endl;

    zmq::context_t ctx;
    std::cout << "ZeroMQ library linked." << std::endl;

    std::cout << "All dependencies linked successfully!" << std::endl;
    return 0;
}