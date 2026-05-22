

# ⬡ &nbsp; S T O C K F L O W

### *Supply Chain Risk Engine · Powered by Data Structures*

<br/>

<a href="https://github.com/Asma-Shoukat/StockDS"><img src="https://img.shields.io/badge/⭐_Star_This_Repo-181717?style=for-the-badge&logo=github" alt="Star"/></a>
&nbsp;
<img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17"/>
&nbsp;
<img src="https://img.shields.io/badge/Crow-HTTP_Framework-4A90D9?style=for-the-badge&logo=c&logoColor=white" alt="Crow"/>
&nbsp;
<img src="https://img.shields.io/badge/CMake-3.10+-064F8C?style=for-the-badge&logo=cmake&logoColor=white" alt="CMake"/>
&nbsp;
<img src="https://img.shields.io/badge/Server-Online_🟢-10b981?style=for-the-badge" alt="Online"/>

<br/><br/>

<table>
<tr>
<td align="center"><b>990+</b><br/><sub>SKUs Indexed</sub></td>
<td align="center"><b>1,458</b><br/><sub>Risk Alerts</sub></td>
<td align="center"><b>587</b><br/><sub>Shipments Queued</sub></td>
<td align="center"><b>22,558</b><br/><sub>Transactions</sub></td>
<td align="center"><b>5</b><br/><sub>Data Structures</sub></td>
<td align="center"><b>O(1)</b><br/><sub>SKU Lookup</sub></td>
</tr>
</table>

<br/>

*A real-time supply chain risk engine demonstrating the practical power of classic Data Structures.*  
*Built with a high-performance **C++ backend** (Crow) and a premium **glassmorphism web frontend**.*

</div>

---

## 📌 &nbsp; Table of Contents

<div align="center">

