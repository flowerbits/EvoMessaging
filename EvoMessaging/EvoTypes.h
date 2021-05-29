#pragma once
#include <stdint.h>
#include <stdio.h>

//Data holders for the protocol live here.
//Take note that the data types generally only have a size of their memory footprint to avoid having to copy data buffers around.
//This means many of the accessors are functions.
//They use pointers and rely on the data existing in one spot since most of the time the accessors will be used only a few times for each instance

enum class Zones {
	All,
	Rad,
	Ele,
	Val,
	Efh,
	Mix,
	Dhw
};

enum class DeviceCodes {
	RadiatorController1 = 0,
	EvohomeColor = 01,
	UnderfloorController = 02,
	RoomTemperatureSensor = 03,
	RadiatorController2 = 04,
	CylinderThermostat = 07,
	Device_8 = 8,
	OpenthermBridge = 10,
	Dts92e = 12,
	WirelessRelay = 13,
	Weather = 17,
	Device_18 = 18,
	Hvac = 20,
	RoundAnalogThermostat = 21,
	Dts92 = 22,
	Device_23 = 23,
	Gateway = 30,
	Device_31 = 31,
	HvacSensor = 32,
	RoundDigitalThermostat = 34,
	HvacVentilation = 37,
	Switch = 39
};

enum class Operations : uint16_t {
	//RfCheck = 0x0001,
	OutdoorSensor = 0x0002,
	ZoneName = 0x0004,
	SystemZones = 0x0005,
	ScheduleSync = 0x0006,
	RelayDemand = 0x0008,
	RelayFailsafe = 0x0009,
	ZoneParams = 0x000A,
	ZoneDevices = 0x000C,
	RfCheck = 0x0016,
	Language = 0x0100,
	ZoneSchedule = 0x0404,
	SystemFault = 0x0418,
	MixZoneConfig = 0x1030,
	BatteryState = 0x1060,
	DhwParams = 0x10a0,
	DeviceInfo = 0x10e0,
	TpiParams = 0x1100,
	DhwTemperature = 0x1260,
	OutdoorHumidity = 0x1280,
	OutdoorTemperature = 0x1290,
	IndoorHumidity = 0x12A0,
	WindowState = 0x12B0,
	DisplayedTemperature = 0x12C0,
	SystemSync = 0x1F09,
	DhwMode = 0x1f41,
	RfBind = 0x1fc9,
	OpenthermTicker = 0x1fd4,
	SetpointNow = 0x2249,
	UfhSetpoint = 0x22c9,
	BoilerSetpoint = 0x22d9,
	SwitchSpeed = 0x22f1,
	SwitchDuration = 0x22f3,
	ZoneSetpoint = 0x2309,
	ZoneMode = 0x2349,
	SystemMode = 0x2e04,
	ZoneTemperature = 0x30c9,
	SystemTime = 0x313f,
	HeatDemand = 0x3150,
	ExternalVentilation = 0x31e0,
	OpenthermMessage = 0x3220,
	ActuatorSync = 0x3B00,
	ActuatorState = 0x3ef0,
	ActuatorCycle = 0x3ef1,
};

typedef enum _messageType {
	RQ = 0,
	I = 1,
	W = 2,
	RP = 3
} MessageType;

#pragma pack(push,1)

struct  EvoAddress {
	uint8_t data[3];

	uint8_t deviceType()
	{
		return data[0] >> 2;
	}
	void SetValues(uint8_t deviceType, uint32_t deviceAddress)
	{
		data[0] = 0;
		data[1] = 0;
		data[2] = 0;

		data[0] = deviceType << 2 & 0xFC;
		data[0] |= deviceAddress >> 16 & 0x3;
		data[1] = deviceAddress >> 8 & 0xFF;
		data[2] = deviceAddress & 0xFF;
	}

	uint32_t deviceAddress()
	{
		return (data[0] << 16 | data[1] << 8 | data[2]) & 0x03FFFF;
	}
};

struct EvoHeaderByte {
	uint8_t data = 0;

	bool HasParameter0() {
		return (data & 2);
	}

	bool HasParameter1() {
		return (data & 1);
	}

