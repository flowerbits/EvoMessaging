#pragma once
#include <stdint.h>
#include <stdio.h>

//Data holders for the protocol live here.
//Take note that the data types generally only have a size of their memory footprint to avoid having to copy data buffers around.
//This means many of the accessors are functions.
//They use pointers and rely on the data existing in one spot since most of the time the accessors will be used only a few times for each instance


typedef enum _messageType {
	RQ = 0,
	I = 1,
	W = 2,
	RP = 3
} MessageType;

#pragma pack(push,1)

struct  EvoAddress {
	uint8_t data[3];

	int deviceType()
	{
		return data[0] >> 2;
	}
	unsigned int deviceAddress()
	{
		return (data[0] << 16 | data[1] << 8 | data[2]) & 0x03FFFF;
	}
};

struct EvoHeaderByte {
	uint8_t data;

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
		return ((data & 0xC) >> 2) !=3;
	}

	MessageType Type() {
		return (MessageType)((data & 0x30) >> 4);
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

	uint16_t OpCode()
	{
		return opCodeData[0] << 8 | opCodeData[1];
	}

	uint8_t Zone()
	{
		return zoneData;
	}
};


struct EvoTemperature 
{
	uint8_t data[2];

	inline uint16_t Value() 
	{
		return data[0] << 8 | data[1];
	}

	inline bool IsValid()
	{
		return Value() != 0x31FF && Value() != 0x7EFF && Value() != 0x7FFF;
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
		data =(uint8_t)(0xC8 * value);
	}

	bool HasValue() 
	{
		return data <= 0xC8;
	}
};

struct ZoneDemand
{
	uint8_t data[2];

	inline uint8_t ZoneIndex()
	{
		return data[0];
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

	inline EvoTemperature* Temperature()
	{
		return (EvoTemperature*)(data + 1);
	}
};

struct BatteryStatus
{
	uint8_t data[3];

	inline uint8_t ZoneIndex()
	{
		return data[0];
	}

	inline EvoPercentage* Level()
	{
		return (EvoPercentage*)(data + 1);
	}

	inline uint8_t BatteryOk()
	{
		return data[2];
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

	TpiParameters(uint8_t * d, uint8_t length)
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
