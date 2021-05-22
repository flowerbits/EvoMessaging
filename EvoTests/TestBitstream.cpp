#include "pch.h"
#include <Bitstream.h>


uint8_t testData[]{ 0x01, 0x03, 0x07, 0x0F };

TEST(BitstreamTest, EightBits) {
    uint8_t* output;
    int bitCount = 0;
    Bitstream bs;

    bs.Open(4);
    for (int i = 0; i < 4; i++)
        bs.Write(8, testData[i] );

    output = bs.ToByteArray();

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(testData[i], output[i]);

    bs.FreeData();
}


TEST(BitstreamTest, IncrementalBits) {
    uint8_t* output;
    int bitCount = 0;
    Bitstream bs;

    bs.Open(4);
    for (int i = 0; i < 4; i++)
        bs.Write(i+1, testData[i]);

    output = bs.ToByteArray();
    uint8_t expectedOutput[] = { 0xff, 0xc0, 0x00, 0x00 };

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(expectedOutput[i], output[i]);

    bs.FreeData();
}

TEST(BitstreamTest, SixBits) {
    uint8_t* output;
    int bitCount = 0;
    Bitstream bs;

    bs.Open(4);
    for (int i = 0; i < 4; i++)
        bs.Write(6, testData[i]);

    output = bs.ToByteArray();
    uint8_t expectedOutput[] = { 0x04, 0x31, 0xcf, 0x00 };

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(expectedOutput[i], output[i]);

    bs.FreeData();
}