	bool HasAddress0() {
		return ((data & 0xC) >> 2) != 1;
	}

	bool  HasAddress1() {
		return ((data & 0xC) >> 2) == 0 || ((data & 0xC) >> 2) == 3;
	}

	bool HasAddress2() {
		return ((data & 0xC) >> 2) != 3;
	}

	MessageType Type() {
		return (MessageType)((data & 0x30) >> 4);
	}

	void SetMessageType(MessageType type)
	{
		data |= (type << 4) & 0x30;
	}
	void SetParameterFlags(bool p0, bool p1)
	{
		data |= (p0 << 1) & 0x2;
		data |= p1 & 0x1;
	}
	void SetAddressFlags(bool a0, bool a1, bool a2) {
		uint8_t newFlag = 0;
		if (a0 && a1 && a2)
			newFlag = 0;
		else if (a0 && a2)
			newFlag = 2;
		else if (a0 && a1)
			newFlag = 3;
		else //a2 by itself?
			newFlag = 1;

		data |= newFlag << 2 & 0xC;
	}
};

struct OperationCode {
	uint8_t data[2];

	inline uint16_t GetValue()
	{
		return ((data[0] & 0xFF) << 8) | data[1] & 0xFF;
	}

	inline void SetValue(uint16_t shortValue)
	{
		data[0] = shortValue >> 8 & 0xFF;
		data[1] = shortValue & 0xFF;
	}
	inline void SetValue(Operations value)
	{
		uint16_t shortValue = (uint16_t)value;
		SetValue(shortValue);
	}
};

struct BindFeature
{
	uint8_t zoneData;
	uint8_t opCodeData[2];
	uint8_t addressData[3];

	inline EvoAddress* Address()
	{
		return (EvoAddress*)addressData;
	}

	OperationCode* OpCode()
	{
		return (OperationCode*)opCodeData;
	}

	uint8_t Zone()
	{
		return zoneData;
	}
};


struct EvoTemperature
{
	uint8_t data[2];

	inline int16_t Value()
	{
		return data[0] << 8 | data[1];
	}

	inline bool IsValid()
	{
		return Value() != 0xFF31 && Value() != 0xFF7E && Value() != 0xFFF7;
	}

	/// <summary>
	/// Returns the temperature in celcius, thousands of a degree. 
	/// </summary>
	int Celcius()
	{
		int result = Value();
		if (result >= 0x8000)
			result -= 0x10000;
		return result;
	}

	void SetCelcius(int value)
	{

	}

};

struct EvoPercentage
{
	uint8_t data;

	double Percentage()
	{
		return data / 0xC8;
	}

	void SetPercentage(double value)
	{
		if (value > 100)
			value = 100;
		data = (uint8_t)(0xC8 * value);
	}

	bool HasValue()
	{
		return data <= 0xC8;
	}
};

struct RelayDemand
{
	uint8_t data[2];

	inline uint8_t ZoneIndex()
	{
		return data[0];
	}

	void SetZoneIndex(uint8_t value)
	{
		data[0] = value;
	}

	inline EvoPercentage* Demand()
	{
		return (EvoPercentage*)(data + 1);
	}
};

struct ZoneTemperature
{
	uint8_t data[3];

	inline uint8_t ZoneIndex()
	{
		return data[0];
	}

	void SetZoneIndex(uint8_t value)
	{
		data[0] = value;
	}

	inline EvoTemperature* Temperature()
	{
		return (EvoTemperature*)(data + 1);
	}
	
};

struct ZoneFailsafe
{
	uint8_t data[2];
	uint8_t unknown;

	ZoneFailsafe()
	{
		data[0] = 0;
		data[1] = 0;
		unknown = 0xFF;
	}

	inline uint8_t ZoneIndex()
	{
		return data[0];
	}

	inline bool Enabled()
	{
		return data[1];
	}

	inline void SetEnabled(bool value)
	{
		data[1] = value;
	}

	inline void SetZoneIndex(uint8_t value)
	{
		data[0] = value;
	}
};

struct BatteryStatus
{
	uint8_t data[3];

	inline uint8_t ZoneIndex()
	{
		return data[0];
	}

