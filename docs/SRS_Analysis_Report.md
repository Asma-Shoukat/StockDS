# 📊 StockFlow - SRS vs Implementation Analysis Report

**Project:** SmartInventory (StockFlow)  
**Date:** December 25, 2025  
**Status:** In Development  

---

## 📁 Current Dataset Analysis

### Current Data Files

| File | Records | Fields |
|------|---------|--------|
| `inventory.csv` | 10 items | sku_id, product_name, category, quantity, velocity, status, icon |
| `alerts.csv` | 5 alerts | alert_id, type, title, message, item_id, time_ago |
| `dashboard_stats.csv` | 5 metrics | metric, value, change_percent, change_direction |

### ⚠️ Dataset Verdict: **TOO SMALL**

Your current dataset is **insufficient** for a Data Structures project. Here's why:

| Data Structure | Minimum Records Needed | Current | Why More Data? |
|---------------|----------------------|---------|----------------|
| **Hash Map** | 50-100+ items | 10 | Can't demonstrate O(1) vs O(n) performance difference |
| **Priority Queue** | 20-30 alerts | 5 | Need variety of risk scores to show heap ordering |
| **Trie (Autocomplete)** | 100+ product names | 10 | Autocomplete useless with 10 items |
| **Knapsack (DP)** | 30-50 items | 10 | Optimization problem needs choices |
| **Queue (Supply Chain)** | 15-20 shipments | 0 | Need incoming shipment data |
| **Sliding Window** | 30+ days of sales | 0 | Need historical sales data |

### 🎯 Recommended Dataset Size

For a **Viva-ready DS project**, you need:

| Data Type | Recommended Count |
|-----------|------------------|
| **Inventory Items** | 50-100 products |
| **Alerts/Risk Events** | 20-30 alerts |
| **Incoming Shipments** | 15-20 (for Queue simulation) |
| **Historical Sales** | 30 days minimum (for Sliding Window) |

---

## 📥 Kaggle Dataset Recommendations

### Option 1: Retail Inventory Dataset (RECOMMENDED)
- **Name:** "Retail Store Inventory" or "Supermarket Sales"
- **Why:** Contains product IDs, stock levels, categories, prices
- **Link:** Search "retail inventory dataset" on Kaggle

### Option 2: Supply Chain Dataset
- **Name:** "Supply Chain Shipment Pricing"
- **Why:** Has shipment data for Queue simulation
- **Link:** Search "supply chain dataset" on Kaggle

### Option 3: Create Your Own (EASIEST for Viva)
Generate 50-100 rows matching your current CSV format. This way:
- You understand every field
- Data matches your SRS exactly
- Easy to explain in Viva

---

## ✅ What's Been ACHIEVED

| SRS Requirement | Status | Implementation Details |
|----------------|--------|------------------------|
| **C++ Backend (Crow Framework)** | ✅ Done | `src/main.cpp` - HTTP server on port 8080 |
| **HTML5/CSS3/JS Frontend** | ✅ Done | 4 HTML templates with modern styling |
| **CSV Data Loading** | ✅ Done | Functions: `loadInventoryFromCSV()`, `loadAlertsFromCSV()`, `loadDashboardStatsFromCSV()` |
| **FR-06: Product Management View** | ✅ Done | `inventory.html` shows all items in table |
| **FR-03: Risk Dashboard Display** | ✅ Partial | Alerts panel shows 5 alerts (no Priority Queue sorting) |
| **NFR-02: Color-coded Alerts** | ✅ Done | Critical=Red, Warning=Yellow, Info=Blue |
| **Dashboard Page (UI)** | ✅ Done | 4 metric cards, Flow Trend chart, alerts |
| **Inventory Page (UI)** | ✅ Done | Table with Mustache templating |
| **Analysis Page (UI)** | ✅ Done | Supply chain flow diagram |
| **Settings Page (UI)** | ✅ Done | Threshold configuration form |

---

## ⚠️ PARTIALLY Implemented

| SRS Requirement | Current State | Gap |
|----------------|---------------|-----|
| **Hash Map (unordered_map)** | Using `vector<StockItem>` | Need `unordered_map<string, StockItem>` for O(1) lookup |
| **Priority Queue for Alerts** | Alerts displayed in CSV order | No `std::priority_queue` ranking by Risk Score |
| **Supply Chain Visualization** | Static diagram in Analysis page | No actual Queue of shipments moving over time |

---

## ❌ NOT YET Implemented (Major Gaps)

### Module 1: Command Center (Dashboard)

| Feature | Data Structure | Status |
|---------|---------------|--------|
| Day Counter ("Day 45") | Variable | ❌ Missing |
| "Advance Day" Button | Queue trigger | ❌ Missing |
| "Undo" Button | Stack pop | ❌ Missing |
| Supply Line Animation | Queue visualization | ❌ Missing |

