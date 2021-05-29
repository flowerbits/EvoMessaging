#include "EvoMessage.h"
#include "string.h"
#include "Manchester.h"
#include <stdio.h>


/// <summary>
/// This is used to build a message. Given a properly formatted header and opcode we can deduce the pointers so that the data can be filled in.
/// </summary>
EvoMessage::EvoMessage(EvoHeaderByte fromHeader)
{
	rawData[0] = fromHeader.data;
	LoadPositions();
}

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

void EvoMessage::LoadPositions() {
	header = (EvoHeaderByte*)(rawData);

	unsigned int position = 1;

	if (header->HasAddress0()) {
		address0 = (EvoAddress*)(rawData + position);
		position += 3;
	}

	if (header->HasAddress1()) {
		address1 = (EvoAddress*)(rawData + position);
		position += 3;
	}

	if (header->HasAddress2()) {
		address2 = (EvoAddress*)(rawData + position);
		position += 3;
	}

	if (header->HasParameter0()) {
		param0 = (rawData + position);
		position++;
	}
	if (header->HasParameter1()) {
		param1 = (rawData + position);
		position++;
	}

	opCode = (OperationCode*)(rawData + position);
	position += 2;
	payloadLength = (rawData + position);
	position++;
	payload = rawData + position;

}
/// <summary>
/// Parses the raw data into appropriate fields 
/// </summary>
/// <returns>true if the data is a valid message</returns>
bool EvoMessage::ParseData() {
	LoadPositions();
	//This makes sure we don't exceed our buffer
	if (payload - rawData + *payloadLength > rawLength)
		return false; 

	checksum = payload + *payloadLength;

	return ChecksumValid();
}

/// <summary>
/// Adds all bytes from the raw data to see if the checksum is valid according to a checksum generated with:
/// 0x100 - ((sum of other bytes) & 0xFF)
/// </summary>
/// <returns>true if the sum is 0</returns>
bool EvoMessage::ChecksumValid() {
	uint8_t sum = 0;

	int packetLength = payload - rawData + *payloadLength +1 ;
	for (int i = 0; i < packetLength; i++)
	{
		sum += rawData[i];
	}
	
	return sum == 0;
}

void EvoMessage::GenerateChecksum() {
	uint8_t sum = 0;
	int packetLength = payload - rawData + *payloadLength;

	for (int i = 0; i < packetLength; i++)
	{
		sum += rawData[i];
	}

	rawData[packetLength] = 0x100 - sum & 0xFF;
	rawLength = packetLength + 1; //It is 0 based and includes Checksum
}

int EvoMessage::PrintAddress(EvoAddress* address, bool isUsed, char* buffer) {
	if (!isUsed)
		return sprintf(buffer, "--:------ ");

	return sprintf(buffer, "%02u:%06u ", address->deviceType(), address->deviceAddress());
}

/// <summary>
/// Prints to the format specified by Evofw3 since this format is consumed by multiple other projects
/// </summary>
bool EvoMessage::ToEvoString(char* buffer) {
	const char* headerType;
	int length = 0;
	if (header->Type() == MessageType::I)
		headerType = " I";
	else if (header->Type() == MessageType::RP)
		headerType = "RP";
	else if (header->Type() == MessageType::W)
		headerType = " W";
	else if (header->Type() == MessageType::RQ)
		headerType = "RQ";
	else headerType = "XX";

	length = sprintf(buffer, "%03X %s ", rssi, headerType);

	if (header->HasParameter1())
		length += sprintf(buffer + length, "%03X ", *param1);
	else length += sprintf(buffer + length, "--- ");

	length += PrintAddress(address0, header->HasAddress0(), buffer+length);
	length += PrintAddress(address1, header->HasAddress1(), buffer+length);
	length += PrintAddress(address2, header->HasAddress2(), buffer+length);
	length += sprintf(buffer + length, "%04X %03d ", *opCode, *payloadLength);
	
	for (int i = 0; i < *payloadLength; i++) {
		length += sprintf(buffer + length, "%02X", payload[i]);
	}
	length += sprintf(buffer + length, "\r\n");

	return true;
}
