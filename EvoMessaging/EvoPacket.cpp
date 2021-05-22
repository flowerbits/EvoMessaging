#include "EvoPacket.h"
#include "Manchester.h"

EvoPacket::EvoPacket()
{
	currentResult = ProcessResult::Ok;
	currentState = PacketState::Ready;
	dataSize = 0;
}

EvoPacket::~EvoPacket()
{
}

EvoPacket::PacketState EvoPacket::GetState()
{
	return currentState;
}

void EvoPacket::SetState(EvoPacket::PacketState newState)
{
	currentState = newState;
 }

EvoPacket::ProcessResult EvoPacket::ProcessByte_Ready(uint8_t byte)
{
	syncBuffer = (syncBuffer << 8) | byte;
	if (syncBuffer == EVO_SYNC_WORD)
	{
		currentState = PacketState::Receiving;
		return currentResult = ProcessResult::Synchronized;
	}
	return currentResult = ProcessResult::Ok;
}

void EvoPacket::Reset()
{
	currentResult = ProcessResult::Ok;
	currentState = PacketState::Ready;
	dataSize = 0;
}

EvoPacket::ProcessResult EvoPacket::ProcessByte_Receiving(uint8_t byte)
{
	if (byte == 0x00)
	{
		currentState = PacketState::Collision;
		return currentResult = ProcessResult::CollisionError;
	}

	if (byte == 0xF1)
	{
		currentState = PacketState::Aborted;
		return currentResult = ProcessResult::SyncError;
	}

	if (byte == 0x35)
	{
		currentState = PacketState::Completed;
		return currentResult = ProcessResult::MessageComplete;
	}

	if (!manchester_code_valid(byte))
	{
		currentState = PacketState::Error;
		return currentResult = ProcessResult::ManchesterError;
	}

	//Valid data!
	dataBuffer[dataSize++] = byte;
	return currentResult = ProcessResult::Ok;
}

EvoPacket::ProcessResult EvoPacket::ProcessByte_Failed(uint8_t byte)
{
	//Um, why are you trying to do this? It`s in a failed state, so read the error.
	return currentResult;
}

EvoPacket::ProcessResult EvoPacket::ProcessByte(uint8_t byte)
{
	if (currentState == PacketState::Ready)
		return ProcessByte_Ready(byte);
	if (currentState == PacketState::Receiving)
		return ProcessByte_Receiving(byte);
	return ProcessByte_Failed(byte);
}

bool EvoPacket::GetMessage(EvoMessage * message)
{
	if (currentResult != ProcessResult::MessageComplete) return false;

	//Decode and create the message
	*message = EvoMessage();
	return message->LoadFromManchester(dataBuffer, dataSize);
}
