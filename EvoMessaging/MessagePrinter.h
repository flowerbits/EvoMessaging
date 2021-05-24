#pragma once

#include <sstream>
#include "EvoTypes.h"

class MessagePrinter
{
public:
	static void PrintTemperature(std::ostream &stream, EvoTemperature * temperature);
	static void PrintBindFeature(std::ostream &stream, BindFeature* feature);
	static void PrintAddress(std::ostream &stream, EvoAddress* address);
	static void PrintZoneTemperature(std::ostream& stream, ZoneTemperature* temperature);

	static void PrintBindFeatureArray(std::ostream& stream, struct EvoArray<BindFeature>* evoArray);
	static void PrintZoneTemperatureArray(std::ostream& stream, struct EvoArray<ZoneTemperature>* evoArray);
};

