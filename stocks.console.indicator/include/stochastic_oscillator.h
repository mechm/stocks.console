#pragma once

#include "indicator.h"

#include <vector>

std::vector<double> calculateStochasticK(const std::vector<PriceData>& data, int period);
std::vector<double> calculateStochasticD(const std::vector<double>& k_values, int d_period);
std::vector<Signal> getStochasticSignals(const std::vector<double>& k_values, const std::vector<double>& d_values);