#ifndef SMA_H
#define SMA_H

#include <vector>

void printSMAHelp();
/// @brief Calculate Simple Moving Average
double calculateSMA(const std::vector<double>& prices, int period);

/// @brief Get buy/sell signal based on current price vs SMA
int getSMASignal(double currentPrice, double smaValue, double threshold);

/// @brief Print SMA analysis
void printSMAAnalysis(const double currentPrice, const double smaValue, const int signal);

#endif // SMA_H