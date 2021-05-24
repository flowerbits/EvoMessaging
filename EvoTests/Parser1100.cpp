#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>

uint8_t payload_1100[] = { 0x00, 0x18, 0x04, 0x04, 0xFF, 0x00, 0x96, 0x01 };
//{'cycle_rate': 6, 'min_on_time': 1.0, 'min_off_time': 1.0, '_unknown_0': 'FF', 'proportional_band_width': 1.5, '_unknown_1': '01'}

TEST(Parsers, 1100) {
    char printBuffer[255];

    TpiParameters p = TpiParameters(payload_1100, sizeof(payload_1100));

    EXPECT_EQ(p.GetBoilerParameters()->CyclesPerHour(), 6.0);
    EXPECT_EQ(p.GetBoilerParameters()->MinOnTime(), 1.0 );
    EXPECT_EQ(p.GetBoilerParameters()->MinOffTime(), 1.0 );
    EXPECT_EQ(p.GetBoilerParameters()->Unknown(), 0xFF );
    
    EXPECT_EQ(p.HasBandwidth(), true );
    EXPECT_EQ(p.GetBandwidthData()->Bandwidth(), 1.5);
    EXPECT_EQ(p.GetBandwidthData()->Unknown(), 1);
}


