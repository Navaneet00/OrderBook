# C++ Limit Order Book (Matching Engine Core)

A **high-performance C++ implementation of a Limit Order Book**, similar to the core infrastructure used in modern electronic exchanges and high-frequency trading systems.

This project simulates how financial markets manage **buy and sell orders**, maintain price levels, and prepare orders for matching.

The focus is on **data structures, performance, and system design**, using modern C++ and STL containers.

---

# What is a Limit Order Book?

A **Limit Order Book (LOB)** is the central component of most financial exchanges.

Market participants submit two types of orders:

* **Bid (Buy Order)** — willing to buy at a specific price
* **Ask (Sell Order)** — willing to sell at a specific price

Orders are organized by **price levels** and **time priority**.

When:

Bid Price ≥ Ask Price

a **trade match occurs**, and the order matching engine executes the trade.

---

# Project Structure

OrderBook/

├── main.cpp              # Entry point / simulation runner
├── Order.h               # Order structure definition
├── OrderBook.h           # OrderBook class interface
├── OrderBook.cpp         # OrderBook implementation
└── MatchingEngine.h      # Matching logic (future expansion)

---

# Order Structure

Each order contains:

* **orderId** — unique identifier
* **side** — BID or ASK
* **price** — limit price
* **quantity** — number of shares/contracts
* **timestamp** — nanosecond precision for time priority

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

* **O(log N)** price level insertion
* **O(1)** order access within a level
* Efficient best bid/ask retrieval

---

# Implemented Features

### Order Management

addOrder(Order)
Adds a new order to the correct price level.

cancelOrder(orderId)
Removes an order from the book.

---

### Market Data

getBestBid()
Returns the highest buy price currently in the book.

getBestAsk()
Returns the lowest sell price currently in the book.

printBook()
Displays the current state of the order book.

---

# Design Goals

* Use **modern C++**
* Emphasize **cache-friendly data structures**
* Maintain **price-time priority**
* Prepare architecture for a **matching engine**

---

# Future Improvements

Matching engine implementation
Trade execution logic
Order modification support
Latency measurement
Lock-free structures for multithreading
Persistence / replay logs
Market order support
Performance benchmarking

---

# Learning Goals

This project helps understand concepts used in:

* High-Frequency Trading (HFT)
* Exchange Matching Engines
* Low-latency C++ systems
* Financial market microstructure
* Concurrent system design

---

# Example

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

If a **bid ≥ ask**, the matching engine executes a trade.

---

# Build

Compile with:

g++ -std=c++17 main.cpp OrderBook.cpp -o orderbook

Run:

./orderbook

---

# Author

Built as part of a **systems programming and trading infrastructure learning project** focusing on **C++ performance engineering and exchange architecture**.
