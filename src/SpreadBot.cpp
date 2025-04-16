#include "SpreadBot.h"

#include <iomanip>
#include <iostream>
#include <random>

#include "Config.h"

void SpreadBot::setLogger(TradeLogger* l) {
    logger = l;
}

void SpreadBot::maybeTrade(const Tick& currentTick, std::default_random_engine& rng) {
    static std::bernoulli_distribution fillChance(0.7);  // 70% de chance que l'ordre soit exécuté

    if (inPosition) {
        double exitPrice = currentTick.ask;
        double profit = exitPrice - entryPrice;
        PnL += profit;
        tradeCounter++;

        if (logger) {
            logger->logTrade(tradeCounter, "LONG", entryPrice, exitPrice, profit, PnL, entryTime,
                             currentTick.timestamp);
        }
        inPosition = false;
    }
    if (!inPosition && currentTick.spread < Config::SPREAD_THRESHOLD) {
        bool filled = fillChance(rng);
        entryPrice = currentTick.bid;
        inPosition = true;
        entryTime = currentTick.timestamp;
    }
}

double SpreadBot::getPnL() {
    return PnL;
}