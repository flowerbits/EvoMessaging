#include "pch.h"
#include "SpiFramer.h"

uint8_t toFrame[] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0x00, 0x33, 0x55, 0x53, 0x55, 0x55 };
uint8_t framedData[] = { 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5f, 0xf0, 0x05, 0x99, 0x55, 0x59, 0x55, 0x55, 0x55 };

TEST(FramingTests, FrameMessage) {
    uint8_t results[sizeof(framedData)] = {};
    
    bool result = SpiFramer::FrameData(toFrame, sizeof(toFrame), results, sizeof(framedData));
    EXPECT_EQ(result, true);

    for (int i = 0; i < sizeof(framedData); i++) {
        EXPECT_EQ(results[i], framedData[i]);
    }
}

TEST(FramingTests, UnFrameMessage) {
    uint8_t results[sizeof(toFrame)] = {};

    bool result = SpiFramer::UnframeData(framedData, sizeof(framedData), results, sizeof(toFrame));
    EXPECT_EQ(result, true);

    for (int i = 0; i < sizeof(toFrame); i++) {
        EXPECT_EQ(results[i], toFrame[i]);
    }
}