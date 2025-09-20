#pragma once

#include "./indicator.h"

#include <vector>

std::vector<double> calculateATR(const std::vector<PriceData>& data, int period);
