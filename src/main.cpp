#include "crow.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include <ctime>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

// Data Structures
struct StockItem {
    string sku_id;
    string name;
    string category;
    int quantity;
    int velocity;
    string status;
    string icon;
    double unit_price;
    int risk_score;
};

struct Alert {
    string alert_id;
    string type;
    string title;
    string message;
    string item_id;
    string time_ago;
    int risk_score;

    // Sort by risk_score (descending)
    bool operator<(const Alert& other) const {
        return risk_score < other.risk_score;
    }
};

struct Shipment {
    string shipment_id;
    string product_id;
    string product_name;
    int quantity;
    string supplier;
    string expected_date;
    string status;
    string priority;
};

struct Transaction {
    string transaction_id;
    string product_id;
    int quantity_change;
    string type;
    string timestamp;
    int running_stock;
};

struct DailySales {
    string date;
    int sales_count;
    int total_quantity;
};

struct DashboardMetrics {
    int total_skus;
    int bottlenecks;
    int flow_rate;
    string system_status;
    int critical_items;
    int low_items;
};

struct SystemState {
    int day_number;
    vector<StockItem> inventory_snapshot;
    vector<Alert> alerts_snapshot;
    string timestamp;
};

// Global Containers
unordered_map<string, StockItem> inventoryMap;
priority_queue<Alert> alertQueue;
queue<Shipment> shipmentQueue;
stack<SystemState> historyStack;

vector<StockItem> inventoryList;
vector<Alert> alertsList;
vector<Shipment> shipmentsList;
vector<Transaction> transactionsList;
vector<DailySales> dailySalesList;

DashboardMetrics metrics;
int currentDay = 1;

// Configuration Settings
string companyName = "StockFlow Inc.";
int lowStockThreshold = 20;
int criticalStockThreshold = 5;
int forecastWindow = 7;

// Background Timer Control
atomic<bool> timerRunning(true);
int autoAdvanceInterval = 5;

// CSV Loading Functions

void loadInventoryFromCSV() {
    inventoryMap.clear();
    inventoryList.clear();
    
    ifstream file("data/inventory.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/inventory.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line); // Skip header
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        StockItem item;
        string temp;
        
        getline(ss, item.sku_id, ',');
        getline(ss, item.name, ',');
        getline(ss, item.category, ',');
        
        getline(ss, temp, ',');
        item.quantity = temp.empty() ? 0 : stoi(temp);
        
        getline(ss, temp, ',');
        item.velocity = temp.empty() ? 0 : stoi(temp);
        
        getline(ss, item.status, ',');
        getline(ss, item.icon, ',');
        
        // Randomize initial stock for simulation variety
        int randomStock = rand() % 100;
        if (randomStock < 70) {
            item.quantity = 50 + (rand() % 100);
            item.status = "Stocked";
        }
        else if (randomStock < 90) {
            item.quantity = 10 + (rand() % 10);
            item.status = "Low";
        }
        else {
            item.quantity = 1 + (rand() % 4);
            item.status = "Critical";
        }
        
        if (item.status == "Critical") {
            item.risk_score = 80 + (item.velocity / 10);
        } else if (item.status == "Low") {
            item.risk_score = 50 + (item.velocity / 10);
        } else {
            item.risk_score = 20;
        }
        
        inventoryMap[item.sku_id] = item;
        inventoryList.push_back(item);
    }
    
    file.close();
    cout << "Loaded " << inventoryList.size() << " items into Hash Map" << endl;
}

void loadAlertsFromCSV() {
    alertsList.clear();
    priority_queue<Alert> emptyQueue;
    swap(alertQueue, emptyQueue);
    
    ifstream file("data/alerts.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/alerts.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        Alert alert;
        string temp;
        
        getline(ss, alert.alert_id, ',');
        getline(ss, alert.type, ',');
        getline(ss, alert.title, ',');
        getline(ss, alert.message, ',');
        getline(ss, alert.item_id, ',');
        getline(ss, alert.time_ago, ',');
        
        getline(ss, temp, ',');
        alert.risk_score = temp.empty() ? 0 : stoi(temp);
        
        alertQueue.push(alert);
        alertsList.push_back(alert);
    }
    
    file.close();
    cout << "Loaded " << alertsList.size() << " alerts into Priority Queue" << endl;
}

