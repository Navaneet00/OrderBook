#include "OrderBook.h"
#include <iostream>
using namespace std;

void OrderBook::addOrder(const Order& order){
    if(order.side == Side::BID){
        bids[order.price].push(order);
    }
    else{
        asks[order.price].push(order);
    }
}

void OrderBook::cancelOrder(uint64_t orderId){

    // Search bids
    for (auto& [price, q] : bids){
        queue<Order> temp;

        while(!q.empty()){
            Order o = q.front();
            q.pop();

            if(o.orderId != orderId){
                temp.push(o);
            }
        }

        q = move(temp);
    }

    // Search asks
    for (auto& [price, q] : asks){
        queue<Order> temp;

        while(!q.empty()){
            Order o = q.front();
            q.pop();

            if(o.orderId != orderId){
                temp.push(o);
            }
        }

        q = move(temp);
    }
}

double OrderBook::getBestBid() const{
    if(bids.empty()){ return 0; }

    return bids.begin()->first;
}

double OrderBook::getBestAsk() const{
    if(asks.empty()) { return 0; }

    return asks.begin()->first;
}

void OrderBook::printBook() const{

    cout << "----- Order Book -----" << endl;
    cout << "Bids: " << endl;

    for (auto& [price, q] : bids){
        cout << "Price: " << price << " | Orders: " << q.size() << endl;
    }

    cout << endl << "Asks: " << endl;

    for (auto& [price, q] : asks){
        cout << "Price: " << price << " | Orders: " << q.size() << endl;
    }

    cout << "---------------" << endl;
}