### Module 2: Warehouse (Inventory)

| Feature | Data Structure | Status |
|---------|---------------|--------|
| Smart Search Bar | Trie | ❌ Missing |
| Autocomplete Dropdown | Trie traversal | ❌ Missing |
| Product Detail Modal | Hash Map lookup | ❌ Missing |

### Module 3: Manager's Office (Optimizer) - **ENTIRE PAGE MISSING**

| Feature | Data Structure | Status |
|---------|---------------|--------|
| Optimizer Page | - | ❌ Page doesn't exist |
| Budget Input Field | - | ❌ Missing |
| Available Items List | - | ❌ Missing |
| Knapsack Algorithm | Dynamic Programming | ❌ Missing |
| Optimized Cart Display | DP result | ❌ Missing |
| Budget Progress Bar | - | ❌ Missing |
| Visual Cart Animation | - | ❌ Missing |

---

## 🔧 Data Structures Implementation Status

| Data Structure | SRS Purpose | Code Status | File Location |
|---------------|-------------|-------------|---------------|
| `std::unordered_map` | Inventory O(1) lookup | ❌ Not used | - |
| `std::queue` | Supply chain FIFO simulation | ❌ Not implemented | - |
| `std::priority_queue` | Risk-ranked alerts (Max-Heap) | ❌ Not implemented | - |
| `std::stack` | Undo/History snapshots | ❌ Not implemented | - |
| **Trie** (custom class) | Autocomplete search O(L) | ❌ Not implemented | - |
| **Knapsack** (DP function) | Budget optimization | ❌ Not implemented | - |
| **Sliding Window** | Demand forecasting | ❌ Not implemented | - |

---

## 📈 Completion Summary

| Category | Percentage | Notes |
|----------|-----------|-------|
| **UI/Frontend Design** | 70% | 4 pages done, Optimizer page missing |
| **Backend Routing (Crow)** | 80% | All routes work, need API endpoints |
| **CSV Data Loading** | 90% | Working, but dataset too small |
| **Data Structures (Core)** | **15%** | Critical gap - this is DS project! |
| **Functional Requirements** | **35%** | 9 FRs in SRS, ~3 done |
| **Overall Project** | **~40%** | Need DS implementations |

---

## 🎯 Implementation Priority Order

### Phase 1: Data Foundation (Do First)
1. ✅ Expand `inventory.csv` to 50+ items
2. ✅ Create `shipments.csv` for Queue simulation
3. ✅ Create `sales_history.csv` for Sliding Window

### Phase 2: Core Data Structures
4. 🔄 **Hash Map** - Replace vector with `unordered_map<string, StockItem>`
5. 🔄 **Priority Queue** - Implement risk scoring with `std::priority_queue`
6. 🔄 **Queue** - Supply chain simulation with `std::queue`
7. 🔄 **Stack** - Undo system with `std::stack`

### Phase 3: Advanced Features
8. 🔄 **Trie** - Build custom Trie class for autocomplete
9. 🔄 **Knapsack DP** - Budget optimizer algorithm
10. 🔄 **Sliding Window** - Demand forecasting

### Phase 4: UI Integration
11. 🔄 Add Day Counter + Advance/Undo buttons to Dashboard
12. 🔄 Add Search bar with autocomplete to Inventory
13. 🔄 Create entire Optimizer page

---

## 📂 Current Project Structure

```
StockDS/
├── src/
│   └── main.cpp              # C++ backend with Crow
├── templates/
│   ├── index.html            # Dashboard page
│   ├── inventory.html        # Inventory table
│   ├── analysis.html         # Bottleneck analysis
│   └── settings.html         # Configuration
├── data/
│   ├── inventory.csv         # 10 items (NEED 50+)
│   ├── alerts.csv            # 5 alerts
│   ├── dashboard_stats.csv   # 5 metrics
│   ├── storage.csv           # Storage data
│   └── srs.txt               # Requirements document
├── build/                    # CMake build output
├── vcpkg/                    # Package manager
└── CMakeLists.txt            # Build configuration
```

---

## 🚀 Next Steps

1. **Decide on dataset approach:**
   - Option A: Expand current CSVs manually (50+ items)
   - Option B: Download Kaggle dataset and adapt
   - Option C: Generate synthetic data with a script

2. **Start implementing data structures in order of difficulty:**
   - Easy: Hash Map, Queue, Stack
   - Medium: Priority Queue
   - Hard: Trie, Knapsack DP

3. **Add missing UI elements** as each DS is implemented

---

*Report generated for Viva preparation - StockFlow Data Structures Project*
