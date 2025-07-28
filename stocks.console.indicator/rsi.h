#pragma once
#ifndef RSI_H
#define RSI_H

#include <iostream>
#include <vector>

void printRSIHelp();
double calculateRSI(const std::vector<double>& prices, const int period);
double calculateRSIWilder(const std::vector<double>& prices, const int period);
int getRSISignal(double rsi);
void printRSIAnalysis(double currentPrice, double rsi, int signal);
#endif