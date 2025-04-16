#pragma once
#include <deque>

#include "Config.h"
#include "Tick.h"
#include "TradeLogger.h"
enum class PositionState { OUT, LONG, SHORT };

class MeanReversionBot {
   public:
    int window = Config::MEAN_REVERSION_WINDOW;
    PositionState inPosition = PositionState::OUT;
    bool waitingToSell = false;
    double entryPrice = 0.0;
    double entryTime;
    double PnL = 0.0;
    int tradeCounter = 0;
    double maxProfitPrice;
    double minProfitPrice;

    std::deque<double> lastPrices = {};
    TradeLogger* logger = nullptr;
    void setLogger(TradeLogger* l);
    double calculAvg();
    double calculSigma(double avg);
    void maybeTrade(const Tick& currentTick, double price);
    void updateDeque(double price);
    double getPnL();
    void exitTrade(bool direction, bool TP, const Tick& currentTick);
};