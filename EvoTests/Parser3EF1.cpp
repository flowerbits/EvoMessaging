#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>

uint8_t payload_3EF1[] = { 0x00, 0x00, 0x00, 0x01, 0x58, 0x00, 0xFF };
//{'actuator_enabled': False, 'modulation_level': 0.0, 'actuator_countdown': 344, 'cycle_countdown': 0, '_unknown_0': 'FF'}

uint8_t payload_3EF1_2[] = { 0x00, 0x02, 0x92, 0x02, 0x56, 0x00, 0xFF };
//{'actuator_enabled': False, 'modulation_level' : 0.0, 'actuator_countdown' : 598, 'cycle_countdown' : 658, '_unknown_0' : 'FF'}

TEST(Parsers, 3EF1) {
    ActuatorCycle* cycle = (ActuatorCycle*)payload_3EF1;
    EXPECT_EQ(cycle->ActuatorCountdown(), 344);
    EXPECT_EQ(cycle->ModulationLevel()->Percentage(), 0.0);
    EXPECT_EQ(cycle->CycleCountdown(), 0);
    EXPECT_EQ(cycle->Flags(), 0xFF);

    cycle = (ActuatorCycle*)payload_3EF1_2;
    EXPECT_EQ(cycle->ActuatorCountdown(), 598);
    EXPECT_EQ(cycle->ModulationLevel()->Percentage(), 0.0);
    EXPECT_EQ(cycle->CycleCountdown(), 658);
    EXPECT_EQ(cycle->Flags(), 0xFF);
}
