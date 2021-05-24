#include "MessagePrinter.h"
#include <iomanip>
#include <stdio.h>
#include <io.h>
#include <iostream>

using namespace std;

void MessagePrinter::PrintTemperature(ostream& stream, EvoTemperature * temperature)
{
	stream <<  "'temperature': " << (temperature->Celcius() / 100.0);
}

void MessagePrinter::PrintBindFeature(ostream& stream, BindFeature* feature) {
	stream << "['" << uppercase <<  dec << std::setw(2) << setfill('0') << (unsigned int)feature->Zone() << "','" << setw(4) << hex << feature->OpCode() << "','";
	PrintAddress(stream, feature->Address());
	stream << "']";
}

void MessagePrinter::PrintAddress(ostream& stream, EvoAddress* address)
{
	stream << dec << setw(2) << setfill('0') << address->deviceType() << ":" << setw(6) << address->deviceAddress();
}

void MessagePrinter::PrintZoneTemperature(ostream& stream, ZoneTemperature* temperature)
{
	stream << "{'zone': " << uppercase << dec << std::setw(2) << setfill('0') << (unsigned int)temperature->ZoneIndex() << ",";
	PrintTemperature(stream, temperature->Temperature());
	stream << "}";
}


void MessagePrinter::PrintZoneTemperatureArray(ostream& stream, struct EvoArray<ZoneTemperature>* evoArray) {
	stream << "[";
	for (int i = 0; i < evoArray->ValueCount(); i++)
	{
		PrintZoneTemperature(stream, evoArray->Values() + i);
		if (i < evoArray->ValueCount() - 1)
			stream << ",";
	}
	stream << "]";
}

void MessagePrinter::PrintBindFeatureArray(ostream& stream,struct EvoArray<BindFeature>* evoArray) {
	stream << "[";
	for (int i = 0; i < evoArray->ValueCount(); i++)
	{
		PrintBindFeature(stream, evoArray->Values() + i);
		if (i < evoArray->ValueCount()-1)
			stream << ",";
	}
	stream << "]";
}