void loadShipmentsFromCSV() {
    shipmentsList.clear();
    queue<Shipment> emptyQueue;
    swap(shipmentQueue, emptyQueue);
    
    ifstream file("data/shipments.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/shipments.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        Shipment ship;
        string temp;
        
        getline(ss, ship.shipment_id, ',');
        getline(ss, ship.product_id, ',');
        getline(ss, ship.product_name, ',');
        
        getline(ss, temp, ',');
        ship.quantity = temp.empty() ? 0 : stoi(temp);
        
        getline(ss, ship.supplier, ',');
        getline(ss, ship.expected_date, ',');
        getline(ss, ship.status, ',');
        getline(ss, ship.priority, ',');
        
        shipmentQueue.push(ship);
        shipmentsList.push_back(ship);
    }
    
    file.close();
    cout << "Loaded " << shipmentsList.size() << " shipments into Queue" << endl;
}

void loadTransactionsFromCSV() {
    transactionsList.clear();
    
    ifstream file("data/transactions.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/transactions.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        Transaction trans;
        string temp;
        
        getline(ss, trans.transaction_id, ',');
        getline(ss, trans.product_id, ',');
        
        getline(ss, temp, ',');
        trans.quantity_change = temp.empty() ? 0 : stoi(temp);
        
        getline(ss, trans.type, ',');
        getline(ss, trans.timestamp, ',');
        
        getline(ss, temp, ',');
        trans.running_stock = temp.empty() ? 0 : stoi(temp);
        
        transactionsList.push_back(trans);
    }
    
    file.close();
    cout << "Loaded " << transactionsList.size() << " transactions" << endl;
}

void loadDailySalesFromCSV() {
    dailySalesList.clear();
    
    ifstream file("data/daily_sales.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/daily_sales.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        DailySales day;
        string temp;
        
        getline(ss, day.date, ',');
        
        getline(ss, temp, ',');
        day.sales_count = temp.empty() ? 0 : stoi(temp);
        
        getline(ss, temp, ',');
        day.total_quantity = temp.empty() ? 0 : stoi(temp);
        
        dailySalesList.push_back(day);
    }
    
    file.close();
    cout << "Loaded " << dailySalesList.size() << " days of sales data" << endl;
}

void loadDashboardStatsFromCSV() {
    ifstream file("data/dashboard_stats.csv");
    if (!file.is_open()) {
        cout << "ERROR: Could not open data/dashboard_stats.csv" << endl;
        return;
    }
    
    string line;
    getline(file, line);
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string metric, value;
        
        getline(ss, metric, ',');
        getline(ss, value, ',');
        
        if (metric == "total_skus") metrics.total_skus = stoi(value);
        else if (metric == "bottlenecks") metrics.bottlenecks = stoi(value);
        else if (metric == "flow_rate") metrics.flow_rate = stoi(value);
        else if (metric == "system_status") metrics.system_status = value;
        else if (metric == "critical_items") metrics.critical_items = stoi(value);
        else if (metric == "low_items") metrics.low_items = stoi(value);
    }
    
    file.close();
    cout << "Loaded dashboard stats" << endl;
}

void loadAllDataFromCSV() {
    cout << "\n========== Loading Data ==========" << endl;
    loadInventoryFromCSV();
    loadAlertsFromCSV();
    loadShipmentsFromCSV();
    loadTransactionsFromCSV();
    loadDailySalesFromCSV();
    loadDashboardStatsFromCSV();
    cout << "========== Data Loaded ==========" << endl;
}

// --- Settings Management ---

void saveSettingsToFile() {
    ofstream file("data/settings.cfg");
    if (file.is_open()) {
        file << companyName << endl;
        file << lowStockThreshold << endl;
        file << criticalStockThreshold << endl;
        file << forecastWindow << endl;
        file.close();
        cout << "Settings saved to data/settings.cfg" << endl;
    } else {
        cout << "ERROR: Could not save settings to file" << endl;
    }
}

