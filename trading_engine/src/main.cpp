#include <iostream>
#include "benchmark.h"
#include "trading_engine.h"

int main() {
    std::cout << "=== Trading Engine Demo ===" << std::endl;
    
    Benchmark total_bench;
    total_bench.start();
    
    // Create trading engine
    TradingEngine engine;
    
    // Set trade callback to print trades
    engine.set_trade_callback([](const Trade& trade) {
        trade.print();
    });
    
    // Scenario 1: Simple buy-sell match
    std::cout << "\n--- Scenario 1: Simple Buy-Sell Match ---" << std::endl;
    engine.submit_order(1, Side::BUY, 100.0, 50);
    std::cout << "Submitted BUY order: price=100.0, qty=50" << std::endl;
    
    std::cout << "\nSubmitting matching SELL order..." << std::endl;
    engine.submit_order(2, Side::SELL, 100.0, 50);
    std::cout << "Submitted SELL order: price=100.0, qty=50 (Should match)" << std::endl;
    
    engine.print_orderbook();
    
    // Scenario 2: Partial fill with multiple orders
    std::cout << "\n--- Scenario 2: Partial Fill ---" << std::endl;
    engine.submit_order(3, Side::BUY, 99.5, 100);
    std::cout << "Submitted BUY order: price=99.5, qty=100" << std::endl;
    
    engine.submit_order(4, Side::SELL, 99.5, 60);
    std::cout << "Submitted SELL order: price=99.5, qty=60 (Partial match)" << std::endl;
    
    engine.print_orderbook();
    
    // Scenario 3: Aggressive buy order crosses multiple sell orders
    std::cout << "\n--- Scenario 3: Aggressive Buy Crosses Multiple Sells ---" << std::endl;
    engine.submit_order(5, Side::SELL, 100.0, 50);
    engine.submit_order(6, Side::SELL, 100.5, 75);
    std::cout << "Submitted 2 SELL orders at 100.0 (qty=50) and 100.5 (qty=75)" << std::endl;
    
    std::cout << "\nSubmitting aggressive BUY order to cross..." << std::endl;
    engine.submit_order(7, Side::BUY, 101.0, 100);
    std::cout << "Submitted BUY order: price=101.0, qty=100 (Crosses both sells)" << std::endl;
    
    engine.print_orderbook();
    
    // Scenario 4: FIFO price-time priority
    std::cout << "\n--- Scenario 4: FIFO Price-Time Priority ---" << std::endl;
    engine.submit_order(8, Side::SELL, 98.0, 50);
    engine.submit_order(9, Side::SELL, 98.0, 75);
    std::cout << "Submitted 2 SELL orders at 98.0 (qty=50 then qty=75)" << std::endl;
    
    std::cout << "\nSubmitting BUY to match against queue (should match order 8 first)..." << std::endl;
    engine.submit_order(10, Side::BUY, 98.0, 60);
    std::cout << "Submitted BUY order: price=98.0, qty=60" << std::endl;
    
    engine.print_orderbook();
    
    // Scenario 5: Cancel order
    std::cout << "\n--- Scenario 5: Cancel Order ---" << std::endl;
    bool cancelled = engine.cancel_order(9);
    std::cout << "Cancelled order 9: " << (cancelled ? "Success" : "Failed") << std::endl;
    
    engine.print_orderbook();
    
    // Scenario 6: No match scenario
    std::cout << "\n--- Scenario 6: No Match (Wide Spread) ---" << std::endl;
    engine.submit_order(11, Side::BUY, 95.0, 200);
    engine.submit_order(12, Side::SELL, 105.0, 150);
    std::cout << "Submitted BUY at 95.0 and SELL at 105.0 (no match)" << std::endl;
    
    engine.print_orderbook();
    
    // Print final statistics
    engine.print_stats();
    
    total_bench.stop();
    total_bench.report("Total Execution Time");
    
    return 0;
}
