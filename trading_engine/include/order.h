#pragma once
#include <cstdint>
#include <iostream>

enum class Side {
    BUY,
    SELL
};

enum class OrderStatus {
    PENDING,
    PARTIALLY_FILLED,
    FILLED,
    CANCELLED
};

struct Order {
    int id;
    Side side;
    double price;
    int quantity;
    int filled_quantity;
    uint64_t timestamp;
    OrderStatus status;

    Order(int id, Side side, double price, int quantity, uint64_t timestamp)
        : id(id), side(side), price(price), quantity(quantity), 
          filled_quantity(0), timestamp(timestamp), status(OrderStatus::PENDING) {}

    int remaining_quantity() const {
        return quantity - filled_quantity;
    }

    bool is_filled() const {
        return filled_quantity == quantity;
    }

    void fill(int qty) {
        filled_quantity += qty;
        if (filled_quantity == quantity) {
            status = OrderStatus::FILLED;
        } else if (filled_quantity > 0) {
            status = OrderStatus::PARTIALLY_FILLED;
        }
    }

    void print() const {
        std::cout << "Order[id=" << id << ", side=" << (side == Side::BUY ? "BUY" : "SELL")
                  << ", price=" << price << ", qty=" << quantity 
                  << ", filled=" << filled_quantity << "]" << std::endl;
    }
};
