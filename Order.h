#ifndef ORDER_H
#define ORDER_H

#include <cstdint>

enum class Side {
    BID, ASK
};

struct Order {
    uint64_t orderId;
    Side side;
    double price;
    uint64_t quantity;
    uint64_t timestamp;

    Order(uint64_t o, Side s, double p, uint64_t q, uint64_t t) : orderId(o), side(s), price(p), quantity(q), timestamp(t) {}
};

#endif