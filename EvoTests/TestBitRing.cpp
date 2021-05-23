#include "pch.h"
#include "BitRing.h"

extern uint8_t testData[];

TEST(BitRingTest, EightBitRing) {
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    for (int i = 0; i < 4; i++)
        br.Write(8, testData[i]);

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(testData[i], (uint8_t)br.Read(8));

    br.FreeData();
}


TEST(BitRingTest, IncrementalBitRing) {
    uint8_t* output;
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    for (int i = 0; i < 4; i++)
        br.Write(i + 1, testData[i]);

    uint8_t expectedOutput[] = { 0xff, 0xc0, 0x00, 0x00 };

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(expectedOutput[i], (uint8_t)br.Read(8));

    br.FreeData();
}

TEST(BitRingTest, SixBitRing) {
    uint8_t* output;
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    for (int i = 0; i < 4; i++)
        br.Write(6, testData[i]);

    uint8_t expectedOutput[] = { 0x04, 0x31, 0xcf, 0x00 };

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(expectedOutput[i], (uint8_t)br.Read(8));

    br.FreeData();
}


TEST(BitRingTest, EightBitWrap) {
    uint8_t* output;
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    br.Write(8, 0x00);
    br.Write(8, 0x00);
    EXPECT_EQ(0, br.Read(8));
    EXPECT_EQ(0, br.Read(8));

    for (int i = 0; i < 4; i++)
        br.Write(8, testData[i]);

    for (int i = 0; i < 4; i++)
        EXPECT_EQ(testData[i], (uint8_t)br.Read(8));

    br.FreeData();
}

TEST(BitRingTest, SixBitWrap) {
    uint8_t* output;
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    br.Write(8, 0x00);
    br.Write(8, 0x00);
    EXPECT_EQ(0, br.Read(8));
    EXPECT_EQ(0, br.Read(8));

    for (int i = 0; i < 4; i++)
        br.Write(6, testData[i]);

    uint8_t expectedOutput[] = { 0x04, 0x31, 0xcf};

    for (int i = 0; i < 3; i++)
        EXPECT_EQ(expectedOutput[i], (uint8_t)br.Read(8));

    br.FreeData();
}


TEST(BitRingTest, SixBitFives) {
    uint8_t* output;
    int bitCount = 0;
    BitRing br;

    br.Open(4);
    br.Write(8, 0x00);
    br.Write(8, 0x00);
    EXPECT_EQ(0, br.Read(8));
    EXPECT_EQ(0, br.Read(8));

    for (int i = 0; i < 4; i++)
        br.Write(6,0x55);

    uint8_t expectedOutput[] = { 0x04, 0x31, 0xcf };

    for (int i = 0; i < 3; i++)
        EXPECT_EQ(0x55, (uint8_t)br.Read(8));

    br.FreeData();
}

