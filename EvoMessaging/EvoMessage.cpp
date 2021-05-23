#include "EvoMessage.h"
#include "string.h"
#include "Manchester.h"
#include <stdio.h>


/// <summary>
/// Loads a message from already manchester decoded bytes.
/// </summary>
/// <param name="data">The raw data to load the message from</param>
/// <param name="length">Length of the data</param>
/// <returns>true if the packet is valid</returns>
bool EvoMessage::LoadFromBytes(const uint8_t* data, uint8_t length) {

	for (int i = 0; i < length; i++)
	{
		rawData[i] = data[i];
	}
	return ParseData();
}

/// <summary>
/// Loads a message from manchester encoded data. 
/// </summary>
/// <param name="encodedData">The data to load from</param>
/// <param name="encodedLength">Length should be an even size</param>
/// <returns>true if the packet is valid</returns>
bool EvoMessage::LoadFromManchester(const uint8_t* encodedData, uint8_t encodedLength) {
	rawLength = 0;

	if (encodedLength % 2 != 0) //Manchester requires an even count.
		return false; 

	for (int i = 0; i < encodedLength; i+=2)
		rawData[rawLength++] = (manchester_decode(encodedData[i]) << 4) | manchester_decode(encodedData[i + 1]);

	return ParseData();
}


/// <summary>
/// Parses the raw data into appropriate fields 
/// </summary>
/// <returns>true if the data is a valid message</returns>
bool EvoMessage::ParseData() {
	header = EvoHeaderByte(rawData[0]);

	unsigned int position = 1;

	if (header.HasAddress0) {
		address0 = EvoAddress(rawData + position);
		position += 3;
	}

	if (header.HasAddress1) {
		address1 = EvoAddress(rawData + position);
		position += 3;
	}
		
	if (header.HasAddress2) {
		address2 = EvoAddress(rawData + position);
		position += 3;
	}

	if (header.HasParameter0) {
		param0 = rawData[position];
		position++;
	}
	if (header.HasParameter1){
		param1 = rawData[position];
		position++;
	}

	opCode = rawData[position] << 8 | rawData[position+1];
	position += 2;
	payloadLength = rawData[position++];

	//This makes sure we don't exceed our buffer
	if (payloadLength + position > rawLength)
		return false; 

	payload = rawData+position;
	checksum = rawData[position + payloadLength];

	return ChecksumValid(position+payloadLength+1);
}

/// <summary>
/// Adds all bytes from the raw data to see if the checksum is valid according to a checksum generated with:
/// 0x100 - ((sum of other bytes) & 0xFF)
/// </summary>
/// <param name="packetLength">Length of the packet data from header through checksum</param>
/// <returns>true if the sum is 0</returns>
bool EvoMessage::ChecksumValid(unsigned int packetLength) {
	uint8_t sum = 0;
	for (int i = 0; i <= packetLength; i++)
	{
		sum += rawData[i];
	}
	
	return sum == 0;
}

int EvoMessage::PrintAddress(EvoAddress* address, bool isUsed, char* buffer) {
	if (!isUsed)
		return sprintf(buffer, "--:------ ");

	return sprintf(buffer, "%02u:%06u ", address->deviceType, address->deviceIndex);
}

/// <summary>
/// Prints to the format specified by Evofw3 since this format is consumed by multiple other projects
/// </summary>
bool EvoMessage::ToEvoString(char* buffer) {
	const char* headerType;
	int length = 0;
	if (header.messageType == MessageType::I)
		headerType = " I";
	else if (header.messageType == MessageType::RP)
		headerType = "RP";
	else if (header.messageType == MessageType::W)
		headerType = " W";
	else if (header.messageType == MessageType::RQ)
		headerType = "RQ";
	else headerType = "XX";

	length = sprintf(buffer, "%03X %s ", rssi, headerType);

	if (header.HasParameter1)
		length += sprintf(buffer + length, "%03X ", param1);
	else length += sprintf(buffer + length, "--- ");

	length += PrintAddress(&address0, header.HasAddress0, buffer+length);
	length += PrintAddress(&address1, header.HasAddress1, buffer+length);
	length += PrintAddress(&address2, header.HasAddress2, buffer+length);
	length += sprintf(buffer + length, "%04X %03X ", opCode, payloadLength);
	
	for (int i = 0; i < payloadLength; i++) {
		length += sprintf(buffer + length, "%02X", payload[i]);
	}

	return true;
}