#pragma once
#include "Tick.h"
#include "TradeLogger.h"
#include <iostream>
#include <iomanip>
#include <random>
#include <deque>
class MeanReversionBot{
public:
    int window=100;
    int inPosition=0; // -- 0 = out / 1= long / 2=short
    bool waitingToSell=false;
    double entryPrice=0.0;
    double entryTime;
    double PnL =0.0;
    int tradeCounter = 0;
    double maxProfitPrice; // pour le long
    double minProfitPrice; // pour le short

    std::deque<double> lastPrices = {};
    TradeLogger* logger = nullptr; // pointeur vers le logger
    void setLogger(TradeLogger* l);
    double calculAvg();
    double calculSigma(double avg);
    void maybeTrade(const Tick& currentTick,double price);
    void updateDeque(double price);
    double getPnL();
    void exitTrade(bool direction, bool TP,const Tick& currentTick);
    
};