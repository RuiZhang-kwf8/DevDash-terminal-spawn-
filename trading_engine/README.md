# Trading Engine

A high-performance C++17 orderbook trading engine with FIFO price-time matching algorithm.

## Features

- **Order Book Management**: Separate buy and sell side order books with price level organization
- **FIFO Price-Time Priority Matching**: Orders matched in price-time priority order (FIFO at each price level)
- **Partial Fill Support**: Orders can be filled in multiple trades
- **Order Cancellation**: Cancel existing orders from the book
- **Trade Callbacks**: Register callbacks to handle executed trades in real-time
- **Performance Metrics**: Track total trades, volume, and notional value
- **Benchmark Support**: Built-in timing for performance analysis

## Architecture

### Core Components

#### `Order` (include/order.h)
- Represents a limit order in the system
- Tracks order state: PENDING, PARTIALLY_FILLED, FILLED, CANCELLED
- Maintains filled quantity and remaining quantity

#### `Trade` (include/trade.h)
- Represents an executed trade between buyer and seller
- Contains buyer order ID, seller order ID, price, quantity, and timestamp

#### `OrderBook` (include/orderbook.h, src/orderbook.cpp)
- Central matching engine
- Maintains separate maps for buy (descending price) and sell (ascending price) orders
- Price levels contain FIFO queues of orders
- Implements matching logic with price-time priority

#### `TradingEngine` (include/trading_engine.h, src/trading_engine.cpp)
- High-level interface to the trading system
- Orchestrates order submission and cancellation
- Maintains trade history
- Provides callbacks for trade execution
- Collects statistics on executed trades

#### `Benchmark` (include/benchmark.h, src/benchmark.cpp)
- High-resolution timer for performance measurement
- Reports elapsed time in milliseconds

## Building

### Prerequisites
- CMake 3.14 or higher
- C++17 compatible compiler (clang, g++, MSVC)

### Build Instructions

```bash
cd trading_engine
mkdir build
cd build
cmake ..
make
```

### Running the Demo

```bash
./trading_engine
```

## Usage Example

```cpp
#include "trading_engine.h"

int main() {
    TradingEngine engine;
    
    // Set callback for trade notifications
    engine.set_trade_callback([](const Trade& trade) {
        std::cout << "Trade executed: " << trade.quantity << " @ " << trade.price << std::endl;
    });
    
    // Submit orders
    engine.submit_order(1, Side::BUY, 100.0, 50);    // BUY 50 @ 100.0
    engine.submit_order(2, Side::SELL, 100.0, 50);   // SELL 50 @ 100.0 -> MATCH!
    
    // Query order book
    engine.print_orderbook();
    
    // Cancel an order
    engine.cancel_order(3);
    
    // Get statistics
    engine.print_stats();
    
    return 0;
}
```

## Order Matching Algorithm

The engine uses a continuous order book model with the following matching rules:

1. **Price Priority**: Orders at better prices match first
2. **Time Priority**: Orders at the same price level match in FIFO order (first in, first out)
3. **Quantity Matching**: Orders match up to their quantity
4. **Partial Fills**: Orders can be partially filled across multiple trades

### Matching Flow

1. Incoming order is submitted
2. Matched against opposite side orders:
   - BUY orders match against SELL orders (best ASK first, then FIFO)
   - SELL orders match against BUY orders (best BID first, then FIFO)
3. Matched portions are removed from both orders
4. Remaining quantity (if any) is added to the order book

## Order Book Structure

```
BUY SIDE (Descending price)          SELL SIDE (Ascending price)
101.0: [Order1(10), Order2(20)]      100.5: [Order3(30)]
100.5: [Order3(50)]                  101.0: [Order4(40), Order5(25)]
100.0: [Order4(100)]                 101.5: [Order6(75)]
```

## Demo Scenarios

The provided demo (`src/main.cpp`) demonstrates:

1. **Simple Buy-Sell Match**: Exact quantity match at same price
2. **Partial Fill**: Buy and sell with remaining quantity posted to book
3. **Aggressive Buy Across Multiple Sells**: Single order crossing multiple price levels
4. **FIFO Price-Time Priority**: Matching against orders in queue order
5. **Order Cancellation**: Removing orders from the book
6. **No Match Scenario**: Orders that don't cross (wide spread)

## Output Example

```
=== Trading Engine Demo ===

--- Scenario 1: Simple Buy-Sell Match ---
Trade: Buyer=1 Seller=2 Price=100 Qty=50 Timestamp=...

=== ORDER BOOK ===

ASK SIDE (Sell Orders):
  No sell orders

BID SIDE (Buy Orders):
  No buy orders
==================

=== TRADING ENGINE STATISTICS ===
Total Trades Executed: 7
Total Volume Traded: 310
Total Notional Value: 30855
Average Trade Price: 99.5323
===================================

Total Execution Time: 0.156 ms
```

## Performance Characteristics

- **Add Order**: O(log n) for book insertion + O(m) for matching where m is matched quantity
- **Cancel Order**: O(log n) + O(k) where k is queue size at price level
- **Get Best Bid/Ask**: O(1) (tree map maintains order)
- **Memory**: O(n) where n is total orders in the book

## Future Enhancements

- [ ] Market order support
- [ ] Stop loss and take profit orders
- [ ] Iceberg orders
- [ ] Order book snapshots
- [ ] Trade statistics and analytics
- [ ] Support for multiple symbols
- [ ] Risk management (position limits, margin)
- [ ] Exchange protocol integration (FIX, REST API)

## File Structure

```
trading_engine/
├── CMakeLists.txt              # Build configuration
├── include/
│   ├── benchmark.h             # Performance timing utilities
│   ├── order.h                 # Order definition
│   ├── trade.h                 # Trade definition
│   ├── orderbook.h             # Order book interface
│   └── trading_engine.h        # Trading engine interface
├── src/
│   ├── main.cpp               # Demo application
│   ├── benchmark.cpp          # Benchmark implementation
│   ├── orderbook.cpp          # Order book implementation
│   └── trading_engine.cpp     # Trading engine implementation
└── build/                      # Build directory (generated)
```

## License

This is an educational implementation of a trading engine for demonstration purposes.

## Author

Created as part of DevDash trading system development.
