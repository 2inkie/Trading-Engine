# Set the minimum CMake version required
cmake_minimum_required(VERSION 3.15)

# Set the project name and C++ standard
project(TradingEngine VERSION 1.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ZeroMQ_DIR "C:/dev/vcpkg/installed/x64-windows/share/zeromq")
set(cppzmq_DIR "C:/dev/vcpkg/installed/x64-windows/share/cppzmq")
set(nlohmann_json_DIR "C:/dev/vcpkg/installed/x64-windows/share/nlohmann_json")
set(unofficial-sqlite3_DIR "C:/dev/vcpkg/installed/x64-windows/share/unofficial-sqlite3")

find_package(ZeroMQ CONFIG REQUIRED)
find_package(cppzmq CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

link_directories("C:/dev/vcpkg/installed/x64-windows/lib")

add_executable(engine src/main.cpp)

target_compile_definitions(engine PRIVATE SQLITE_DLL)

target_link_libraries(engine PRIVATE
    libzmq
    nlohmann_json::nlohmann_json 
    ${unofficial-sqlite3_LIBRARIES}
)

target_include_directories(engine PUBLIC include)