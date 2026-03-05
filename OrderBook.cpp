#include "OrderBook.h"
#include <iostream>
using namespace std;

void OrderBook::addOrder(const Order& order){
    lock_guard<mutex> lock(bookMutex);

    if(order.side == Side::BID){
        bids[order.price].push_back(order);
    }
    else{
        asks[order.price].push_back(order);
    }
    orderIndex[order.orderId] = {order.side, order.price};
}

void OrderBook::cancelOrder(uint64_t orderId){
    lock_guard<mutex> lock(bookMutex);

    if(orderIndex.find(orderId) == orderIndex.end()) { return; }

    auto [side, price] = orderIndex[orderId];

    if(side == Side::BID){
        auto& book = bids;
        auto& dq = book[price];

        for(auto it = dq.begin(); it != dq.end(); it++){
            if(it->orderId == orderId){
                dq.erase(it);
                break;
            }
        }

        if(dq.empty()){
            book.erase(price);
        }
    }
    else{
        auto& book = asks;
        auto& dq = book[price];

        for(auto it = dq.begin(); it != dq.end(); it++){
            if(it->orderId == orderId){
                dq.erase(it);
                break;
            }
        }

        if(dq.empty()){
            book.erase(price);
        }
    }

    orderIndex.erase(orderId);
}

double OrderBook::getBestBid() const{
    lock_guard<mutex> lock(bookMutex);

    if(bids.empty()){ return 0; }

    return bids.begin()->first;
}

double OrderBook::getBestAsk() const{
    lock_guard<mutex> lock(bookMutex);

    if(asks.empty()) { return 0; }

    return asks.begin()->first;
}

void OrderBook::printBook() const{
    lock_guard<mutex> lock(bookMutex);

    cout << "----- Order Book -----" << endl;
    cout << "Bids: " << endl;

    for (auto& [price, orders] : bids){
        cout << "Price: " << price << " | Orders: " << orders.size() << endl;
    }

    cout << endl << "Asks: " << endl;

    for (auto& [price, orders] : asks){
        cout << "Price: " << price << " | Orders: " << orders.size() << endl;
    }

    cout << "---------------" << endl;
}