void createDefaultSettingsFile() {
    ifstream checkFile("data/settings.cfg");
    if (!checkFile.is_open()) {
        companyName = "StockFlow Inc.";
        lowStockThreshold = 20;
        criticalStockThreshold = 5;
        forecastWindow = 7;
        saveSettingsToFile();
        cout << "Created default settings file" << endl;
    }
    checkFile.close();
}

void loadSettingsFromFile() {
    createDefaultSettingsFile();
    
    ifstream file("data/settings.cfg");
    if (file.is_open()) {
        string temp;
        if (getline(file, temp) && !temp.empty()) companyName = temp;
        if (getline(file, temp) && !temp.empty()) {
            try { lowStockThreshold = stoi(temp); } catch(...) { lowStockThreshold = 20; }
        }
        if (getline(file, temp) && !temp.empty()) {
            try { criticalStockThreshold = stoi(temp); } catch(...) { criticalStockThreshold = 5; }
        }
        if (getline(file, temp) && !temp.empty()) {
            try { forecastWindow = stoi(temp); } catch(...) { forecastWindow = 7; }
        }
        file.close();
        cout << "Settings loaded from file" << endl;
    }
}

// --- Logic & Algorithms ---

StockItem* getProductById(string sku_id) {
    if (inventoryMap.find(sku_id) != inventoryMap.end()) {
        return &inventoryMap[sku_id];
    }
    return nullptr;
}

vector<Alert> getTopAlerts(int n) {
    vector<Alert> topAlerts;
    priority_queue<Alert> tempQueue = alertQueue;
    
    int count = 0;
    while (!tempQueue.empty() && count < n) {
        topAlerts.push_back(tempQueue.top());
        tempQueue.pop();
        count++;
    }
    return topAlerts;
}

Shipment processNextShipment() {
    if (shipmentQueue.empty()) {
        return Shipment();
    }
    
    Shipment next = shipmentQueue.front();
    shipmentQueue.pop();
    
    if (inventoryMap.find(next.product_id) != inventoryMap.end()) {
        inventoryMap[next.product_id].quantity += next.quantity;
        cout << "Shipment arrived: " << next.quantity << " units of " << next.product_name << endl;
    }
    
    return next;
}

void saveStateToHistory() {
    SystemState state;
    state.day_number = currentDay;
    state.inventory_snapshot = inventoryList;
    state.alerts_snapshot = alertsList;
    
    time_t now = time(0);
    state.timestamp = ctime(&now);
    
    historyStack.push(state);
    cout << "State saved to history (Day " << currentDay << ")" << endl;
}

bool undoLastAction() {
    if (historyStack.empty()) {
        cout << "No history to undo!" << endl;
        return false;
    }
    
    SystemState previousState = historyStack.top();
    historyStack.pop();
    
    currentDay = previousState.day_number;
    inventoryList = previousState.inventory_snapshot;
    alertsList = previousState.alerts_snapshot;
    
    // Rebuild hash map from restored state
    inventoryMap.clear();
    for (int i = 0; i < inventoryList.size(); i++) {
        string skuId = inventoryList[i].sku_id;
        inventoryMap[skuId] = inventoryList[i];
    }
    
    cout << "Restored to Day " << currentDay << endl;
    return true;
}

double calculateMovingAverage(int windowSize) {
    if (dailySalesList.empty() || windowSize <= 0) return 0.0;
    
    int totalDays = dailySalesList.size();
    int actualWindow = min(windowSize, totalDays);
    int startIndex = totalDays - actualWindow;
    
    double sum = 0;
    for (int i = startIndex; i < totalDays; i++) {
        sum += dailySalesList[i].total_quantity;
    }
    
    return sum / actualWindow;
}