	inline void SetZoneIndex(uint8_t value)
	{
		data[0] = value;
	}

	inline EvoPercentage* Level()
	{
		return (EvoPercentage*)(data + 1);
	}

	inline uint8_t BatteryOk()
	{
		return data[2];
	}

	inline void SetBatteryOk(bool value)
	{
		data[2] = value;
	}
};

struct BoilerParameters
{
	uint8_t data[5];

	inline uint8_t DomainId()
	{
		return data[0];
	}

	inline double CyclesPerHour()
	{
		return data[1] / 4.0;
	}

	inline double MinOnTime()
	{
		return data[2] / 4.0;
	}

	inline double MinOffTime()
	{
		return data[3] / 4.0;
	}

	inline uint8_t Unknown()
	{
		return data[4];
	}

	void SetCyclesPerHour(double value)
	{
		data[1] = (uint8_t)(value * 4);
	}

	void SetMinOnTime(double value)
	{
		data[2] = (uint8_t)(value * 4);
	}

	void SetMinOffTime(double value)
	{
		data[3] = (uint8_t)(value * 4);
	}

	void SetUnknown(uint8_t value)
	{
		data[4] = value;
	}
};

struct BandwidthData
{
	uint8_t data[3];

	uint8_t Unknown()
	{
		return data[2];
	}

	double Bandwidth()
	{
		return (data[0] << 8 | data[1]) / 100.0;
	}

	void SetBandwidth(double value)
	{
		uint16_t bw = 0;
		bw = value * 100;
		data[1] = bw & 0xFF;
		data[0] = (bw >> 8) & 0xFF;
	}

	void SetUnkown(uint8_t value)
	{
		data[2] = value;
	}
};

struct TpiParameters {
	uint8_t* data;
	uint8_t dataLength;

	TpiParameters() = default;

	TpiParameters(uint8_t* d, uint8_t length)
	{
		data = d;
		dataLength = length;
	}

	inline bool HasBandwidth()
	{
		return dataLength >= 8;
	}

	BoilerParameters* GetBoilerParameters()
	{
		return (BoilerParameters*)data;
	}

	BandwidthData* GetBandwidthData()
	{
		if (!HasBandwidth())
			return 0;

		return (BandwidthData*)(data + 5);
	}
};
struct ActuatorCycleRequest
{
	uint8_t data[2];

	inline uint8_t Padding()
	{
		return data[0];
	}

	inline uint8_t ZoneIndex()
	{
		return data[1];
	}

	inline void SetPadding(uint8_t value)
	{
		data[0] = value;
	}

	inline void SetZoneIndex(uint8_t value)
	{
		data[1] = value;
	}
};

struct ActuatorCycleResponse
{
	uint8_t data[7];

	inline uint8_t Padding()
	{
		return data[0];
	}

	inline uint16_t CycleCountdown()
	{
		return data[1] << 8 | data[2];
	}

	inline uint16_t ActuatorCountdown()
	{
		return data[3] << 8 | data[4];
	}

	inline EvoPercentage* ModulationLevel()
	{
		return (EvoPercentage*)(data + 5);
	}

	inline uint8_t Flags() {
		return data[6];
	}
};


struct ActuatorState {
	uint8_t data[3];

	inline uint8_t SetPadding(uint8_t value)
	{
		data[0] = value;
	}
	inline uint8_t SetFlags(uint8_t value)
	{
		data[2] = value;
	}

	inline uint8_t Padding() {
		return data[0];
	}

	inline uint8_t Flags() {
		return data[2];
	}

	inline EvoPercentage* ModulationLevel()
	{
		return (EvoPercentage*)(data + 1);
	}

	inline bool IsEnabled()
	{
		return ModulationLevel()->Percentage() > 0;
	}
};

template <typename T>
struct EvoArray {
	uint8_t* data;
	uint8_t dataLength;

	EvoArray() = default;

	EvoArray(uint8_t* d, uint8_t length)
	{
		data = d;
		dataLength = length;
	}
	inline T* Values()
	{
		return (T*)data;
	}

	inline uint8_t ValueCount()
	{
		return dataLength / sizeof(T);
	}

};

#pragma pack(pop)
