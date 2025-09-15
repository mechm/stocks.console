#pragma once

#include "./indicator.h"

#include <vector>

std::vector<double> calculateROC(const std::vector<double>& prices, int period);
std::vector<Signal> getROCSignals(const std::vector<double>& roc_values);
