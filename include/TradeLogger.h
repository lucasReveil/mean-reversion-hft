#pragma once
#include <fstream>
#include <string>

class TradeLogger {
private:
std::ofstream file;

public:
    TradeLogger(const std::string& filename) {
        file.open(filename);
        file << "trade_id,direction,entry_time,exit_time,buy_price,sell_price,profit,pnl\n"; // header
    }

    ~TradeLogger() {
        file.close();
    }

    void logTrade(int tradeId, std::string direction,double buyPrice, double sellPrice, double profit, double pnl,double entryTime, double exitTime) {
        file << tradeId << "," << direction << "," << entryTime << "," << exitTime << "," << buyPrice << "," << sellPrice << "," << profit << "," << pnl << "\n";
    }
};