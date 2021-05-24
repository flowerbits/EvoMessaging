#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>

uint8_t payload_0008[2][3] = { {0x00, 0xC8 }, {0x00, 0x00} };

TEST(Parsers, 0008) {
    ZoneDemand* demand = (ZoneDemand * )payload_0008[0];
    EXPECT_EQ(demand->Demand()->HasValue(), true);
    EXPECT_EQ(demand->Demand()->Percentage(), 1);
    EXPECT_EQ(demand->ZoneIndex(), 0);
    
    demand = (ZoneDemand*)payload_0008[1];
    EXPECT_EQ(demand->Demand()->HasValue(), true);
    EXPECT_EQ(demand->ZoneIndex(), 0);
    EXPECT_EQ(demand->Demand()->Percentage(), 0);
}
