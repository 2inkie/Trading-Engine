# Trading-Engine

Modular C++ trading engine designed to backtest and execute trading strategies based on signals from an ensemble of external Python models. This engine is built for robustness and extensibility.

## How It Works: Component Functions
The engine is built on a set of decoupled components, each with a single responsibility. They are orchestrated by a central `EventLoop`.

* **`EventLoop`**: It drives the simulation forward bar-by-bar, calling each of the other components in the correct sequence to process data, generate signals, manage risk, and execute trades.

* **`Portfolio`**: A thread-safe state manager that holds the system's financial state. It tracks available cash, current asset holdings, and the total mark-to-market value of the account.

* **`DataProvider`**: Responsible for reading historical market data from binary files (`.bin`) and feeding it to the `EventLoop` one bar at a time. It's the bridge between stored data and the live simulation.

* **`SignalSource`**: Manages all communication with the external Python models using ZeroMQ. It sends the latest market data to all models, collects their `SignalPacket` replies, and aggregates them into a single, final **Target Portfolio** using a confidence-weighted algorithm (it will be changed in the future).

* **`RiskManager`**: Acts as the final safety check. It takes the **Target Portfolio** proposed by the models and shapes it to comply with a set of pre-configured rules (eg, max drawdown, max position concentration, max leverage), preventing catastrophic actions.

* **`ExecutionHandler`**: The final step, it compares the current portfolio with the risk-approved target, calculates the exact number of shares to buy or sell, and updates the `Portfolio` state while simulating real-world costs like commissions and slippage.

## File Structure
The project uses a separated structure for header and source files, making it easy to navigate and maintain.

```
trading_engine/
├── build/
├── config/
│   └── backtest_config.json
├── data/
│   └── AAPL.bin
├── include/
│   ├── core/
│   │   ├── DataBar.h
│   │   ├── Portfolio.h
│   │   ├── SignalPacket.h
│   │   └── TradeOrder.h
│   ├── interfaces/
│   │   ├── IDataProvider.h
│   │   ├── IExecutionHandler.h
│   │   ├── IRiskManager.h
│   │   └── ISignalSource.h
│   ├── data/
│   │   └── BinFileReader.h
│   ├── execution/
│   │   └── BacktestExecutionHandler.h
│   ├── risk/
│   │   └── PortfolioRiskManager.h
│   ├── signals/
│   │   └── AggregatedIPCSource.h
│   └── EventLoop.h
├── src/
│   ├── core/
│   │   └── Portfolio.cpp
│   ├── data/
│   │   └── BinFileReader.cpp
│   ├── execution/
│   │   └── BacktestExecutionHandler.cpp
│   ├── risk/
│   │   └── PortfolioRiskManager.cpp
│   ├── signals/
│   │   └── AggregatedIPCSource.cpp
│   ├── EventLoop.cpp
│   └── main.cpp
├── tests/
└── CMakeLists.txt
```

## How to Build
This project uses CMake and requires a C++ package manager (like vcpkg) for its dependencies (ZeroMQ, nlohmann-json, SQLite).

1.  Clone the repository.

2.  Configure CMake (from the root directory), pointing it to the package manager's toolchain file.
    ```bash
    # Replace [path-to-vcpkg] with the actual vcpkg installation path
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
    ```

3.  Build the project.
    ```bash
    cmake --build build
    ```

The final executable, `engine`, will be located in the `build` directory.