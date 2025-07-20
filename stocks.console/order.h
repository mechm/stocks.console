#pragma once
#include <json/value.h>
#include "../stocks.console.api/alpacha.h"

void HandleOrder(Alpacha& alpacha, int command, const Json::Value& root);
