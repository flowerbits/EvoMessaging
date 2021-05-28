#pragma once
#include "EvoTypes.h"
#include "EvoMessage.h"

class MessageFactory
{
public:
	static EvoMessage* GenericRequest(EvoAddress sourceAddress, EvoAddress destinationAddress, Operations opcode);
	static EvoMessage* GenericBroadcast(EvoAddress sourceAddress, Operations opcode);
	static EvoMessage* AdvertiseBinding(EvoAddress sourceAddress, uint8_t zoneIndex, uint16_t supportedOperations[], uint8_t featureCount);
	static EvoMessage* AdvertiseSetpoint(EvoAddress sourceAddress, uint8_t zoneIndex, int setpoint);
	static EvoMessage* AdvertiseBatteryState(EvoAddress sourceAddress, uint8_t zoneIndex, bool batteryLow, double batteryLevel);
	static EvoMessage* AdvertiseTemperature(EvoAddress sourceAddress, uint8_t zoneIndex, int temperature);
	static EvoMessage* AdvertiseRelayFailsafe(EvoAddress sourceAddress, uint8_t zoneIndex, bool enabled);
	static EvoMessage* AdvertiseRelayDemand(EvoAddress sourceAddress, uint8_t zoneIndex, double percentage);
	static EvoMessage* RequestActuatorCycle(EvoAddress sourceAddress, EvoAddress destinationAddress, uint8_t zoneIndex);
};