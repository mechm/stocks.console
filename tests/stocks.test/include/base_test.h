#pragma once

#include "indicator.h"

#include <string>

std::string signalToString(Signal signal);
bool areAlmostEqual(double a, double b, double epsilon = 0.01);