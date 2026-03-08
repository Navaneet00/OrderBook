#include "OrderBook.h"
#include "MatchingEngine.h"
#include <iostream>
#include <thread>
#include <random>
#include <atomic>
#include <chrono>
#include <signal.h>
using namespace std;

// // Old version
// int main(){
//     OrderBook book;
//     MatchingEngine engine;

//     book.addOrder(Order(1,Side::BID,100,10,1));
//     book.addOrder(Order(2,Side::BID,101,5,2));
//     book.addOrder(Order(3,Side::BID,102,7,3));
//     book.addOrder(Order(4,Side::BID,99,8,4));
//     book.addOrder(Order(5,Side::BID,103,6,5));
//     book.addOrder(Order(6,Side::ASK,104,5,6));
//     book.addOrder(Order(7,Side::ASK,103,6,7));
//     book.addOrder(Order(8,Side::ASK,102,4,8));
//     book.addOrder(Order(9,Side::ASK,101,3,9));
//     book.addOrder(Order(10,Side::ASK,100,8,10));

//     book.addOrder(Order(11,Side::BID,104,4,11));
//     book.addOrder(Order(12,Side::BID,105,3,12));
//     book.addOrder(Order(13,Side::ASK,99,5,13));
//     book.addOrder(Order(14,Side::ASK,98,2,14));
//     book.addOrder(Order(15,Side::BID,106,4,15));
//     book.addOrder(Order(16,Side::ASK,101,6,16));
//     book.addOrder(Order(17,Side::BID,100,5,17));
//     book.addOrder(Order(18,Side::ASK,102,3,18));
//     book.addOrder(Order(19,Side::BID,107,2,19));
//     book.addOrder(Order(20,Side::ASK,100,4,20));

//     cout << "\nInitial Book:\n";
//     book.printBook();
//     cout << endl;

//     book.cancelOrder(2);

//     cout << "After cancel:" << endl;
//     book.printBook();
//     cout << endl;

//     engine.matchOrders(book);

//     cout << "\nAfter matching:\n";
//     book.printBook();

//     cout << "Best Bid: " << book.getBestBid() << endl;
//     cout << "Best Ask: " << book.getBestAsk() << endl;

//     cout << "\nTrade History:\n";

//     for (auto& trade : engine.getTradeHistory()){
//         cout << "BUY: " << trade.buyOrderId << 
//                 " | SELL: " << trade.sellOrderId << 
//                 " | PRICE: " << trade.price << 
//                 " | QUANTITY: " << trade.quantity << 
//                 " | TIMESTAMP: " << trade.timestamp << " ns" << endl;
//     }

//     cout << "Total Volume: " << engine.getTotalVolume() << endl;
//     cout << "Average Match Latency: " << engine.getAverageLatency() << " ns." << endl;

//     return 0;
// }



atomic<bool> running(true);

// Signal Handler
void signalHandler(int signal){
    if(signal == SIGINT){
        std::cout << "\nCaught Ctrl+C, shutting down gracefully..." << std::endl;
        running.store(false);
    }
}


// New Version
int main(){

    signal(SIGINT, signalHandler);

    OrderBook book;
    MatchingEngine engine;

    thread matcher([&](){
        engine.start(book, running);
    });

    thread producer([&](){
        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<> sideDist(0,1);
        uniform_int_distribution<> priceDist(95,110);
        uniform_int_distribution<> qtyDist(1,10);

        uint64_t id = 1;

        auto start = chrono::high_resolution_clock::now();

        while(running){
            Side side = sideDist(gen)? Side::BID : Side::ASK;

            book.addOrder(Order(
                id++,
                side,
                priceDist(gen),
                qtyDist(gen),
                id
            ));
        }

        auto end = chrono::high_resolution_clock::now();

        auto runtime = chrono::duration_cast<chrono::seconds>(end-start).count();

        cout << "Producer runtime: " << runtime << " seconds." << endl;
    });

    this_thread::sleep_for(chrono::seconds(5));

    running.store(false);

    producer.join();
    matcher.join();

    cout << "\nBenchmark Report\n";
    cout << "-----------------\n";

    cout << "Total Trades: " << engine.getTotalTrades() << endl;
    cout << "Total Volume: " << engine.getTotalVolume() << endl;
    cout << "Throughput: " << engine.getTotalTrades() / 5 << " trades / sec" << endl;
    cout << "Average Match Latency: " << engine.getAverageLatency() << " ns." << endl;
    cout << "Best Bid: " << book.getBestBid() << endl;
    cout << "Best Ask: " << book.getBestAsk() << endl;

    return 0;
}