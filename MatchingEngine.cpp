#include "MatchingEngine.h"
#include <iostream>
#include <chrono>
using namespace std;

void MatchingEngine::matchOrders(OrderBook& book){
    std::lock_guard<std::mutex> lock(book.bookMutex);
    
    while(!book.bids.empty() && !book.asks.empty()){

        auto start = chrono::high_resolution_clock::now();

        auto& bestBidLevel = book.bids.begin()->second;
        auto& bestAskLevel = book.asks.begin()->second;

        Order& bid = bestBidLevel.front();
        Order& ask = bestAskLevel.front();

        if(bid.price < ask.price) { break; }

        double tradePrice = ask.price;
        uint64_t tradeQuantity = min(bid.quantity, ask.quantity);

        Trade trade {
            bid.orderId,
            ask.orderId,
            tradePrice,
            tradeQuantity,
            static_cast<uint64_t>(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count())
        };

        tradeHistory.push_back(trade);
        bid.quantity -= tradeQuantity;
        ask.quantity -= tradeQuantity;

        if(bid.quantity == 0){
            bestBidLevel.pop_front();
        }

        if(ask.quantity == 0){
            bestAskLevel.pop_front();
        }

        if(bestBidLevel.empty()){
            book.bids.erase(book.bids.begin());
        }

        if(bestAskLevel.empty()){
            book.asks.erase(book.asks.begin());
        }

        auto end = chrono::high_resolution_clock::now();

        latencyLog.push_back(chrono::duration_cast<chrono::nanoseconds>(end-start).count());
    }
}

const vector<Trade>& MatchingEngine::getTradeHistory() const{
    return tradeHistory;
}

uint64_t MatchingEngine::getTotalTrades() const{
    return tradeHistory.size();
}

uint64_t MatchingEngine::getTotalVolume() const{
    uint64_t total = 0;

    for(const Trade& trade : tradeHistory){
        total += trade.quantity;
    }
    
    return total;
}

double MatchingEngine::getAverageLatency() const {
    if (latencyLog.empty()) { return 0; }

    long long sum = 0;

    for (auto& log : latencyLog){
        sum += log;
    }

    return (double)(sum / latencyLog.size());
}

void MatchingEngine::start(OrderBook& book){
    while(running){
        matchOrders(book);
        this_thread::sleep_for(chrono::microseconds(50));
    }
}

void MatchingEngine::stop(){
    running = false;
}