#pragma once
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include "order.h"
#include "trade.h"

class OrderBook {
public:
    OrderBook() = default;
    ~OrderBook() = default;

    // Add an order and return matched trades
    std::vector<Trade> add_order(std::shared_ptr<Order> order);
    
    // Cancel an order
    bool cancel_order(int order_id);
    
    // Get order by ID
    std::shared_ptr<Order> get_order(int order_id) const;
    
    // Get best bid and ask prices
    double get_best_bid() const;
    double get_best_ask() const;
    
    // Get orders at a specific price level
    const std::vector<std::shared_ptr<Order>>& get_buy_orders(double price) const;
    const std::vector<std::shared_ptr<Order>>& get_sell_orders(double price) const;
    
    // Print order book state
    void print() const;
    
    // Get total buy/sell volume at price level
    int get_buy_volume_at_price(double price) const;
    int get_sell_volume_at_price(double price) const;

private:
    // Buy orders grouped by price (descending order)
    std::map<double, std::vector<std::shared_ptr<Order>>, std::greater<double>> buy_orders_;
    
    // Sell orders grouped by price (ascending order)
    std::map<double, std::vector<std::shared_ptr<Order>>> sell_orders_;
    
    // All orders by ID for quick lookup
    std::map<int, std::shared_ptr<Order>> all_orders_;
    
    // Order ID to price mapping for quick removal
    std::map<int, double> order_to_price_;
    std::map<int, bool> order_is_buy_;  // true if buy, false if sell
    
    // Empty vector for default returns
    static const std::vector<std::shared_ptr<Order>> empty_vector_;
    
    // Match orders using FIFO price-time priority
    std::vector<Trade> match_order(std::shared_ptr<Order> order);
    
    // Fill an order against existing orders
    void fill_against_orders(std::shared_ptr<Order> incoming_order, 
                            std::vector<Trade>& trades);
};
