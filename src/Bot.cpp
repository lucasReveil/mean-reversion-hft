// v1 du bot
#include "Bot.h"
#include <random>
#include <iostream>
#include <iomanip>
void Bot::setLogger(TradeLogger* l) {
    logger = l;
}
void Bot::maybeTrade(const Tick& currentTick,std::default_random_engine& rng) {
    static std::bernoulli_distribution fillChance(0.7); // 70% de chance que l'ordre soit exécuté

     if(waitingToSell) {
         double exitPrice = currentTick.ask;
         double profit = exitPrice - entryPrice;
         PnL += profit;
         tradeCounter++;
         /*std::cout << "[TRADE " << tradeCounter << "] Buy: " << entryPrice
         << " | Sell: " << exitPrice
         << " | Profit: " << profit
         << " | Total PnL: " << PnL << std::endl;
        */

         if (logger) {
             logger->logTrade(tradeCounter, "LONG",entryPrice, exitPrice, profit, PnL,entryTime,currentTick.timestamp);
         }
         waitingToSell = false;
         inPosition = false;
     }
     if(!inPosition && currentTick.spread<0.035) {
         bool filled = fillChance(rng); //tirage aleatoire de  70%
         if(filled){
             entryPrice = currentTick.bid;
             inPosition = true;
             waitingToSell = true;
             entryTime=currentTick.timestamp;
            // std::cout << "[ENTRY] Order filled at bid: " << entryPrice << std::endl;
         }
         else{
            // std::cout << "[ENTRY FAIL] Order NOT filled this tick." << std::endl;
         }
     }
 }
double Bot::getPnL(){
    return PnL;
}