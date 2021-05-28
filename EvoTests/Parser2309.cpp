#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>
#include "MessageFactory.h"
#include <stdint.h>


uint8_t payload_2309[] = { 0x00, 0x0A, 0x28 };

TEST(Parsers, 2309) {
    EvoArray<BindFeature> array(bindPayload, sizeof(bindPayload));

    std::ostringstream result;
    MessagePrinter::PrintBindFeatureArray(result, &array);

    result.flush();
    EXPECT_TRUE(expectedResult == result.str());
}

TEST(Parsers, 2309_Create) {
    EvoAddress address;
    uint16_t features[] = { (uint16_t)Operations::ZoneSetpoint, (uint16_t)Operations::ZoneTemperature, (uint16_t)Operations::RelayDemand, (uint16_t)Operations::RfBind };
    address.data[0] = 0x58;
    address.data[1] = 0x9D;
    address.data[2] = 0xBF;

    EvoMessage* newMessage = MessageFactory::AdvertiseBinding(address, 0, features, 4);

    ASSERT_EQ(sizeof(bindPayload), *newMessage->payloadLength);
    for (int i = 0; i < *newMessage->payloadLength; i++)
    {
        EXPECT_EQ(newMessage->payload[i], bindPayload[i]);
    }
    newMessage->GenerateChecksum();
    EXPECT_EQ(newMessage->ChecksumValid(), true);
}
