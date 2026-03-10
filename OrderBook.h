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


// Node struct
struct Node{
    Order data;
    atomic<Node*> next;

    Node(Order order) : data(order), next(nullptr) {}
};

// Lockfree Queue
class LockFreeQueue{
    atomic<Node*> head;
    atomic<Node*> tail;

    public:
    
    // Default Constructor
    LockFreeQueue(){
        Node* dummy = new Node(Order{});
        head.store(dummy);
        tail.store(dummy);
    }

    // Enqueue operation
    void enqueue(const Order& order){
        Node* newNode = new Node(order);

        while(true){
            Node* last = tail.load(memory_order_acquire);
            Node* next = last->next.load(memory_order_acquire);

            if(last == tail.load()){
                if(next == nullptr){
                    if(last->next.compare_exchange_weak(next, newNode)){
                        tail.compare_exchange_weak(last, newNode);
                        return;
                    }
                }
                else{
                    tail.compare_exchange_weak(last, next);
                }
            }
        }
    }

    // Dequeue operation
    bool dequeue(Order& result){
        while(true){
            Node* first = head.load();
            Node* last = tail.load();
            Node* next = first->next.load();

            if(first == head.load()){
                if(first == last){
                    if(next == nullptr){
                        return false;
                    }
                    tail.compare_exchange_weak(last, next);
                }
                else{
                    result = next->data;
                    if (head.compare_exchange_weak(first, next)) {
                        delete first;
                        return true;
                    }
                }
            }
        }
    }

    bool empty(){
        Node* first = head.load();
        return first->next.load() == nullptr;
    }
};


class LockFreeOrderBook{
    static constexpr int MAX_PRICE = 1000000;   // $10,000.00

    vector<LockFreeQueue> bids;
    vector<LockFreeQueue> asks;

    atomic<int> bestBid{0};
    atomic<int> bestAsk{MAX_PRICE};

    public:

    LockFreeOrderBook() : bids(MAX_PRICE), asks(MAX_PRICE){}

    // // Add Order
    // void addOrder(const Order& order);

    // // Cancel Order
    // void cancelOrder(uint64_t orderId);

    // // Get Best Bid
    // double getBestBid() const;

    // // Get Best Ask
    // double getBestAsk() const;

    // // Print Book
    // void printBook() const;
};

#endif