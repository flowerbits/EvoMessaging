#include "pch.h"
#include "EvoTypes.h"

uint8_t payload_1060[] = { 0x00, 0xFF, 0x01 };
// | {'zone_idx': '00', 'battery_low': False, 'battery_level': None}

TEST(Parsers, 1060) {
	BatteryStatus* bat = (BatteryStatus*)payload_1060;

	EXPECT_EQ(bat->ZoneIndex(), 0);
	EXPECT_EQ(bat->BatteryOk(), 1);
	EXPECT_EQ(bat->Level()->HasValue(), 0);
}