string getDemandTrend(int windowSize) {
    if (dailySalesList.size() < 2) return "stable";
    
    int totalDays = dailySalesList.size();
    int actualWindow = min(windowSize, totalDays);
    int startIndex = totalDays - actualWindow;
    int midPoint = startIndex + (actualWindow / 2);
    
    double firstHalfSum = 0; int firstCount = 0;
    double secondHalfSum = 0; int secondCount = 0;
    
    for (int i = startIndex; i < totalDays; i++) {
        if (i < midPoint) {
            firstHalfSum += dailySalesList[i].total_quantity;
            firstCount++;
        } else {
            secondHalfSum += dailySalesList[i].total_quantity;
            secondCount++;
        }
    }
    
    double firstAvg = (firstCount > 0) ? firstHalfSum / firstCount : 0;
    double secondAvg = (secondCount > 0) ? secondHalfSum / secondCount : 0;
    
    if (secondAvg > firstAvg * 1.05) return "increasing";
    else if (secondAvg < firstAvg * 0.95) return "decreasing";
    else return "stable";
}

void refillShipmentQueue() {
    int shipmentsAdded = 0;
    
    for (int i = 0; i < inventoryList.size() && shipmentsAdded < 5; i++) {
        if (inventoryList[i].status == "Critical" || inventoryList[i].status == "Low") {
            // 20% chance to generate shipment
            if (rand() % 100 < 20) {
                Shipment newShip;
                newShip.shipment_id = "AUTO-" + to_string(currentDay) + "-" + to_string(rand() % 1000);
                newShip.product_id = inventoryList[i].sku_id;
                newShip.product_name = inventoryList[i].name;
                newShip.quantity = 40 + (rand() % 60);
                newShip.supplier = "Auto Supplier";
                newShip.expected_date = "Day " + to_string(currentDay + 1);
                newShip.status = "in_transit";
                newShip.priority = "normal";
                
                shipmentQueue.push(newShip);
                shipmentsAdded++;
            }
        }
    }
    
    if (shipmentsAdded > 0) {
        cout << "[QUEUE] Added " << shipmentsAdded << " new shipments" << endl;
    }
}

void advanceDay() {
    saveStateToHistory();
    currentDay++;
    refillShipmentQueue();
    
    // Process arriving shipments
    int shipmentsProcessed = 0;
    while (!shipmentQueue.empty() && shipmentsProcessed < 5) {
        processNextShipment();
        shipmentsProcessed++;
    }
    
    // Simulate sales
    int itemsSold = 0;
    int itemsToSell = 30 + (rand() % 20);
    
    for (int count = 0; count < itemsToSell; count++) {
        int i = rand() % inventoryList.size();
        
        if (inventoryList[i].quantity > 0) {
            int dailySales = 1 + (rand() % 2);
            int newQuantity = max(0, inventoryList[i].quantity - dailySales);
            
            inventoryList[i].quantity = newQuantity;
            itemsSold++;
            
            if (inventoryList[i].quantity < criticalStockThreshold) inventoryList[i].status = "Critical";
            else if (inventoryList[i].quantity < lowStockThreshold) inventoryList[i].status = "Low";
            else inventoryList[i].status = "Stocked";
            
            inventoryMap[inventoryList[i].sku_id] = inventoryList[i];
        }
    }
    
    // Update dashboard metrics
    int criticalCount = 0;
    int lowCount = 0;
    int stockedCount = 0;
    for (auto& item : inventoryList) {
        if (item.status == "Critical") criticalCount++;
        else if (item.status == "Low") lowCount++;
        else stockedCount++;
    }
    metrics.critical_items = criticalCount;
    metrics.low_items = lowCount;
    metrics.bottlenecks = criticalCount + lowCount;
    
    cout << "[DAY " << currentDay << "] Sold: " << itemsSold 
         << " | Stocked: " << stockedCount << " | Low: " << lowCount 
         << " | Critical: " << criticalCount << endl;
}

void backgroundTimer() {
    cout << "Background timer started (" << autoAdvanceInterval << " seconds per day)" << endl;
    
    while (timerRunning) {
        for (int i = 0; i < autoAdvanceInterval && timerRunning; i++) {
            this_thread::sleep_for(chrono::seconds(1));
        }
        
        if (timerRunning) {
            cout << "\n[AUTO] Timer triggered - ";
            advanceDay();
        }
    }
    cout << "Background timer stopped" << endl;
}

