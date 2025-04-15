#include "MeanReversionBot.h"
#include <iostream>
#include <cmath>

void MeanReversionBot::setLogger(TradeLogger* l) {
    logger = l;
}
double MeanReversionBot::calculAvg(){
    double s=0;
    for(int i=0;i<MeanReversionBot::lastPrices.size();i++){
        s+=MeanReversionBot::lastPrices[i];
    }
    return s/MeanReversionBot::lastPrices.size();
}
double MeanReversionBot::calculSigma(double avg){
    double s=0;
    for(int i=0;i<MeanReversionBot::lastPrices.size();i++){
        s+=std::pow((lastPrices[i]-avg),2); //somme des ecarts a la moyenne au carré
    }
    double variance = s/(lastPrices.size()-1.0); //n-1 car echantillon
    return std::sqrt(variance);
}
void MeanReversionBot::maybeTrade(const Tick& currentTick,double price){
    if(lastPrices.size()<MeanReversionBot::window){ //attendre d'avoir assez de data pour moyenne glissante
        return;
    }
    double avg = calculAvg();
    double sigma = calculSigma(avg);
    if(inPosition==0){ // pas en position
       // double lastPrice = lastPrices[lastPrices.size() - 2]; // avant-dernier tick
        double lowerBand = avg - 2 * sigma; //formule des bandes de bollinger mu+-2sigma
        double upperBand = avg + 2 * sigma;
        double lastPrice = lastPrices.back();
        // Signal Long
        if (lastPrice < lowerBand && price >= lowerBand) { //cross up - retour vers la moyenne 
            inPosition = 1;
            entryPrice = currentTick.bid;
            maxProfitPrice=entryPrice;
            entryTime = currentTick.timestamp;
            std::cout << "[ENTRY] Long at bid: " << entryPrice << std::endl;
            return; // return pour eviter de test conditions de sortie dans le même tick
        }
        
        // Signal Short
        if (lastPrice > upperBand && price <= upperBand) { //cross down
            inPosition = 2;
            entryPrice = currentTick.ask;
            minProfitPrice=entryPrice;
            entryTime = currentTick.timestamp;
            std::cout << "[ENTRY] Short at ask: " << entryPrice << std::endl;
            return;
        }
    }
    if(inPosition==1){ //  Long = on cherche a resell
        /*
        //sortie a croisement souple de moyenne
        if(price>avg-(sigma*0.3)){ //take profit
            exitTrade(1,1,currentTick);
        }
        */
       // version trailing stop
        if(price>maxProfitPrice){
            maxProfitPrice=price;
        }
        if(price<maxProfitPrice*0.9){
            exitTrade(1,1,currentTick);
        } 
        else if(price<avg-(3*sigma)){ //stop loss
            exitTrade(1,0,currentTick);
        }
    }
    if(inPosition==2){ // Short = on wait rebuy
        if(price<minProfitPrice){
            minProfitPrice=price;
        }
        if(price>minProfitPrice*1.1){
            exitTrade(0,1,currentTick);
        }
        else if(price>avg+(3*sigma)){ //stop loss
            exitTrade(0,0,currentTick);
        }
    }
}
void MeanReversionBot::exitTrade(bool isLong, bool isTP,const Tick& currentTick){
        double exitPrice = isLong ? currentTick.bid : currentTick.ask;
        double profit = isLong ? (exitPrice - entryPrice) : (entryPrice - exitPrice);
        PnL += profit;
        tradeCounter++;
        std::string direction = isLong ? "LONG" : "SHORT";
        std::string reason = isTP ? "TAKE PROFIT" : "STOP LOSS";
        std::cout << "[TRADE " << tradeCounter << " " << reason << "] "
                  << "Entry: " << entryPrice << " | Exit: " << exitPrice
                  << " | PnL: " << profit << " | Total: " << PnL << std::endl;
    
        if (logger) {
            logger->logTrade(tradeCounter,direction,entryPrice, exitPrice, profit, PnL,entryTime,currentTick.timestamp);
        }
    
        inPosition = 0;
}
void MeanReversionBot::updateDeque(double price){
    if(MeanReversionBot::lastPrices.size()>=MeanReversionBot::window){
        lastPrices.pop_front();
    }
    lastPrices.push_back(price);
}
double MeanReversionBot::getPnL(){
    return PnL;
}