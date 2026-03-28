#pragma once
#include <memory>
#include <vector>
#include <functional>
#include "orderbook.h"
#include "benchmark.h"

class TradingEngine {
public:
    TradingEngine();
    ~TradingEngine() = default;

    // Submit an order to the engine
    std::vector<Trade> submit_order(int order_id, Side side, double price, int quantity);
    
    // Cancel an existing order
    bool cancel_order(int order_id);
    
    // Get current order book state
    const OrderBook& get_orderbook() const { return orderbook_; }
    OrderBook& get_orderbook_mut() { return orderbook_; }
    
    // Get order by ID
    std::shared_ptr<Order> get_order(int order_id) const;
    
    // Set trade callback
    void set_trade_callback(std::function<void(const Trade&)> callback);
    
    // Print engine statistics
    void print_stats() const;
    
    // Print order book
    void print_orderbook() const;

private:
    OrderBook orderbook_;
    std::vector<Trade> all_trades_;
    std::function<void(const Trade&)> trade_callback_;
    Benchmark engine_benchmark_;
};
