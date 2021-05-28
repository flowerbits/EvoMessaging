#pragma once
#define MAX_MESSAGE_SIZE 50

#include <stdint.h>
#include "EvoTypes.h"

class EvoMessage
{
private:
	int PrintAddress(EvoAddress* address, bool isUsed, char* buffer );
	void LoadPositions();
public:
	/// <summary>
	/// This holds the manchester encoded data. It must be twice the possible message size.
	/// </summary>
	uint8_t rawData[MAX_MESSAGE_SIZE] = {};

	EvoHeaderByte *header;
	uint8_t rawLength;

	uint16_t rssi;

	uint8_t *param0, *param1, *payloadLength, *checksum;
	EvoAddress *address0, *address1, *address2;
	uint8_t* payload;
	OperationCode* opCode;

	EvoMessage() = default;
	~EvoMessage() = default;
	EvoMessage(EvoHeaderByte fromHeader);
	bool ChecksumValid();
	void GenerateChecksum();
	bool ParseData();
	bool LoadFromManchester(const uint8_t* encodedData, uint8_t encodedLength);
	bool LoadFromBytes(const uint8_t* data, uint8_t length);
	bool ToEvoString(char* buffer);
	
};

