#pragma once

#include "../../stocks.console.api/include/alpacha.h"

#include <json/value.h>

void HandleOrder(const Alpacha& alpacha, int command, const Json::Value& root);