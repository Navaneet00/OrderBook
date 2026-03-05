#ifndef TRADE_H
#define TRADE_H

#include <cstdint>

struct Trade{
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    double price;
    uint64_t quantity;
    uint64_t timestamp;
};

#endif