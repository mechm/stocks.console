#pragma once
#include <string>
#include "../stocks.console.api/alpacha.h"

#ifndef Main_H
#define Main_H

string GetValidDateOrEmpty();
AssetResult GetValidAssetWithCancel(string& symbol, Alpacha alpacha);
int GetValidPeriod();

#endif