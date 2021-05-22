#pragma once

#include <stdint.h>

class SpiFramer
{
public:
	static bool FrameData(uint8_t* input, unsigned int inputLength, uint8_t* output, unsigned int outputLength);
	static bool UnframeData(uint8_t* input, unsigned int inputLength, uint8_t* output, unsigned int outputLength);
};

