#pragma once

#include "../stocks.console.api/alpacha.h"
#include <json/value.h>

static void HandleAccountDetails(Alpacha& alpacha);
static void HandleIndicatorAnalysis(Alpacha& alpacha);
static void HandleOrder(const Alpacha& alpacha, int command, const Json::Value& root);

static int GetValidPeriod();
static int GetValidIndicator();
