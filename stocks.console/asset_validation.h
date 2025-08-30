#pragma once

#include "../stocks.console.api/alpacha.h"

AssetResult GetValidAssetWithCancel(const Alpacha& alpacha);
bool isUserCancelling(const std::string& input);
