#ifndef MACD_H
#define MACD_H

#include <vector>

double calculateEMA(const std::vector<double>& prices, int period);
double calculateMACD(const std::vector<double>& prices, int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9);
double calculateMACDSignal(const std::vector<double>& macdValues, int signalPeriod = 9);
double calculateMACDHistogram(double macdLine, double signalLine);

#endif // MACD_H#pragma once