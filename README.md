# C++ Limit Order Book (Matching Engine Core)

A **high-performance C++ implementation of a Limit Order Book**, similar to the core infrastructure used in modern electronic exchanges and high-frequency trading systems.

This project simulates how financial markets manage **buy and sell orders**, maintain price levels, and execute trades via a **matching engine**.

The focus is on **data structures, performance, and system design**, using modern C++ and STL containers, along with **thread safety and benchmarking**.

---

# What is a Limit Order Book?

A **Limit Order Book (LOB)** is the central component of most financial exchanges.

Market participants submit two types of orders:

* **Bid (Buy Order)** — willing to buy at a specific price
* **Ask (Sell Order)** — willing to sell at a specific price

Orders are organized by **price levels** and **time priority** (first-in, first-out within each price).

When:

Bid Price ≥ Ask Price

a **trade match occurs**, and the order matching engine executes the trade.

---

# Project Structure

```
OrderBook/

├── main.cpp              # Entry point / simulation runner
├── Trade.h               # Trade structure definition
├── Order.h               # Order structure definition
├── OrderBook.h           # OrderBook class interface
├── OrderBook.cpp         # OrderBook implementation
├── MatchingEngine.h      # Matching engine declaration
├── MatchingEngine.cpp    # Matching engine implementation
└── README.md             # Project documentation
```
---

# Architecture Overview

       ┌───────────────────────┐
       │   Producer Thread     │
       │ Generates random BIDs │
       │   and ASKs orders     │
       └───────────┬───────────┘
                   │
                   ▼
       ┌────────────────────────┐
       │  Thread-Safe OrderBook │
       │ ┌───────────┐ ┌──────┐ │
       │ │   BIDS    │ │ ASKS │ │
       │ │   100 ↑   │ │ 98 ↓ │ │
       │ │   101 ↑   │ │ 99 ↓ │ │
       │ │   102 ↑   │ │ 100↓ │ │
       │ └───────────┘ └──────┘ │
       └───────────┬────────────┘
                   │
                   ▼
       ┌────────────────────────┐
       │ Matching Engine Thread │
       │ Executes trades when   │
       │ BestBid ≥ BestAsk      │
       │ Updates Trade History  │
       └───────────┬────────────┘
                   │
                   ▼
       ┌─────────────────────────┐
       │ Trade History           │
       │ Records executed trades │
       │ Price, Quantity, IDs    │
       │ Latency per match       │
       └─────────────────────────┘

- **BIDS ladder**: Highest price at top → FIFO execution at each level
- **ASKS ladder**: Highest price at top → FIFO execution at each level
- **Threads**: Producer and Matching Engine threads run concurrently
- **Trade History**: logs all executed trades with latency for analysis.

---

# Order Structure

Each order contains:

* **orderId** — unique identifier
* **side** — BID or ASK
* **price** — limit price
* **quantity** — number of shares/contracts
* **timestamp** — relative time in nanoseconds (for latency and time priority)

This mirrors how real trading systems maintain **price-time priority**.

---

# Core Data Structures

The order book maintains two price ladders:

### Bids (Buy Orders)

std::map<double, std::queue<Order>, std::greater<double>>

* Highest price first
* FIFO execution within the same price level

### Asks (Sell Orders)

std::map<double, std::queue<Order>>

* Lowest price first
* FIFO execution within the same price level

This structure guarantees:

* **O(log N)** insertion for new price levels
* **O(1)** access to first order in each level
* Efficient best bid/ask retrieval

---

# Implemented Features

### Order Management

addOrder(Order) - Adds a new order to the correct price level.

cancelOrder(orderId) - Cancels an order safely with mutex protection.

---

### Market Data

getBestBid() - Returns the highest buy price currently in the book.

getBestAsk() - Returns the lowest sell price currently in the book.

printBook() - Displays the current state of the order book.

---

### Matching Engine

* Executes trades when **BestBid ≥ BestAsk**
* Handles partial fills
* Updates trade history with price, quantity, buyer/seller IDs, and timestamp
* Measures latency per match in nanoseconds

---

### Thread Safety

* **OrderBook** protected by **std::mutex**
* **producer** thread generates orders
* **matching engine** runs in a separate thread
* **std::atomic<bool>** flag to stop threads safely
* Zero race conditions — all operations are thread-safe

---

### Benchmarking

**Example output from a 5-second simulation:**

```
Producer runtime: 5 seconds.

Benchmark Report
-----------------
Total Trades: 3,053,610
Total Volume: 9,245,611
Throughput: 610,722 trades / sec
Average Match Latency: 227 ns
Best Bid: 102
Best Ask: 103
```

* **Throughput:** Orders processed per second
* **Average Match Latency:** Time per trade match in nanoseconds
* **Trade History:** Full log for analysis

---

# Future Improvements

* Multiple producer threads
* Real-time market order support
* Persistent logs / replay
* Lock-free optimizations for ultra-low latency
* Min/Max latency reporting
* Graphical or ASCII visualization of order book depth

---

# Learning Goals

This project helps understand concepts used in:

* High-Frequency Trading (HFT)
* Exchange Matching Engines
* Low-latency C++ systems
* Financial market microstructure
* Concurrent system design
* Thread-safe data structures
* Performance benchmarking

---

# Example Book

Buy Orders (Bids)

| Price | Quantity |
| ----- | -------- |
| 101.5 | 200      |
| 101.0 | 150      |
| 100.5 | 300      |

Sell Orders (Asks)

| Price | Quantity |
| ----- | -------- |
| 102.0 | 100      |
| 102.5 | 250      |
| 103.0 | 400      |

If a **bid ≥ ask**, the matching engine executes a trade, updating the book and trade history.

---

# Build

Compile with:

g++ -std=c++17 main.cpp OrderBook.cpp MatchingEngine.cpp -o orderbook -pthread

Run:

./orderbook

---

# Author

Built as part of a **systems programming and trading infrastructure learning project**, focusing on **C++ performance engineering, concurrency, and exchange architecture**.
