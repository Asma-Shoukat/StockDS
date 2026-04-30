# 📊 StockFlow (SmartInventory)

StockFlow is a **Data Structures & Algorithms-based Supply Chain Risk Management System**. It features a high-performance C++ backend powered by the Crow framework and a modern web-based frontend using Mustache templates. The application is designed to track inventory, analyze demand trends, manage incoming supplies, and prioritize risk items using classic data structures.

## 🚀 Features

- **Risk Dashboard**: Displays key metrics, flow trends, and a color-coded alert system for risk management.
- **Inventory Management**: Real-time product management view with SKU tracking, categorization, and velocity metrics.
- **Supply Chain Analytics**: Bottleneck analysis, demand trend forecasting (Moving Average), and active shipment tracking.
- **Dynamic Configuration**: Adjustable thresholds for low stock, critical stock, and forecast windows.
- **Time Simulation**: Built-in day counter with an automated background timer to simulate supply chain flow, sales, and arriving shipments over time.

## 🛠️ Tech Stack

- **Backend**: C++17
- **Web Framework**: [Crow](https://crowcpp.org/) (Fast C++ microframework for the web)
- **Templating Engine**: Mustache
- **Frontend**: HTML5, CSS3 (Custom Vanilla CSS), JavaScript
- **Build System**: CMake
- **Package Manager**: vcpkg
- **Data Storage**: Local CSV files (`data/`)

## 🧱 Core Data Structures Implemented

StockFlow is built heavily around core data structures to optimize operations:

- **Hash Map (`std::unordered_map`)**: Provides $O(1)$ lookup times for inventory items based on their SKU ID.
- **Priority Queue (`std::priority_queue`)**: Manages and sorts the risk alerts dynamically based on their calculated Risk Score (Max-Heap structure).
- **Queue (`std::queue`)**: Simulates the FIFO (First-In-First-Out) nature of the supply chain for processing incoming shipments.
- **Stack (`std::stack`)**: Powers the state management system, allowing for snapshots and "Undo" functionality to revert to previous days.

## 📂 Project Structure

```text
StockDS/
├── src/
│   └── main.cpp              # Core application logic & Crow routing
├── templates/                # Mustache HTML templates
│   ├── index.html            # Dashboard page
│   ├── inventory.html        # Inventory table view
│   ├── analysis.html         # Supply chain & bottleneck analysis
│   └── settings.html         # System configuration
├── data/                     # CSV data & settings
│   ├── inventory.csv         # Inventory dataset
│   ├── alerts.csv            # System alerts & warnings
│   ├── shipments.csv         # Incoming shipments data
│   ├── transactions.csv      # Historical transactions
│   ├── daily_sales.csv       # Daily sales records
│   └── dashboard_stats.csv   # High-level metrics
├── docs/                     # Project documentation (e.g., SRS Report)
└── CMakeLists.txt            # CMake configuration
```

## ⚙️ How to Build and Run

### Prerequisites

1. **C++17 Compiler** (e.g., MSVC, GCC, Clang)
2. **CMake** (v3.10 or higher)
3. **vcpkg** (Used for installing the Crow dependency)

### Build Instructions

1. Navigate to the project root directory.
2. Build the project using CMake:
   ```powershell
   cmake --build build --config Release
   ```

3. Run the executable:
   ```powershell
   .\build\Release\main.exe
   ```

4. Open your web browser and navigate to the application (typically `http://localhost:18080`, check console for the exact port).

## 🔮 Future Roadmap & Enhancements

Based on the SRS (Software Requirements Specification) analysis, the following features are planned for future updates:
- **Trie Data Structure**: For $O(L)$ smart search and autocomplete in the inventory view.
- **Knapsack Algorithm (Dynamic Programming)**: For an "Optimizer" module to calculate the best inventory restock cart within a strict budget.
- **Sliding Window Optimization**: Real-time advanced demand forecasting.
