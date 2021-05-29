#pragma once
#include "EvoMessage.h"
#include "EvoTypes.h"

class TestHelper {
public:
	static EvoMessage* GetMessageFromFramedData(uint8_t* framedData, int length);
};
