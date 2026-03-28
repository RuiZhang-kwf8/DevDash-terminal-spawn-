#pragma once
#include <cstdint>
#include <iostream>

struct Trade {
    int buyer_order_id;
    int seller_order_id;
    double price;
    int quantity;
    uint64_t timestamp;

    Trade(int buyer_order_id, int seller_order_id, double price, int quantity, uint64_t timestamp)
        : buyer_order_id(buyer_order_id), seller_order_id(seller_order_id),
          price(price), quantity(quantity), timestamp(timestamp) {}

    void print() const {
        std::cout << "Trade: Buyer=" << buyer_order_id << " Seller=" << seller_order_id
                  << " Price=" << price << " Qty=" << quantity
                  << " Timestamp=" << timestamp << std::endl;
    }
};