[✨ Features](#-features) &nbsp;·&nbsp;
[🧱 Data Structures](#-data-structures) &nbsp;·&nbsp;
[🏗️ Architecture](#️-architecture) &nbsp;·&nbsp;
[🖥️ Interface](#️-system-interface) &nbsp;·&nbsp;
[⚙️ Setup](#️-setup) &nbsp;·&nbsp;
[🌐 API](#-api-reference) &nbsp;·&nbsp;
[🔮 Roadmap](#-roadmap)

</div>

---

## ✨ &nbsp; Features

<div align="center">

| | Feature | Description |
|:---:|:---|:---|
| 🗓️ | **Day Simulation** | Advance time — triggers sales, shipment arrivals & queue refills |
| ⏮️ | **Time Travel (Undo)** | Roll back to any previous day via a LIFO Stack of full snapshots |
| 🚨 | **Risk Alert Engine** | Max-Heap Priority Queue surfaces highest-risk items at O(1) |
| 📦 | **Inventory Tracking** | O(1) SKU lookup via Hash Map across 990+ product records |
| 🚚 | **FIFO Supply Chain** | Shipments processed First-In-First-Out via `std::queue` |
| 📉 | **Demand Forecasting** | Sliding Window computes 5–7 day moving average + trend direction |
| ⚙️ | **Live Settings** | Thresholds & forecast window — saved to disk, live-applied |
| 🤖 | **Auto-Advance Timer** | Background `std::thread` simulates real-time logistics every 5s |

</div>

---

## 🧱 &nbsp; Data Structures

> Every data structure was chosen for a **concrete algorithmic reason** — not convenience.

<br/>

<details open>
<summary><b>🗺️ &nbsp; Hash Map — O(1) Inventory Lookup</b></summary>
<br/>

```cpp
unordered_map<string, StockItem> inventoryMap;
// Constant-time product access by SKU ID across 990 items
StockItem* getProductById(string sku_id) {
    if (inventoryMap.find(sku_id) != inventoryMap.end())
        return &inventoryMap[sku_id];
    return nullptr;
}
```
A `vector` would cost **O(n)** per search. The Hash Map keeps it **O(1)** regardless of inventory size.

</details>

<details>
<summary><b>⚠️ &nbsp; Priority Queue — Risk-Sorted Max-Heap</b></summary>
<br/>

```cpp
priority_queue<Alert> alertQueue;   // Max-Heap by risk_score
// Risk score formula:
if (item.status == "Critical")  item.risk_score = 80 + (item.velocity / 10);
else if (item.status == "Low")  item.risk_score = 50 + (item.velocity / 10);
else                            item.risk_score = 20;
```
High-velocity + low-stock items float to the top automatically. Critical alerts are **never buried**.

</details>

<details>
<summary><b>🚚 &nbsp; Queue — FIFO Shipment Pipeline</b></summary>
<br/>

```cpp
queue<Shipment> shipmentQueue;
// Shipments ordered first → arrive first. Mirrors real logistics.
Shipment processNextShipment() {
    Shipment next = shipmentQueue.front();
    shipmentQueue.pop();
    inventoryMap[next.product_id].quantity += next.quantity;
    return next;
}
```

</details>

<details>
<summary><b>⏮️ &nbsp; Stack — Full System Undo</b></summary>
<br/>

```cpp
stack<SystemState> historyStack;
// Each day-advance pushes a full snapshot
void saveStateToHistory() { historyStack.push(state); }
// One pop = instant rollback to previous day
bool undoLastAction() {
    SystemState prev = historyStack.top(); historyStack.pop();
    inventoryList = prev.inventory_snapshot;  // full restore
    return true;
}
```

</details>

<details>
<summary><b>📉 &nbsp; Sliding Window — Demand Trend Detection</b></summary>
<br/>

```cpp
// Splits the window in half — compares averages to detect trend
if (secondAvg > firstAvg * 1.05)       return "increasing";
else if (secondAvg < firstAvg * 0.95)  return "decreasing";
else                                    return "stable";
```
Analyzes the last **K days** of sales. Smooths outliers to predict future demand accurately.

</details>

<br/>

### Complexity Summary

| Data Structure | Operation | Complexity |
|:---|:---|:---:|
| `std::unordered_map` | SKU Lookup / Update | **O(1)** avg |
| `std::priority_queue` | Insert Alert / Peek Top | **O(log N)** / **O(1)** |
| `std::queue` | Enqueue / Dequeue Shipment | **O(1)** |
| `std::stack` | Push Snapshot / Pop Undo | **O(1)** |
| Sliding Window | Demand Forecast | **O(K)** |

---

## 🏗️ &nbsp; Architecture

<div align="center">

![System Architecture](docs/screenshots/architecture.png)

</div>

<br/>

The system follows a clean **3-layer architecture**: a browser-based frontend communicates with a multithreaded **Crow C++ HTTP server** over REST/JSON, which delegates all logic to the **DSA Core Engine** backed by CSV-loaded data files.

| Layer | Role |
|:---|:---|
| 🌐 **Frontend** | HTML5 + Chart.js — fetches JSON via Fetch API |
| ⚙️ **Server** | Crow (C++17) — port 8080 · 8 threads · REST routing |
| 🧠 **DSA Core** | `unordered_map`, `priority_queue`, `queue`, `stack`, Sliding Window |
| 💾 **Data** | CSV files loaded at startup — `inventory.csv`, `alerts.csv`, `shipments.csv` |

### &nbsp; Tech Stack

<div align="center">

| Layer | Technology |
|:---:|:---|
| ⚙️ **Language** | C++17 |
| 🌐 **HTTP Server** | [Crow Framework](https://crowcpp.org/) — multithreaded microframework |
| 🧩 **Templating** | Crow Mustache — server-side HTML rendering |
| 🎨 **Frontend** | HTML5, CSS3 (Glassmorphism dark theme), Vanilla JS |
| 📊 **Charts** | Chart.js via CDN |
| 🔤 **Fonts / Icons** | Google Fonts (Inter) + Material Symbols |
| 🔨 **Build** | CMake 3.10+ · vcpkg |
| 🧵 **Concurrency** | `std::thread` + `std::atomic<bool>` background timer |

</div>

---

## 🖥️ &nbsp; System Interface

> Premium dark glassmorphism SPA — collapsible sidebar, animated KPI cards, live Chart.js graphs.

<br/>

### &nbsp;`/` &nbsp; — &nbsp; Command Center Dashboard

The nerve center. Four live KPI cards, a Sales Flow Trend line chart, and an Inventory Distribution donut. Top-right status pill pulses 🟢 when the C++ server is online.

> **DSA:** KPIs pull from `inventoryList`; alert panel drains a copy of the `priority_queue` — highest-risk always first.

<div align="center">

![Dashboard](docs/screenshots/dashboard.png)

</div>

---

### &nbsp;`/inventory` &nbsp; — &nbsp; Warehouse View

Full searchable table of all **990 SKUs**. Each row shows quantity progress bar, velocity (units/day), and a color-coded status badge. Filter by `All` · `Stocked` · `Low Stock` · `Critical`.

> **DSA:** Every row backed by `inventoryMap` (Hash Map). Status thresholds are configurable live from Settings.

<div align="center">

![Inventory](docs/screenshots/inventory.png)

</div>

---

### &nbsp;`/analysis` &nbsp; — &nbsp; Analytics Lab

Six cards expose each data structure's live runtime metrics. Includes a Sliding Window Demand Forecast chart and Alert Distribution donut.

<div align="center">

| Card | Structure | Stats |
|:---:|:---|:---|
| 🗺️ | Hash Map | 990 indexed · <1µs lookup |
| ⚠️ | Priority Queue | 1,458 alerts · 465 high-priority |
| 📬 | FIFO Queue | 587 shipments · 24h avg processing |
| ⏮️ | Stack (LIFO) | 50 snapshots · 100% rollback success |
| 📉 | Sliding Window | 5-day window · 2,236 avg sales/day |
| 📈 | Trend Analysis | Current: **Stable** · 31 days analyzed |

</div>

<div align="center">

![Analysis](docs/screenshots/analysis.png)

![Analysis Charts](docs/screenshots/analysis_lower.png)

</div>

---

### &nbsp;`/settings` &nbsp; — &nbsp; Configuration Panel

Live-edit company name, stock thresholds, and forecast window. POSTs to `/api/save-settings` — updates global state and persists to `data/settings.cfg`.

<div align="center">

![Settings](docs/screenshots/settings.png)

</div>

---

## ⚙️ &nbsp; Setup

### Prerequisites

```
C++17 Compiler  ·  CMake 3.10+  ·  vcpkg  ·  Git
```

### Build & Run

```powershell
# 1 — Clone
git clone https://github.com/Asma-Shoukat/StockDS.git
cd StockDS

# 2 — Install Crow (first time only)
.\vcpkg\bootstrap-vcpkg.bat
.\vcpkg\vcpkg install crow

# 3 — Configure
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=.\vcpkg\scripts\buildsystems\vcpkg.cmake

# 4 — Build
cmake --build build --config Release

# 5 — Run
.\build\Release\main.exe
```

Then open **[http://localhost:8080](http://localhost:8080)** 🚀

<details>
<summary><b>Expected server output</b></summary>

```
========== Loading Data ==========
Loaded 990 items into Hash Map
Loaded 1458 alerts into Priority Queue
Loaded 587 shipments into Queue
Loaded 22558 transactions
Loaded 31 days of sales data
========== Data Loaded ==========

==========================================================
  STOCKFLOW SERVER - Data Structures Demo
==========================================================
    [+] Hash Map       - O(1) product lookup
    [+] Priority Queue - Risk-sorted alerts
    [+] Queue          - FIFO shipment processing
    [+] Stack          - Undo/History feature
    [+] Sliding Window - Demand forecasting
==========================================================
  Dashboard : http://localhost:8080/
  Inventory : http://localhost:8080/inventory
  Analysis  : http://localhost:8080/analysis
==========================================================
  [AUTO-TIMER] Days advance every 5 seconds
==========================================================
```

</details>

---

## 🌐 &nbsp; API Reference

<details>
<summary><b>Page Routes</b></summary>
<br/>

| Method | Route | Page |
|:---:|:---|:---|
| `GET` | `/` | Dashboard |
| `GET` | `/inventory` | Inventory table |
| `GET` | `/analysis` | DSA analytics |
| `GET` | `/settings` | Settings form |

</details>

<details>
<summary><b>REST API Endpoints</b></summary>
<br/>

| Method | Endpoint | Description | DSA Used |
|:---:|:---|:---|:---:|
| `GET` | `/api/product/<sku>` | Fetch product by SKU | HashMap O(1) |
| `POST` | `/api/advance-day` | Simulate one day | Stack push + Queue pop |
| `POST` | `/api/undo` | Revert to previous day | Stack pop |
| `GET` | `/api/alerts?limit=N` | Top-N risk alerts | Priority Queue drain |
| `GET` | `/api/forecast?window=K` | Moving avg + trend | Sliding Window |
| `POST` | `/api/save-settings` | Update + persist settings | File I/O |
| `GET` | `/api/get-settings` | Fetch current settings | — |

</details>

---

## 📂 &nbsp; Project Structure

```
StockDS/
│
├── 📄 src/main.cpp              ← 961 lines: all DSA logic, routes, background timer
│
├── 📁 templates/                ← Crow Mustache HTML (server-side rendered)
│   ├── index.html               ← Dashboard — KPI cards, charts, alert panel
│   ├── inventory.html           ← 990 SKUs table with filter buttons
│   ├── analysis.html            ← 6 DSA metric cards + Sliding Window chart
│   └── settings.html            ← Settings form with live save
│
├── 📁 data/                     ← CSV datasets loaded at startup
│   ├── inventory.csv            ← 990 products (SKU, name, category, qty, velocity)
│   ├── alerts.csv               ← 1,458 risk alerts (type, title, risk_score)
│   ├── shipments.csv            ← 587 inbound shipments (supplier, date, priority)
│   ├── transactions.csv         ← 22,558 transaction records (1.2 MB)
│   ├── daily_sales.csv          ← 31 days of sales aggregates
│   ├── dashboard_stats.csv      ← Seed metrics
│   └── settings.cfg             ← Persisted runtime config
│
├── 📁 docs/
│   ├── SRS_Analysis_Report.md   ← Requirements vs Implementation gap analysis
│   └── screenshots/             ← All README images
│
├── CMakeLists.txt               ← C++17 · Crow linkage
├── srs.txt                      ← Original Software Requirements Specification
└── run.txt                      ← Quick build & run reference
```

---

## 🔮 &nbsp; Roadmap

| Feature | Algorithm | Purpose |
|:---:|:---|:---|
| 🔍 **Smart Search** | Trie (Prefix Tree) — O(L) | Instant product-name autocomplete as user types |
| 💰 **Budget Optimizer** | 0/1 Knapsack (DP) | Optimal restock cart maximizing risk reduction within budget |
| 🎨 **Cart Animation** | DP trace-back | Visualize items being selected into virtual cart |

---

<div align="center">

<br/>

**Built for DSA Viva · 3rd Semester · C++ Systems Programming**

<br/>

*StockFlow proves that Data Structures are not academic abstractions —*  
*they are engineering decisions with measurable, real-world impact.*

<br/>

[![GitHub](https://img.shields.io/badge/View_on_GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/Asma-Shoukat/StockDS)
&nbsp;
[![C++](https://img.shields.io/badge/Made_with-C%2B%2B17-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
&nbsp;
[![Crow](https://img.shields.io/badge/Powered_by-Crow_HTTP-222?style=for-the-badge)](https://crowcpp.org/)

<br/>

</div>
