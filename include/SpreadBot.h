#pragma once
#include <random>

#include "Tick.h"
#include "TradeLogger.h"

class SpreadBot {
   public:
    bool inPosition = false;
    double entryPrice = 0.0;
    double entryTime;
    double PnL = 0.0;
    int tradeCounter = 0;
    Tick lastTick;
    TradeLogger* logger = nullptr;
    void setLogger(TradeLogger* l);
    void maybeTrade(const Tick& currentTick, std::default_random_engine& rng);
    double getPnL();
};