#pragma once
#include "Tick.h"

class OrderBook {
public:
    Tick currentTick;
    OrderBook(double initialPrice, double initialSpread);
    void update(double midPrice, double spread);
    void print() const;
};
