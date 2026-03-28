#include "orderbook.h"
#include <algorithm>

const std::vector<std::shared_ptr<Order>> OrderBook::empty_vector_;

std::vector<Trade> OrderBook::add_order(std::shared_ptr<Order> order) {
    std::vector<Trade> trades;
    
    // Try to match the incoming order
    trades = match_order(order);
    
    // If order has remaining quantity, add to book
    if (order->remaining_quantity() > 0) {
        double price = order->price;
        
        if (order->side == Side::BUY) {
            buy_orders_[price].push_back(order);
        } else {
            sell_orders_[price].push_back(order);
        }
        
        all_orders_[order->id] = order;
        order_to_price_[order->id] = price;
        order_is_buy_[order->id] = (order->side == Side::BUY);
    } else {
        // Order completely filled
        all_orders_[order->id] = order;
        order_to_price_[order->id] = order->price;
        order_is_buy_[order->id] = (order->side == Side::BUY);
    }
    
    return trades;
}

std::vector<Trade> OrderBook::match_order(std::shared_ptr<Order> order) {
    std::vector<Trade> trades;
    
    if (order->side == Side::BUY) {
        // Match against sell orders
        auto it = sell_orders_.begin();
        
        while (it != sell_orders_.end() && order->remaining_quantity() > 0 && 
               order->price >= it->first) {
            
            auto& sell_orders_at_price = it->second;
            
            while (!sell_orders_at_price.empty() && order->remaining_quantity() > 0) {
                auto& sell_order = sell_orders_at_price.front();
                
                int match_qty = std::min(order->remaining_quantity(), 
                                        sell_order->remaining_quantity());
                
                // Create trade
                Trade trade(order->id, sell_order->id, sell_order->price, 
                           match_qty, order->timestamp);
                trades.push_back(trade);
                
                // Fill orders
                order->fill(match_qty);
                sell_order->fill(match_qty);
                
                // Remove filled order from book
                if (sell_order->is_filled()) {
                    sell_orders_at_price.erase(sell_orders_at_price.begin());
                }
            }
            
            // Remove price level if empty
            if (sell_orders_at_price.empty()) {
                it = sell_orders_.erase(it);
            } else {
                ++it;
            }
        }
    } else {
        // Match against buy orders
        auto it = buy_orders_.begin();
        
        while (it != buy_orders_.end() && order->remaining_quantity() > 0 && 
               order->price <= it->first) {
            
            auto& buy_orders_at_price = it->second;
            
            while (!buy_orders_at_price.empty() && order->remaining_quantity() > 0) {
                auto& buy_order = buy_orders_at_price.front();
                
                int match_qty = std::min(order->remaining_quantity(), 
                                        buy_order->remaining_quantity());
                
                // Create trade
                Trade trade(buy_order->id, order->id, buy_order->price, 
                           match_qty, order->timestamp);
                trades.push_back(trade);
                
                // Fill orders
                order->fill(match_qty);
                buy_order->fill(match_qty);
                
                // Remove filled order from book
                if (buy_order->is_filled()) {
                    buy_orders_at_price.erase(buy_orders_at_price.begin());
                }
            }
            
            // Remove price level if empty
            if (buy_orders_at_price.empty()) {
                it = buy_orders_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    return trades;
}

bool OrderBook::cancel_order(int order_id) {
    auto it = all_orders_.find(order_id);
    if (it == all_orders_.end()) {
        return false;
    }
    
    auto order = it->second;
    double price = order_to_price_[order_id];
    bool is_buy = order_is_buy_[order_id];
    
    if (is_buy) {
        auto& orders = buy_orders_[price];
        auto order_it = std::find(orders.begin(), orders.end(), order);
        if (order_it != orders.end()) {
            orders.erase(order_it);
        }
        
        if (orders.empty()) {
            buy_orders_.erase(price);
        }
    } else {
        auto& orders = sell_orders_[price];
        auto order_it = std::find(orders.begin(), orders.end(), order);
        if (order_it != orders.end()) {
            orders.erase(order_it);
        }
        
        if (orders.empty()) {
            sell_orders_.erase(price);
        }
    }
    
    order->status = OrderStatus::CANCELLED;
    all_orders_.erase(it);
    order_to_price_.erase(order_id);
    order_is_buy_.erase(order_id);
    
    return true;
}

std::shared_ptr<Order> OrderBook::get_order(int order_id) const {
    auto it = all_orders_.find(order_id);
    if (it != all_orders_.end()) {
        return it->second;
    }
    return nullptr;
}

double OrderBook::get_best_bid() const {
    if (buy_orders_.empty()) {
        return 0.0;
    }
    return buy_orders_.begin()->first;
}

double OrderBook::get_best_ask() const {
    if (sell_orders_.empty()) {
        return 0.0;
    }
    return sell_orders_.begin()->first;
}

const std::vector<std::shared_ptr<Order>>& OrderBook::get_buy_orders(double price) const {
    auto it = buy_orders_.find(price);
    if (it != buy_orders_.end()) {
        return it->second;
    }
    return empty_vector_;
}

const std::vector<std::shared_ptr<Order>>& OrderBook::get_sell_orders(double price) const {
    auto it = sell_orders_.find(price);
    if (it != sell_orders_.end()) {
        return it->second;
    }
    return empty_vector_;
}

int OrderBook::get_buy_volume_at_price(double price) const {
    auto it = buy_orders_.find(price);
    if (it == buy_orders_.end()) {
        return 0;
    }
    
    int volume = 0;
    for (const auto& order : it->second) {
        volume += order->remaining_quantity();
    }
    return volume;
}

int OrderBook::get_sell_volume_at_price(double price) const {
    auto it = sell_orders_.find(price);
    if (it == sell_orders_.end()) {
        return 0;
    }
    
    int volume = 0;
    for (const auto& order : it->second) {
        volume += order->remaining_quantity();
    }
    return volume;
}

void OrderBook::print() const {
    std::cout << "\n=== ORDER BOOK ===" << std::endl;
    
    std::cout << "\nASK SIDE (Sell Orders):" << std::endl;
    if (sell_orders_.empty()) {
        std::cout << "  No sell orders" << std::endl;
    } else {
        for (auto it = sell_orders_.rbegin(); it != sell_orders_.rend(); ++it) {
            double price = it->first;
            int volume = get_sell_volume_at_price(price);
            std::cout << "  Price: " << price << " | Volume: " << volume << std::endl;
        }
    }
    
    std::cout << "\nBID SIDE (Buy Orders):" << std::endl;
    if (buy_orders_.empty()) {
        std::cout << "  No buy orders" << std::endl;
    } else {
        for (const auto& [price, orders] : buy_orders_) {
            int volume = get_buy_volume_at_price(price);
            std::cout << "  Price: " << price << " | Volume: " << volume << std::endl;
        }
    }
    
    std::cout << "==================\n" << std::endl;
}
