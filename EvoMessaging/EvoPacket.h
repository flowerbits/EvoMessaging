#pragma once
#include "stdint.h"
#include "EvoMessage.h"

#define MAX_PACKET_SIZE 255

static const uint32_t EVO_SYNC_WORD = 0x00335553;

class EvoPacket
{
public:
	enum class ProcessResult {
		Ok = 0,
		Synchronized = 1,
		HeaderFound = 2,
		MessageComplete = 3,
		Error = 0x8000,
		CollisionError = 0x8001,
		ManchesterError = 0x8002,
		SyncError = 0x8003
	};

	enum class PacketState {
		Ready,
		Receiving,
		Aborted, 
		Collision,
		Completed,
		Error = 0x8001
	};

	EvoPacket();
	~EvoPacket();
	ProcessResult ProcessByte(uint8_t byte);
	bool GetMessage(EvoMessage* message);
	PacketState GetState();
	void SetState(PacketState newState);
	void Reset();

private:
	uint8_t dataBuffer[MAX_PACKET_SIZE] = {};
	unsigned int dataSize;
	uint32_t syncBuffer = 0;
	PacketState currentState;
	ProcessResult currentResult;

	ProcessResult ProcessByte_Ready(uint8_t byte);
	ProcessResult ProcessByte_Receiving(uint8_t byte);
	ProcessResult ProcessByte_Failed(uint8_t byte);
};

