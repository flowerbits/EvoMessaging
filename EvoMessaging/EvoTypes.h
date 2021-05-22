#pragma once
#include <stdint.h>

// types for evohome
#define pTypeEvohome 0x45
#define sTypeEvohome 0x00 // Controller

#define pTypeEvohomeZone 0x46 // Seems easier to define a new type here
#define sTypeEvohomeZone 0x00 // Actual temp zone

#define pTypeEvohomeWater 0x47 // Seems easier to define a new type here
#define sTypeEvohomeWater 0x00 // Hot water (Ideally this would just be a zone but for whatever reason evohome treats this differently)

#define pTypeEvohomeRelay 0x44 // Relay
#define sTypeEvohomeRelay 0x00

typedef enum _messageType {
	RQ = 0,
	I = 1,
	W = 2,
	RP = 3
} MessageType;

typedef struct _EvoAddress {
	uint8_t deviceType;
	unsigned int deviceIndex;

	_EvoAddress() = default;
	_EvoAddress(uint8_t * rawData) {
		unsigned int value = (*rawData << 16 | *(rawData + 1) << 8 | *(rawData + 2));
		deviceIndex = value & 0x3ffff;
		deviceType = (value & 0xFC0000) >> 18;
	}
} EvoAddress;

typedef struct _EvoHeaderByte {
	bool HasParameter0, HasParameter1, HasAddress1, HasAddress2, HasAddress0;
	MessageType messageType;

	_EvoHeaderByte() = default;
	_EvoHeaderByte(uint8_t byte) {
		messageType = (MessageType)((byte & 0x30) >> 4);

		unsigned int addr = (byte & 0xC) >> 2;
		HasAddress0 = (addr != 1);
		HasAddress1 = (addr == 0 || addr == 3);
		HasAddress2 = (addr != 3);

		HasParameter0 = (byte & 2);
		HasParameter1 = (byte & 1);
	}
} EvoHeaderByte;


typedef struct _tEVOHOME1
{
	uint8_t len;
	uint8_t type;
	uint8_t subtype;
	uint8_t id1;
	uint8_t id2;
	uint8_t id3;
	uint8_t status;
	uint8_t mode;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hrs;
	uint8_t mins;
	uint8_t action;

} EVOHOME1;

typedef struct _tEVOHOME2
{
	uint8_t len;
	uint8_t type;
	uint8_t subtype;
	uint8_t id1;
	uint8_t id2;
	uint8_t id3;
	uint8_t zone;
	uint8_t updatetype;
	int16_t temperature;
	uint8_t mode;
	uint8_t controllermode;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hrs;
	uint8_t mins;
	uint8_t battery_level;

} EVOHOME2;

typedef struct _tEVOHOME3
{
	uint8_t len;
	uint8_t type;
	uint8_t subtype;
	uint8_t id1;
	uint8_t id2;
	uint8_t id3;
	uint8_t devno;
	uint8_t demand;
	uint8_t updatetype;
	uint8_t battery_level;

} EVOHOME3;