#pragma once

#include <vector>

void printEMAHelp();
void printMACDHelp();

double calculateEMA(const std::vector<double>& prices, int period);
double calculateMACD(const std::vector<double>& prices, int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9);
double calculateMACDSignal(const std::vector<double>& macdValues, int signalPeriod = 9);
double calculateMACDHistogram(double macdLine, double signalLine);