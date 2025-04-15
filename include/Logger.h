#pragma once
#include <fstream>
#include <string>

class PriceLogger {
private:
    std::ofstream file;

public:
    PriceLogger(const std::string& filename) {
        file.open(filename);
        file << "time,price\n"; // header CSV
    }

    void log(double time, double price) {
        file << time << "," << price << "\n";
    }

    ~PriceLogger() {
        if (file.is_open()) file.close();
    }
};
