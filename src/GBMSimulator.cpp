#include "GBMSimulator.h"

#include <cmath>

GBMSimulator::GBMSimulator(double initialPrice, double mu, double sigma, double dt,
                           unsigned int seed)
    : mu(mu), sigma(sigma), dt(dt), currentPrice(initialPrice), rng(seed), normDist(0.0, 1.0) {}

// Simule un pas de temps : GBM discret
double GBMSimulator::nextPrice() {
    double Z = normDist(rng);  // tirage gaussien
    double drift = (mu - 0.5 * sigma * sigma) * dt;
    double diffusion = sigma * std::sqrt(dt) * Z;
    currentPrice *= std::exp(drift + diffusion);
    return currentPrice;
}

double GBMSimulator::getPrice() const {
    return currentPrice;
}
