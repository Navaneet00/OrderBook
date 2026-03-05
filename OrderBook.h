#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <queue>
#include "Order.h"
#include <vector>
#include <functional>
using namespace std;

class OrderBook{
    private:

    // Highest price first
    map<double, queue<Order>, greater<double>> bids;

    // Lowest price first
    map<double, queue<Order>> asks;

    public:

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