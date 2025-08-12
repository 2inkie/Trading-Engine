cmake_minimum_required(VERSION 3.20)
project(TradingEngine VERSION 1.0 LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
find_package(cppzmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# --- Detect cppzmq target name ---
if(TARGET cppzmq::cppzmq)
    set(CPPZMQ_TARGET cppzmq::cppzmq)
elseif(TARGET cppzmq)
    set(CPPZMQ_TARGET cppzmq)
else()
    message(FATAL_ERROR "cppzmq package found, but no known target available.")
endif()

# --- Detect ZeroMQ target name ---
if(TARGET ZeroMQ::ZeroMQ)
    set(ZMQ_TARGET ZeroMQ::ZeroMQ)
elseif(TARGET libzmq)
    set(ZMQ_TARGET libzmq)
elseif(TARGET libzmq-static)
    set(ZMQ_TARGET libzmq-static)
else()
    message(FATAL_ERROR "ZeroMQ package found, but no known target available.")
endif()

# --- Add source files ---
add_executable(engine
    src/main.cpp
    # Add other source files here
)

# --- Link dependencies ---
target_link_libraries(engine
    PUBLIC
        ${CPPZMQ_TARGET}
        ${ZMQ_TARGET}
        nlohmann_json::nlohmann_json
        unofficial::sqlite3::sqlite3
)

# --- Include dirs ---
target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# --- Optional warnings ---
if(MSVC)
    target_compile_options(engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -pedantic)
endif()
