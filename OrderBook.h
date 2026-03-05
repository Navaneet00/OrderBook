#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <unordered_map>
#include <deque>
#include "Order.h"
#include <mutex>
#include <functional>
using namespace std;

class OrderBook{

    public:

    // Highest price first
    map<double, deque<Order>, greater<double>> bids;

    // Lowest price first
    map<double, deque<Order>> asks;

    // Order Index
    unordered_map<uint64_t, pair<Side, double>> orderIndex;

    // Book mutex
    mutable mutex bookMutex;

    // Add Order
    void addOrder(const Order& order);

    // Cancel Order
    void cancelOrder(uint64_t orderId);

    // Get Best Bid
    double getBestBid() const;

    // Get Best Ask
    double getBestAsk() const;

    // Print Book
    void printBook() const;
};

#endif