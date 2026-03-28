#pragma once
#include "order.h"
#include <map>
#include <queue>

class OrderBook {
private:
    // BUY: highest price first
    std::map<double, std::queue<Order>, std::greater<>> buyOrders;

    // SELL: lowest price first
    std::map<double, std::queue<Order>> sellOrders;

public:
    void addOrder(Order order);
    void match();
};