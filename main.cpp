#include "OrderBook.h"
#include "MatchingEngine.h"
#include <iostream>
using namespace std;

int main(){
    OrderBook book;
    MatchingEngine engine;

    book.addOrder(Order(1,Side::BID,100,10,1));
    book.addOrder(Order(2,Side::BID,101,5,2));
    book.addOrder(Order(3,Side::BID,102,7,3));
    book.addOrder(Order(4,Side::BID,99,8,4));
    book.addOrder(Order(5,Side::BID,103,6,5));
    book.addOrder(Order(6,Side::ASK,104,5,6));
    book.addOrder(Order(7,Side::ASK,103,6,7));
    book.addOrder(Order(8,Side::ASK,102,4,8));
    book.addOrder(Order(9,Side::ASK,101,3,9));
    book.addOrder(Order(10,Side::ASK,100,8,10));

    book.addOrder(Order(11,Side::BID,104,4,11));
    book.addOrder(Order(12,Side::BID,105,3,12));
    book.addOrder(Order(13,Side::ASK,99,5,13));
    book.addOrder(Order(14,Side::ASK,98,2,14));
    book.addOrder(Order(15,Side::BID,106,4,15));
    book.addOrder(Order(16,Side::ASK,101,6,16));
    book.addOrder(Order(17,Side::BID,100,5,17));
    book.addOrder(Order(18,Side::ASK,102,3,18));
    book.addOrder(Order(19,Side::BID,107,2,19));
    book.addOrder(Order(20,Side::ASK,100,4,20));

    cout << "\nInitial Book:\n";
    book.printBook();
    cout << endl;

    book.cancelOrder(2);

    cout << "After cancel:" << endl;
    book.printBook();
    cout << endl;

    engine.matchOrders(book);

    cout << "\nAfter matching:\n";
    book.printBook();

    cout << "Best Bid: " << book.getBestBid() << endl;
    cout << "Best Ask: " << book.getBestAsk() << endl;

    cout << "\nTrade History:\n";

    for (auto& trade : engine.getTradeHistory()){
        cout << "BUY: " << trade.buyOrderId << 
                " | SELL: " << trade.sellOrderId << 
                " | PRICE: " << trade.price << 
                " | QUANTITY: " << trade.quantity << 
                " | TIMESTAMP: " << trade.timestamp << " ns" << endl;
    }

    cout << "Total Volume: " << engine.getTotalVolume() << endl;
    cout << "Average Match Latency: " << engine.getAverageLatency() << " ns." << endl;

    return 0;
}