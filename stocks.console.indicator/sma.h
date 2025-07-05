#ifndef SMA_H
#define SMA_H

#include <vector>

/// @brief Calculate Simple Moving Average
double calculateSMA(const std::vector<double>& prices, int period);

/// @brief Get buy/sell signal based on current price vs SMA
int getSMASignal(const double currentPrice, const double smaValue, const double threshold = 0.0);

/// @brief Get SMA signal with price history
int getSMASignalWithHistory(const std::vector<double>& prices, const int period, const double threshold = 0.0);

/// @brief Print SMA analysis
void printSMAAnalysis(const double currentPrice, const double smaValue, const int signal);

#endif // SMA_H