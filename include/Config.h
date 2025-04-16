#pragma once

namespace Config {

// === Trading params ===
constexpr double SPREAD_THRESHOLD = 0.035;
constexpr double TRAIL_STOP_PCT = 0.03;
constexpr int MEAN_REVERSION_WINDOW = 100;

// === Simulation ===
constexpr double INITIAL_PRICE = 100.0;
constexpr double INITIAL_SPREAD = 0.05;
constexpr double TIME_PER_TICK_MS = 10.0;

// === GBM parameters ===
constexpr double DEFAULT_DRIFT = 0.05;
constexpr double DEFAULT_VOLATILITY = 0.3;
constexpr double DEFAULT_DT = 1.0 / 252.0;

// === Logging ===
constexpr const char* LOG_PATH_MRB = "data/tradeMRB.csv";
constexpr const char* LOG_PATH_SPREAD = "data/tradesSpread.csv";
constexpr const char* LOG_PATH_MARKET = "data/market.csv";

}  // namespace Config
