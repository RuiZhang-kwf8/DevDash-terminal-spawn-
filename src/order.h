#pragma once
#include <cstdint>

enum class Side {
    BUY,
    SELL
};

struct Order {
    int id;
    Side side;
    double price;
    int quantity;
    uint64_t timestamp;

    Order(int id, Side side, double price, int quantity, uint64_t timestamp)
        : id(id), side(side), price(price), quantity(quantity), timestamp(timestamp) {}
};