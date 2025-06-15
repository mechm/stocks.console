#pragma once
#ifndef RSI_H
#define RSI_H

#include <iostream>
#include <vector>

double calculateRSI(const std::vector<double>& prices, const int period);
double calculateRSIWilder(const std::vector<double>& prices, const int period);

#endif