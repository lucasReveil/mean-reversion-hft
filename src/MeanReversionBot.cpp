#include "MeanReversionBot.h"

#include <cmath>
#include <iostream>

void MeanReversionBot::setLogger(TradeLogger* l) {
    logger = l;
}

double MeanReversionBot::calculAvg() {
    double s = 0;
    for (int i = 0; i < MeanReversionBot::lastPrices.size(); i++) {
        s += MeanReversionBot::lastPrices[i];
    }
    return s / MeanReversionBot::lastPrices.size();
}

double MeanReversionBot::calculSigma(double avg) {
    double s = 0;
    for (int i = 0; i < MeanReversionBot::lastPrices.size(); i++) {
        s += std::pow((lastPrices[i] - avg), 2);
    }
    double variance = s / (lastPrices.size() - 1.0);  // n-1 car echantillon
    return std::sqrt(variance);
}

void MeanReversionBot::maybeTrade(const Tick& currentTick, double price) {
    if (lastPrices.size() <
        MeanReversionBot::window) {  // attendre d'avoir assez de data pour moyenne glissante
        return;
    }
    double avg = calculAvg();
    double sigma = calculSigma(avg);
    double lowerBand = avg - 2 * sigma;  // formule des bandes de bollinger
    double upperBand = avg + 2 * sigma;
    double lastPrice = lastPrices.back();
    switch (inPosition) {
        case PositionState::OUT:

            // Signal Long
            if (lastPrice < lowerBand && price >= lowerBand) {  // cross up - retour vers la moyenne
                inPosition = PositionState::LONG;
                entryPrice = currentTick.bid;
                maxProfitPrice = entryPrice;
                entryTime = currentTick.timestamp;
                std::cout << "[ENTRY] Long at bid: " << entryPrice << std::endl;
            }

            // Signal Short
            if (lastPrice > upperBand && price <= upperBand) {  // cross down
                inPosition = PositionState::SHORT;
                entryPrice = currentTick.ask;
                minProfitPrice = entryPrice;
                entryTime = currentTick.timestamp;
                std::cout << "[ENTRY] Short at ask: " << entryPrice << std::endl;
            }
            break;
        case PositionState::LONG:
            // trailing stop
            if (price > maxProfitPrice) {
                maxProfitPrice = price;
            }
            if (price < maxProfitPrice * (1 - Config::TRAIL_STOP_PCT)) {
                exitTrade(1, 1, currentTick);
            } else if (price < avg - (3 * sigma)) {  // stop loss
                exitTrade(1, 0, currentTick);
            }
            break;
        case PositionState::SHORT:
            if (price < minProfitPrice) {
                minProfitPrice = price;
            }
            if (price > minProfitPrice * (1 - Config::TRAIL_STOP_PCT)) {
                exitTrade(0, 1, currentTick);
            } else if (price > avg + (3 * sigma)) {
                exitTrade(0, 0, currentTick);
            }
            break;
    }
}

void MeanReversionBot::exitTrade(bool isLong, bool isTP, const Tick& currentTick) {
    double exitPrice = isLong ? currentTick.bid : currentTick.ask;
    double profit = isLong ? (exitPrice - entryPrice) : (entryPrice - exitPrice);
    PnL += profit;
    tradeCounter++;
    std::string direction = isLong ? "LONG" : "SHORT";
    std::string reason = isTP ? "TAKE PROFIT" : "STOP LOSS";
    std::cout << "[TRADE " << tradeCounter << " " << reason << "] "
              << "Entry: " << entryPrice << " | Exit: " << exitPrice << " | PnL: " << profit
              << " | Total: " << PnL << std::endl;

    if (logger) {
        logger->logTrade(tradeCounter, direction, entryPrice, exitPrice, profit, PnL, entryTime,
                         currentTick.timestamp);
    }

    inPosition = PositionState::OUT;
}

void MeanReversionBot::updateDeque(double price) {
    if (MeanReversionBot::lastPrices.size() >= MeanReversionBot::window) {
        lastPrices.pop_front();
    }
    lastPrices.push_back(price);
}

double MeanReversionBot::getPnL() {
    return PnL;
}