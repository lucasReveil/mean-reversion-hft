#pragma once
#include <random>

class GBMSimulator {
   private:
    double mu;     // drift (rendement moyen, ex: 0.05 pour 5% annuel)
    double sigma;  // volatilité annuelle (ex: 0.2 pour 20%)
    double dt;     // pas de temps (en années, ex: 1/252 pour un jour)
    double currentPrice;
    std::default_random_engine rng;
    std::normal_distribution<double> normDist;

   public:
    GBMSimulator(double initialPrice, double mu, double sigma, double dt, unsigned int seed = 0);
    double nextPrice();
    double getPrice() const;
};
