#include "order_book.h"

int main() {
    OrderBook ob;

    ob.addOrder(Order(1, Side::BUY, 100.0, 10, 1));
    ob.addOrder(Order(2, Side::SELL, 99.0, 5, 2));
    ob.addOrder(Order(3, Side::SELL, 100.0, 10, 3));

    ob.match();

    return 0;
}