#pragma once

#include "../../../stocks.console.api/include/alpacha.h"

AssetResult GetValidAssetWithCancel(const Alpacha& alpacha);
bool isUserCancelling(const std::string& input);