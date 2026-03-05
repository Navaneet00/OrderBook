#ifndef MATCHINGENGINE_H
#define MATCHINGENGINE_H

#include "Trade.h"
#include "OrderBook.h"
#include <cstdint>

class MatchingEngine{
    private:
    vector<Trade> tradeHistory;
    vector<long long> latencyLog;

    public:

    // Match orders
    void matchOrders(OrderBook& book);

    // Get Trade History
    const vector<Trade>& getTradeHistory() const;

    // Get Total Volume
    uint64_t getTotalVolume() const;

    // Get Average Latency
    double getAverageLatency() const;
};

#endif