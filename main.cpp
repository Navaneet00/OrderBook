#include "OrderBook.h"
#include <iostream>
using namespace std;

int main(){
    OrderBook book;

    book.addOrder(Order(1, Side::BID, 100.5, 10, 1));
    book.addOrder(Order(2, Side::BID, 101.0, 5, 2));
    book.addOrder(Order(3, Side::ASK, 102.0, 8, 3));
    book.addOrder(Order(4, Side::ASK, 103.0, 12, 4));

    book.printBook();
    cout << endl;

    cout << "Best Bid: " << book.getBestBid() << endl;
    cout << "Best Ask: " << book.getBestAsk() << endl;
    
    cout << endl;

    book.cancelOrder(2);
    cout << "After cancel:" << endl;
    book.printBook();

    return 0;
}