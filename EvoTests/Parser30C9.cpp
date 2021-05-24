#include "pch.h"
#include "EvoTypes.h"

uint8_t payload_30C9[] = { 0x00, 0x08, 0xA2 };
// {'zone_idx': '00', 'temperature': 22.1}

TEST(Parsers, 30C9) {
	EvoArray<ZoneTemperature> temperatures(payload_30C9, sizeof(payload_30C9));

	EXPECT_EQ(temperatures.ValueCount(), 1);
	EXPECT_EQ(temperatures.Values()[0].ZoneIndex(), 0);
	EXPECT_EQ(temperatures.Values()[0].Temperature()->Celcius(), 2210);
}