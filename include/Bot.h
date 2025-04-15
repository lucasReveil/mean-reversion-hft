#pragma once
#include "Tick.h"
#include "TradeLogger.h"
#include <iostream>
#include <iomanip>
#include <random>
class Bot{
public:
    bool inPosition=false;
    bool waitingToSell=false;
    double entryPrice=0.0;
    double entryTime;
    double PnL =0.0;
    int tradeCounter = 0;
    Tick lastTick;
    TradeLogger* logger = nullptr; // pointeur vers le logger
    void setLogger(TradeLogger* l);
    void maybeTrade(const Tick& currentTick,std::default_random_engine& rng);
    double getPnL();
};