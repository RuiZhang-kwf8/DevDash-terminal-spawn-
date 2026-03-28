#include "trading_engine.h"
#include <iostream>
#include <chrono>

TradingEngine::TradingEngine() {
    engine_benchmark_.start();
}

std::vector<Trade> TradingEngine::submit_order(int order_id, Side side, double price, int quantity) {
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    
    auto order = std::make_shared<Order>(order_id, side, price, quantity, timestamp);
    auto trades = orderbook_.add_order(order);
    
    // Call trade callback for each matched trade
    for (const auto& trade : trades) {
        all_trades_.push_back(trade);
        if (trade_callback_) {
            trade_callback_(trade);
        }
    }
    
    return trades;
}

bool TradingEngine::cancel_order(int order_id) {
    return orderbook_.cancel_order(order_id);
}

std::shared_ptr<Order> TradingEngine::get_order(int order_id) const {
    return orderbook_.get_order(order_id);
}

void TradingEngine::set_trade_callback(std::function<void(const Trade&)> callback) {
    trade_callback_ = callback;
}

void TradingEngine::print_stats() const {
    std::cout << "\n=== TRADING ENGINE STATISTICS ===" << std::endl;
    std::cout << "Total Trades Executed: " << all_trades_.size() << std::endl;
    
    if (!all_trades_.empty()) {
        double total_volume = 0;
        double total_notional = 0;
        
        for (const auto& trade : all_trades_) {
            total_volume += trade.quantity;
            total_notional += trade.price * trade.quantity;
        }
        
        std::cout << "Total Volume Traded: " << total_volume << std::endl;
        std::cout << "Total Notional Value: " << total_notional << std::endl;
        std::cout << "Average Trade Price: " << (total_notional / total_volume) << std::endl;
    }
    
    std::cout << "===================================\n" << std::endl;
}

void TradingEngine::print_orderbook() const {
    orderbook_.print();
}