// --- Main & Routes ---

int main() {
    srand(time(0));
    
    loadAllDataFromCSV();
    loadSettingsFromFile();
    
    crow::SimpleApp app;
    crow::mustache::set_base("./templates");
    
    // --- Page Routes ---
    
    CROW_ROUTE(app, "/")([](){
        crow::mustache::context ctx;
        ctx["total_skus"] = metrics.total_skus;
        ctx["low_threshold"] = lowStockThreshold;
        ctx["critical_threshold"] = criticalStockThreshold;
        ctx["bottlenecks_found"] = metrics.bottlenecks;
        ctx["flow_rate"] = metrics.flow_rate;
        ctx["current_day"] = currentDay;
        
        int stocked = 0, low = 0, critical = 0;
        for (auto& item : inventoryList) {
            if (item.status == "Stocked") stocked++;
            else if (item.status == "Low") low++;
            else if (item.status == "Critical") critical++;
        }
        ctx["stocked_count"] = stocked;
        ctx["low_count"] = low;
        ctx["critical_count"] = critical;
        
        vector<Alert> topAlerts = getTopAlerts(5);
        vector<crow::mustache::context> alertsCtx;
        for (auto& a : topAlerts) {
            crow::mustache::context ac;
            ac["type"] = a.type;
            ac["title"] = a.title;
            ac["message"] = a.message;
            ac["time_ago"] = a.time_ago;
            ac["risk_score"] = a.risk_score;
            ac["is_critical"] = (a.type == "critical");
            ac["is_warning"] = (a.type == "warning");
            ac["is_info"] = (a.type == "info");
            alertsCtx.push_back(move(ac));
        }
        ctx["alerts"] = move(alertsCtx);
        
        ctx["moving_average"] = (int)calculateMovingAverage(7);
        ctx["demand_trend"] = getDemandTrend(7);
        ctx["pending_shipments"] = (int)shipmentQueue.size();
        
        vector<crow::mustache::context> shipmentsCtx;
        queue<Shipment> tempQueue = shipmentQueue;
        int count = 0;
        while (!tempQueue.empty() && count < 5) {
            Shipment s = tempQueue.front();
            tempQueue.pop();
            crow::mustache::context sc;
            sc["shipment_id"] = s.shipment_id;
            sc["product_name"] = s.product_name;
            sc["quantity"] = s.quantity;
            sc["expected_date"] = s.expected_date;
            sc["status"] = s.status;
            sc["is_urgent"] = (s.priority == "urgent");
            sc["is_processing"] = (s.status == "processing");
            sc["is_in_transit"] = (s.status == "in_transit");
            sc["is_arriving"] = (s.status == "arriving_today");
            shipmentsCtx.push_back(move(sc));
            count++;
        }
        ctx["shipments"] = move(shipmentsCtx);
        
        return crow::mustache::load("index.html").render(ctx);
    });
    
    CROW_ROUTE(app, "/inventory")([](){
        crow::mustache::context ctx;
        vector<crow::mustache::context> invCtx;
        
        for (auto& item : inventoryList) {
            crow::mustache::context ic;
            ic["sku_id"] = item.sku_id;
            ic["name"] = item.name;
            ic["category"] = item.category;
            ic["quantity"] = item.quantity;
            ic["velocity"] = item.velocity;
            ic["icon"] = item.icon.empty() ? "inventory_2" : item.icon;
            ic["status"] = item.status;
            
            bool isCritical = (item.quantity < criticalStockThreshold);
            bool isLow = (item.quantity >= criticalStockThreshold && item.quantity < lowStockThreshold);
            bool isStocked = (item.quantity >= lowStockThreshold);
            
            ic["is_critical"] = isCritical;
            ic["is_low"] = isLow;
            ic["is_stocked"] = isStocked;
            
            if (isCritical) ic["status_class"] = "critical";
            else if (isLow) ic["status_class"] = "low";
            else ic["status_class"] = "stocked";
            
            invCtx.push_back(move(ic));
        }
        ctx["inventory"] = move(invCtx);
        ctx["total_items"] = (int)inventoryList.size();
        
        return crow::mustache::load("inventory.html").render(ctx);
    });
    
    CROW_ROUTE(app, "/analysis")([](){
        crow::mustache::context ctx;
        ctx["isBottleneck"] = (metrics.bottlenecks > 0);
        ctx["supplier_status"] = "Active";
        ctx["processing_status"] = (metrics.bottlenecks > 10) ? "Congested" : "Normal";
        ctx["distribution_status"] = "Active";
        
        ctx["forecast_window"] = forecastWindow;
        ctx["moving_average"] = (int)calculateMovingAverage(forecastWindow);
        ctx["demand_trend"] = getDemandTrend(forecastWindow);
        
        vector<crow::mustache::context> dailySalesCtx;
        for (auto& day : dailySalesList) {
            crow::mustache::context dc;
            dc["date"] = day.date;
            dc["sales_count"] = day.sales_count;
            dc["total_quantity"] = day.total_quantity;
            dailySalesCtx.push_back(move(dc));
        }
        ctx["daily_sales"] = move(dailySalesCtx);
        ctx["total_days"] = (int)dailySalesList.size();
        
        ctx["pending_shipments"] = (int)shipmentQueue.size();
        
        vector<crow::mustache::context> shipmentsCtx;
        queue<Shipment> tempQueue = shipmentQueue;
        int count = 0;
        while (!tempQueue.empty() && count < 10) {
            Shipment s = tempQueue.front();
            tempQueue.pop();
            crow::mustache::context sc;
            sc["shipment_id"] = s.shipment_id;
            sc["product_name"] = s.product_name;
            sc["quantity"] = s.quantity;
            sc["expected_date"] = s.expected_date;
            sc["status"] = s.status;
            sc["is_urgent"] = (s.priority == "urgent");
            shipmentsCtx.push_back(move(sc));
            count++;
        }
        ctx["shipments"] = move(shipmentsCtx);
        
        return crow::mustache::load("analysis.html").render(ctx);
    });
    
    CROW_ROUTE(app, "/settings")([](){
        crow::mustache::context ctx;
        ctx["company_name"] = companyName; 
        ctx["low_threshold"] = lowStockThreshold;
        ctx["critical_threshold"] = criticalStockThreshold;
        ctx["forecast_window"] = forecastWindow;
        return crow::mustache::load("settings.html").render(ctx);
    });

    // --- API Routes ---
    
    CROW_ROUTE(app, "/api/product/<string>")
    ([](string sku_id){
        crow::json::wvalue response;
        StockItem* item = getProductById(sku_id);
        
        if (item != nullptr) {
            response["found"] = true;
            response["sku_id"] = item->sku_id;
            response["name"] = item->name;
            response["category"] = item->category;
            response["quantity"] = item->quantity;
            response["velocity"] = item->velocity;
            response["status"] = item->status;
        } else {
            response["found"] = false;
            response["error"] = "Product not found";
        }
        return crow::response(response);
    });
    
    CROW_ROUTE(app, "/api/advance-day").methods("POST"_method)
    ([](){
        advanceDay();
        crow::json::wvalue response;
        response["success"] = true;
        response["current_day"] = currentDay;
        response["message"] = "Advanced to Day " + to_string(currentDay);
        return crow::response(response);
    });
    
    CROW_ROUTE(app, "/api/undo").methods("POST"_method)
    ([](){
        bool success = undoLastAction();
        crow::json::wvalue response;
        response["success"] = success;
        response["current_day"] = currentDay;
        response["message"] = success ? "Restored to Day " + to_string(currentDay) : "No history to undo";
        return crow::response(response);
    });
    
    CROW_ROUTE(app, "/api/alerts")
    ([](const crow::request& req){
        int limit = 10;
        if (req.url_params.get("limit")) limit = stoi(req.url_params.get("limit"));
        
        vector<Alert> top = getTopAlerts(limit);
        crow::json::wvalue response;
        vector<crow::json::wvalue> alertsJson;
        
        for (auto& a : top) {
            crow::json::wvalue aj;
            aj["alert_id"] = a.alert_id;
            aj["type"] = a.type;
            aj["title"] = a.title;
            aj["message"] = a.message;
            aj["risk_score"] = a.risk_score;
            alertsJson.push_back(move(aj));
        }
        
        response["alerts"] = move(alertsJson);
        return crow::response(response);
    });
    
    CROW_ROUTE(app, "/api/forecast")
    ([](const crow::request& req){
        int window = forecastWindow;
        if (req.url_params.get("window")) window = stoi(req.url_params.get("window"));
        
        crow::json::wvalue response;
        response["window_size"] = window;
        response["moving_average"] = calculateMovingAverage(window);
        response["trend"] = getDemandTrend(window);
        
        vector<crow::json::wvalue> dailyData;
        int start = max(0, (int)dailySalesList.size() - window);
        for (int i = start; i < dailySalesList.size(); i++) {
            crow::json::wvalue day;
            day["date"] = dailySalesList[i].date;
            day["sales_count"] = dailySalesList[i].sales_count;
            day["total_quantity"] = dailySalesList[i].total_quantity;
            dailyData.push_back(move(day));
        }
        response["daily_data"] = move(dailyData);
        return crow::response(response);
    });
    
    CROW_ROUTE(app, "/api/save-settings").methods("POST"_method)([](const crow::request& req) {
        auto x = crow::json::load(req.body);
        if (!x) return crow::response(400, "Invalid JSON");

        try {
            if (x.has("companyName")) companyName = string(x["companyName"].s());
            if (x.has("lowThreshold")) lowStockThreshold = (int)x["lowThreshold"].i();
            if (x.has("criticalThreshold")) criticalStockThreshold = (int)x["criticalThreshold"].i();
            if (x.has("forecastWindow")) forecastWindow = (int)x["forecastWindow"].i();

            saveSettingsToFile();
            
            crow::json::wvalue response;
            response["success"] = true;
            response["message"] = "Settings saved successfully";
            return crow::response(200, response);
        } catch (const exception& e) {
            return crow::response(500, "Error saving settings");
        }
    });

    CROW_ROUTE(app, "/api/reset-settings").methods("POST"_method)([]() {
        companyName = "StockFlow Inc.";
        lowStockThreshold = 20;
        criticalStockThreshold = 5;
        forecastWindow = 7;
        saveSettingsToFile();
        
        crow::json::wvalue response;
        response["success"] = true;
        response["message"] = "Settings reset to defaults";
        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/api/get-settings")
    ([](){
        crow::json::wvalue response;
        response["companyName"] = companyName;
        response["lowThreshold"] = lowStockThreshold;
        response["criticalThreshold"] = criticalStockThreshold;
        response["forecastWindow"] = forecastWindow;
        return crow::response(200, response);
    });
// ==================== START SERVER ====================
    cout << "  STOCKFLOW SERVER - Data Structures Demo" << endl;
    cout << "==========================================================" << endl;
    cout << "  Data Structures Implemented:" << endl;
    cout << "    [+] Hash Map      - O(1) product lookup" << endl;
    cout << "    [+] Priority Queue - Risk-sorted alerts" << endl;
    cout << "    [+] Queue         - FIFO shipment processing" << endl;
    cout << "    [+] Stack         - Undo/History feature" << endl;
    cout << "    [+] Sliding Window - Demand forecasting" << endl;
    cout << "==========================================================" << endl;
    cout << "  Dashboard: http://localhost:8080/" << endl;
    cout << "  Inventory: http://localhost:8080/inventory" << endl;
    cout << "  Analysis:  http://localhost:8080/analysis" << endl;
    cout << "==========================================================" << endl;
    cout << "  [AUTO-TIMER] Days advance every " << autoAdvanceInterval << " seconds" << endl;
    cout << "==========================================================" << endl;
   
    // Start background timer thread

    thread timerThread(backgroundTimer);
    timerThread.detach(); 
   
    app.port(8080).multithreaded().run();
   
    // Stop timer when server stops
    timerRunning = false;
   
    return 0;
}