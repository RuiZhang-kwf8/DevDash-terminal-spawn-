#include "order_book.h"
#include <iostream>

void OrderBook::addOrder(Order order) {
    if (order.side == Side::BUY) {
        buyOrders[order.price].push(order);
    } else {
        sellOrders[order.price].push(order);
    }
}

void OrderBook::match() {
    while (!buyOrders.empty() && !sellOrders.empty()) {
        auto buyIt = buyOrders.begin();   // highest buy
        auto sellIt = sellOrders.begin(); // lowest sell

        if (buyIt->first < sellIt->first) {
            break;
        }

        Order& buyOrder = buyIt->second.front();
        Order& sellOrder = sellIt->second.front();

        int tradeQty = std::min(buyOrder.quantity, sellOrder.quantity);

        std::cout << "TRADE: "
                  << tradeQty << " @ " << sellIt->first << "\n";

        buyOrder.quantity -= tradeQty;
        sellOrder.quantity -= tradeQty;

        if (buyOrder.quantity == 0) {
            buyIt->second.pop();
            if (buyIt->second.empty()) {
                buyOrders.erase(buyIt);
            }
        }

        if (sellOrder.quantity == 0) {
            sellIt->second.pop();
            if (sellIt->second.empty()) {
                sellOrders.erase(sellIt);
            }
        }
    }
}