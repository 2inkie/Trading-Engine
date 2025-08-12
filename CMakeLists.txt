cmake_minimum_required(VERSION 3.20)
project(TradingEngine VERSION 1.0 LANGUAGES CXX)

# --- Project Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- Dependency Management ---
find_package(cppzmq CONFIG REQUIRED)
find_package(ZeroMQ CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# --- Robust Target Detection ---
if(TARGET cppzmq::cppzmq)
    set(CPPZMQ_TARGET cppzmq::cppzmq)
elseif(TARGET cppzmq)
    set(CPPZMQ_TARGET cppzmq)
else()
    message(FATAL_ERROR "cppzmq package found, but no known target available.")
endif()

if(TARGET ZeroMQ::ZeroMQ)
    set(ZMQ_TARGET ZeroMQ::ZeroMQ)
elseif(TARGET libzmq)
    set(ZMQ_TARGET libzmq)
elseif(TARGET libzmq-static)
    set(ZMQ_TARGET libzmq-static)
else()
    message(FATAL_ERROR "ZeroMQ package found, but no known target available.")
endif()


# --- Executable Definition ---
add_executable(engine src/main.cpp)


# --- Source File Management ---
file(GLOB_RECURSE ENGINE_SOURCES "src/*.cpp")
target_sources(engine PRIVATE ${ENGINE_SOURCES})


# --- Linking and Include Directories ---
target_link_libraries(engine
    PUBLIC
        ${CPPZMQ_TARGET}
        ${ZMQ_TARGET}
        nlohmann_json::nlohmann_json
        unofficial::sqlite3::sqlite3
)

target_include_directories(engine
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)


# --- Compiler Warnings ---
if(MSVC)
    target_compile_options(engine PRIVATE /W4 /permissive-)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -pedantic)
endif()