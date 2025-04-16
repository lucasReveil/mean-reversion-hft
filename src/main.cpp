#include <unistd.h>

#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>

#include "Config.h"
#include "GBMSimulator.h"
#include "Logger.h"
#include "MeanReversionBot.h"
#include "OrderBook.h"
#include "SpreadBot.h"
#include "Tick.h"
#include "TradeLogger.h"

int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--help") {
        std::cout << "Usage:\n";
        std::cout << "./sim -s [seed] -t [ticks]\n";

        std::cout << "  seed: entier pour fixer le marché (optionnel)\n";
        std::cout << "  ticks: nombre de ticks, 0 ou négatifs pour mode live (10 000 défaut)\n";
        return 0;
    }
    int opt;
    unsigned int seed = 19051999;
    int nbTicks = 10000;
    while ((opt = getopt(argc, argv, "s:t:")) != -1) {
        switch (opt) {
            case 's':
                seed = std::stoi(optarg);
                break;
            case 't':
                nbTicks = std::stoi(optarg);
                break;
            default:
                std::cerr << "Usage: ./sim [-s seed] [-t ticks] [-o output_dir]" << std::endl;
                exit(EXIT_FAILURE);
        }
    }
    std::string mode = nbTicks > 0 ? "offline" : "live";
    {  // scope pour que les fichiers csv soient detruits avant execution du script
        // Initialisation
        double spread = Config::INITIAL_SPREAD;
        OrderBook orderBook(Config::INITIAL_PRICE, Config::INITIAL_SPREAD);
        TradeLogger tradeLoggerSpread(Config::LOG_PATH_SPREAD);
        TradeLogger tradeLoggerMRB(Config::LOG_PATH_MRB);
        SpreadBot spreadBot;
        MeanReversionBot mrb;

        spreadBot.setLogger(&tradeLoggerSpread);
        mrb.setLogger(&tradeLoggerMRB);

        if (argc > 2) {
            seed = atoi(argv[2]);
        }
        std::default_random_engine rng(seed);
        std::uniform_real_distribution<double> spreadNoise(0.03,
                                                           0.07);  // léger bruit sur le spread
        GBMSimulator sim(Config::INITIAL_PRICE, Config::DEFAULT_DRIFT, Config::DEFAULT_VOLATILITY,
                         Config::DEFAULT_DT, seed);
        PriceLogger priceLogger(Config::LOG_PATH_MARKET);
        int tickCount = 0;

        // version "live market", génère au fur et a mesur avec pauses de 10ms
        //  Boucle infinie = tick toutes les 10 ms
        if (mode == "live") {
            while (true) {
                tickCount++;
                double price = sim.nextPrice();
                spread = spreadNoise(rng);
                orderBook.update(price, spread);
                spreadBot.maybeTrade(orderBook.currentTick, rng);
                mrb.maybeTrade(orderBook.currentTick, price);
                mrb.updateDeque(price);
                double time_ms = tickCount * Config::TIME_PER_TICK_MS;
                priceLogger.log(time_ms, price);
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(int(Config::TIME_PER_TICK_MS)));
            }
        } else {
            // version offline rapide ou on genere le marché direct pour plot apres
            for (int tick = 0; tick < 10000; tick++) {
                double price = sim.nextPrice();
                spread = spreadNoise(rng);
                orderBook.update(price, spread);
                spreadBot.maybeTrade(orderBook.currentTick, rng);
                mrb.maybeTrade(orderBook.currentTick, price);
                mrb.updateDeque(price);
                double time_ms = tick * Config::TIME_PER_TICK_MS;
                priceLogger.log(time_ms, price);
            }
        }
    }
    {
        std::cout << "Simulation terminée. Génération du graphique..." << std::endl;
        std::system("python3 scripts/plot_results.py");
    }
    return 0;
}
