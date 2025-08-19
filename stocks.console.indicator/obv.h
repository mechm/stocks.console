// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef OBV_H
#define OBV_H

void printOBVHelp();
std::vector<long long> calculateOBV(const std::vector<double>& closingPrices, const std::vector<long long>& volumes);
std::vector<int> getOBVSignals(const std::vector<double>& prices, const std::vector<long long>& obv_values);

#endif //OBV