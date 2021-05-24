#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>

uint8_t payload_3EF0[] = { 0x00, 0xC8, 0xFF };
//{'actuator_enabled': True, 'modulation_level' : 1.0, '_unknown_2' : [1, 1, 1, 1, 1, 1, 1, 1] }

TEST(Parsers, 3EF0) {
    ActuatorState* state = (ActuatorState*)payload_3EF0;

    EXPECT_EQ(state->IsEnabled(), true);
    EXPECT_EQ(state->ModulationLevel()->Percentage(), 1.0);
    EXPECT_EQ(state->Flags(), 0xFF);
}
