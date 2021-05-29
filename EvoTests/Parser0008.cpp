#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>
#include "SpiFramer.h"
#include "MessageFactory.h"
#include "TestHelper.h"


uint8_t payload_0008[2][3] = { {0x00, 0xC8 }, {0x00, 0x00} };

TEST(Parsers, 0008) {
    RelayDemand* demand = (RelayDemand* )payload_0008[0];
    EXPECT_EQ(demand->Demand()->HasValue(), true);
    EXPECT_EQ(demand->Demand()->Percentage(), 1);
    EXPECT_EQ(demand->ZoneIndex(), 0);
    
    demand = (RelayDemand*)payload_0008[1];
    EXPECT_EQ(demand->Demand()->HasValue(), true);
    EXPECT_EQ(demand->ZoneIndex(), 0);
    EXPECT_EQ(demand->Demand()->Percentage(), 0);
}


uint8_t framed_1008[]{
    0x65,0x52,0xB2,0xB5,0x33,0x2B,0x52,0xD4,0xD5,0x4D,0x55,0x53,0x32,0xB5,0x2D,0x4D,
    0x54,0xD5,0x54,0xAB,0x2A,0xCA,0xB2,0xB4,0xAB,0x32,0xCA,0xB2,0xAC,0xB3,0x32,0xD4,
    0xB2,0xCD,0x59 };
//This is the unframed message

uint8_t unframed_1008[] = {
    /*0x53,*/0xA9,0x6A,0x99,0x6A,0x69,0x59,0x65,0x55,0x99,0x6A,0x69,0x59,0x65,0x55,0xAA,
    0xAA,0xAA,0x6A,0xAA,0xA6,0xAA,0xAA,0x9A,0xA6,0xA5,0x9A/*,0x35*/ };

//Expected output string 000  I-- - 22:040383 --:------ 22:040383 0008 002 0042

TEST(CreateMessage, 0008) {
    BitRing outputRing;
    uint8_t buffer[250];
    outputRing.OpenBytes(buffer, 250);
    EvoAddress sender;
    sender.SetValues(22, 40383);
    EvoMessage* message = MessageFactory::AdvertiseRelayDemand(sender, 0, .33);
    EvoMessage expected;
    expected.LoadFromManchester(unframed_1008, sizeof(unframed_1008));

    for (int i = 0; i < message->rawLength; i++)
        EXPECT_EQ(message->rawData[i], expected.rawData[i]);

    SpiFramer::FrameMessageForTransmission(message, &outputRing);
    int byteCount = outputRing.GetBitCount() / 8;
    uint8_t* framedData = new uint8_t[byteCount];

    for (int i = 0; i < byteCount; i++)
        framedData[i] = outputRing.Read(8);
    
    EvoMessage * testMessage = TestHelper::GetMessageFromFramedData(framedData, byteCount);
    EXPECT_TRUE(testMessage != NULL);

    for (int i = 0; i < testMessage->rawLength; i++)
        EXPECT_EQ(testMessage->rawData[i], expected.rawData[i]);

}