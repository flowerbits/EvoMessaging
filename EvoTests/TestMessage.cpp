#include "pch.h"
#include "EvoTypes.h"
#include "EvoMessage.h"
#include "string.h"
#include "EvoPacket.h"

using namespace std;

uint8_t testMessageData[] = { 0x53, 0xA9, 0x6A, 0xA5, 0x9A, 0x95, 0x65, 0x9A, 0x56, 0xA5, 0x9A, 0x95, 0x65, 0x9A, 0x56, 0xA5, 0x56, 0x55, 0xAA, 0xAA, 0xA5, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x65, 0x56, 0x35,
0x00, 0x55, 0x39, 0xBA, 0x68, 0xC6, 0x3F, 0xB3, 0x53, 0x2C, 0xDA, 0xF4, 0x5F, 0xF4, 0xBC, 0x76, 0x75, 0x53, 0xAA, 0x5A, 0x6A, 0x6A, 0x56, 0x6A, 0x9A, 0x99, 0xA5, 0x9A, 0xA5, 0x9A, 0x9A, 0x56, 0xA5, 0x56, 0x55,
0xAA, 0xAA, 0xA9, 0xAA, 0xAA, 0x99, 0x66, 0x35, 0x00 };

string expectedPackets [] = {
	"000  I --- 13:031566 --:------ 13:031566 3EF0 003 0000FF\r\n",
	"000 RQ --- 34:059461 13:013390 --:------ 3EF0 001 00\r\n" };

TEST(Messages, MessageParser) {
    char printBuffer[100];
    EvoPacket packet = EvoPacket();
    EvoMessage message;
    int packetCount = 0;
    string resultString;

    for (int i = 0; i < sizeof(testMessageData); i++)
    {
        auto result = packet.ProcessByte(testMessageData[i]);
        if (result == EvoPacket::ProcessResult::MessageComplete)
        {
            bool validMessage = packet.GetMessage(&message);
            EXPECT_EQ(validMessage, true);
            message.ToEvoString(printBuffer);
            packet.Reset();
            resultString = string(printBuffer);
            EXPECT_TRUE(resultString == expectedPackets[packetCount]);
            packetCount++;
        }
        else if (result >= EvoPacket::ProcessResult::Error)
        {
            packet.Reset();
        }
    }
    EXPECT_EQ(packetCount, 2);

}