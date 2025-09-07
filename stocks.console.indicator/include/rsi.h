#pragma once

#include <vector>

void printRSIHelp();
double calculateRSI(const std::vector<double>& prices, int period);
double calculateRSIWilder(const std::vector<double>& prices, int period);
int getRSISignal(double rsi);
void printRSIAnalysis(double currentPrice, double rsi, int signal);