#pragma once

#include "../stocks.console.api/alpacha.h"
#include <json/value.h>

void HandleIndicatorAnalysis(Alpacha& alpacha);

static int GetValidPeriod();
static int GetValidIndicator();
