#include "MessageFactory.h"
#include "EvoMessage.h"

EvoMessage* MessageFactory::GenericRequest(EvoAddress sourceAddress, EvoAddress destinationAddress, Operations opcode)
{
	EvoHeaderByte header;
	header.SetAddressFlags(true, false, true);
	header.SetParameterFlags(false, false);
	header.SetMessageType(MessageType::RQ);
	EvoMessage* retVal = new EvoMessage(header);
	*retVal->address0 = sourceAddress;
	*retVal->address2 = destinationAddress;
	retVal->opCode->SetValue(opcode);
	return retVal;
}

EvoMessage* MessageFactory::GenericBroadcast(EvoAddress sourceAddress, Operations opcode) 
{
	EvoHeaderByte header;
	header.SetAddressFlags(true, false, true);
	header.SetParameterFlags(false, false);
	header.SetMessageType(MessageType::I);
	EvoMessage* retVal = new EvoMessage(header);
	*retVal->address0 = sourceAddress;
	*retVal->address2 = sourceAddress;
	retVal->opCode->SetValue(opcode);
	return retVal;
}
 EvoMessage* MessageFactory::AdvertiseBatteryState(EvoAddress sourceAddress, uint8_t zoneIndex, bool batteryOk, double batteryLevel)
{
	 EvoMessage* retVal = GenericBroadcast(sourceAddress, Operations::BatteryState);

	 BatteryStatus* temp = (BatteryStatus*)retVal->payload;
	 *retVal->payloadLength = sizeof(BatteryStatus);

	 temp->SetZoneIndex(zoneIndex);
	 temp->Level()->SetPercentage(batteryLevel);
	 temp->SetBatteryOk(batteryOk);

	 retVal->GenerateChecksum();
	 return retVal;

}


EvoMessage* MessageFactory::AdvertiseSetpoint(EvoAddress sourceAddress, uint8_t zoneIndex, int setpoint)
{
	EvoMessage* retVal = GenericBroadcast(sourceAddress, Operations::ZoneSetpoint);

	ZoneTemperature* temp = (ZoneTemperature*)retVal->payload;
	*retVal->payloadLength = sizeof(ZoneTemperature);

	temp->SetZoneIndex(zoneIndex);
	temp->Temperature()->SetCelcius(setpoint);

	retVal->GenerateChecksum();
	return retVal;
}

EvoMessage* MessageFactory::AdvertiseTemperature(EvoAddress sourceAddress, uint8_t zoneIndex, int temperature)
{
	EvoMessage * retVal = AdvertiseSetpoint(sourceAddress, zoneIndex, temperature);
	retVal->opCode->SetValue(Operations::ZoneTemperature);
	retVal->GenerateChecksum();
	return retVal;
}

EvoMessage* MessageFactory::AdvertiseRelayFailsafe(EvoAddress sourceAddress, uint8_t zoneIndex, bool enabled)
{
	EvoMessage* retVal = GenericBroadcast(sourceAddress, Operations::RelayFailsafe);

	ZoneFailsafe* temp = (ZoneFailsafe*)retVal->payload;
	*retVal->payloadLength = sizeof(ZoneFailsafe);

	temp->SetZoneIndex(zoneIndex);
	temp->SetEnabled(enabled);

	retVal->GenerateChecksum();
	return retVal;
}

EvoMessage* MessageFactory::AdvertiseRelayDemand(EvoAddress sourceAddress, uint8_t zoneIndex, double percentage)
{
	EvoMessage* retVal = GenericBroadcast(sourceAddress, Operations::RelayDemand);
	RelayDemand* temp = (RelayDemand*)retVal->payload;
	*retVal->payloadLength = sizeof(RelayDemand);

	temp->Demand()->SetPercentage(percentage);
	temp->SetZoneIndex(zoneIndex);

	retVal->GenerateChecksum();
	return retVal;
}

EvoMessage* MessageFactory::RequestActuatorCycle(EvoAddress sourceAddress, EvoAddress destinationAddress, uint8_t zoneIndex)
{
	EvoMessage* retVal = GenericRequest(sourceAddress, destinationAddress, Operations::ActuatorCycle);
	ActuatorCycleRequest* temp = (ActuatorCycleRequest*)retVal->payload;
	*retVal->payloadLength = sizeof(ActuatorCycleRequest);

	temp->SetZoneIndex(zoneIndex);

	retVal->GenerateChecksum();
	return retVal;
}

EvoMessage* MessageFactory::AdvertiseBinding(EvoAddress sourceAddress, uint8_t zoneIndex, uint16_t supportedOperations[], uint8_t featureCount)
{
	EvoMessage* retVal = GenericBroadcast(sourceAddress, Operations::RfBind);
	*retVal->payloadLength = featureCount * sizeof(BindFeature);
	EvoArray<BindFeature> features = EvoArray<BindFeature>(retVal->payload, *retVal->payloadLength);

	for (int i = 0; i < featureCount; i++)
	{
		BindFeature* toSet = (features.Values()+i);
		*(toSet->Address()) = sourceAddress;
		toSet->OpCode()->SetValue(supportedOperations[i]);
		toSet->zoneData = zoneIndex;
	}

	retVal->GenerateChecksum();
	return retVal;
}

