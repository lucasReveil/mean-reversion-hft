#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>
#include "GBMSimulator.h"
#include "Logger.h"
#include "Tick.h"
#include "OrderBook.h"
#include "TradeLogger.h"
#include "Bot.h"
#include "MeanReversionBot.h"
int main(int argc,char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--help") {
        std::cout << "Usage:\n";
        std::cout << "./sim [mode] [seed]\n";
        std::cout << "  mode: 'live' ou 'offline' (par défaut: offline)\n";
        std::cout << "  seed: entier pour fixer le marché (optionnel)\n";
        return 0;
    }
    
    std::string mode ="offline"; // mode par defaut
    if(argc>1){
        mode= argv[1];
    }
    { //scope pour que les fichiers csv soient detruits avant execution du script
        // Initialisation
        double midPrice = 100.00;        // prix de départ
        double spread = 0.05;            // spread initial
        
        TradeLogger tradeLoggerSpead("data/tradesSpread.csv");
        TradeLogger tradeLoggerMRB("data/tradeMRB.csv");

        OrderBook orderBook(midPrice, spread);
        Bot bot;
        MeanReversionBot mrb;

        bot.setLogger(&tradeLoggerSpead); // connecte le logger au bot
        mrb.setLogger(&tradeLoggerMRB);
        // Random setup
        //std::default_random_engine rng(std::random_device{}());
        unsigned int seed = 19051999;
        if(argc>2){
            seed=atoi(argv[2]);
        }
        std::default_random_engine rng(seed);  // fixe le seed pour generer toujours le meme marcher pour les tests
        std::normal_distribution<double> priceMove(0.0, 0.02); // marche aléatoire
        std::uniform_real_distribution<double> spreadNoise(0.03, 0.07); // léger bruit sur le spread
        GBMSimulator sim(100.0, 0.05, 0.3, 1.0/252,seed);
        double price = sim.nextPrice();
        PriceLogger priceLogger("data/market.csv");
        int tickCount=0;
        //const double dt = 1.0 / (365.0 * 24 * 60 * 60 * 100); // 10 ms en année

        //version "live market", génère au fur et a mesur avec pauses de 10ms
        // Boucle infinie = tick toutes les 10 ms
        if(mode=="live"){
            while (true) {
                /* double delta = priceMove(rng);        // petit mouvement aléatoire (remplacé par GMB)
                midPrice += delta;                       // nouveau prix milieu
                */
                tickCount++;
                double price = sim.nextPrice();
                spread = spreadNoise(rng); 
                orderBook.update(price, spread);         // update carnet
                //orderBook.print();                    // affichage
                bot.maybeTrade(orderBook.currentTick,rng);
                mrb.maybeTrade(orderBook.currentTick,price);
                mrb.updateDeque(price);
                double time_ms=tickCount*10.0; //car 1 tick = 10ms, pour log en ms et pas en 10 ms sur un an
                priceLogger.log(time_ms,price);
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // attendre 10ms
            }
        }
        else{
        //version offline rapide ou on genere le marché direct pour plot apres
            for (int tick=0;tick<10000;tick++) {
                double price = sim.nextPrice();
                spread = spreadNoise(rng); 
                orderBook.update(price, spread);         // update carnet
                //orderBook.print();                          // affichage
                
                bot.maybeTrade(orderBook.currentTick,rng); 
                
                mrb.maybeTrade(orderBook.currentTick,price);
                mrb.updateDeque(price);
                double time_ms=tick*10.0; //car 1 tick = 10ms, pour log en ms et pas en 10 ms sur un an
                priceLogger.log(time_ms,price);
            }
        }
    }
    {
        std::cout << "Simulation terminée. Génération du graphique..." << std::endl;
        std::system("python3 scripts/plot_results.py");
    }
    return 0;
}
