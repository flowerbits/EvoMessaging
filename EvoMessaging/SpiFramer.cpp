#include "SpiFramer.h"
#include "BitTools.h"
#include "BitStream.h"

/// <summary>
/// Frames the data for transmission on the SPI bus. Takes each byte of data and reverses bit order, then adds the start and stop bit.
/// </summary>
/// <returns>true if successful</returns>
bool SpiFramer::FrameData(uint8_t* input, unsigned int inputLength, uint8_t* output, unsigned int outputLength) {
	if (outputLength < 1.25 * inputLength)
		return false; //Not enough space to frame into

    Bitstream bs;
    bs.OpenBytes(output, outputLength);
    uint16_t temp;

    for (int i = 0; i < inputLength; i++)
    {
        temp = 0;
        temp = (reverseBits(input[i]) << 1) | 0x01;
        bs.Write(10, temp);
    }

    return true;
}

/// <summary>
/// Opposite of frame data.
/// </summary>
bool SpiFramer::UnframeData(uint8_t* input, unsigned int inputLength, uint8_t* output, unsigned int outputLength) {
    Bitstream bs, dbs;
    bs.OpenBytes(input, inputLength);
    dbs.OpenBytes(output, outputLength);
    uint16_t temp;

    for (int i = 0; i < inputLength; i++)
    {
        temp = bs.Read(10);
        temp = reverseBits(temp >> 1);
        dbs.Write(8, temp);
    }
    return true;
}