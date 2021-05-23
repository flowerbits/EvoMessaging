// Encoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Bitstream.h"
#include <stdint.h>
#include <iomanip>
#include "Manchester.h"
#include "EvoPacket.h"
#include "BitTools.h"
#include "SpiFramer.h"
#include "BitRing.h"

uint8_t spiSample[] = {
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x5f, 0xf0, 0x05, 0x99, 0x55, 0x59, 0x54, 0xac, 0xb5, 0x2b, 0x52, 0xd2, 0xd5, 0x4b, 0x4b, 0x52, 0xb5, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x54, 0xd5, 0x4a, 0xca, 0xb3, 0x54, 0xab, 0x32, 0xcd, 0x32, 0xac, 0xab, 0x2a, 0xca, 0xb2, 0xad, 0x2b, 0x2d, 0x4a, 0xd5, 0x2c, 0xad, 0x2a, 0xd5, 0x52, 0xad, 0x2b, 0x2a,
    0xca, 0xb5, 0x55, 0x2b, 0x55, 0x55, 0x54, 0xad, 0x4d, 0x2a, 0xd4, 0xd2, 0xad, 0x53, 0x35, 0x4b, 0x32, 0xad, 0x4b, 0x2a, 0xd3, 0x32, 0xad, 0x53, 0x35, 0x52, 0xb4, 0xcc,
    0xb3, 0x52, 0xca, 0xd5, 0x2d, 0x53, 0x4c, 0xcc, 0xb2, 0xcc, 0xd3, 0x52, 0xcc, 0xb5, 0x2c, 0xab, 0x52, 0xcc, 0xb5, 0x2d, 0x33, 0x2a, 0xca, 0xb2, 0xac, 0xab, 0x2a, 0xca,
    0xb2, 0xac, 0xab, 0x2a, 0xca, 0xb2, 0xac, 0xab, 0x2a, 0xca, 0xb2, 0xac, 0xab, 0x2a, 0xca, 0xb2, 0xac, 0xab, 0x2a, 0xca, 0xb2, 0xd5, 0x55, 0x56, 0x55, 0x55 };

uint8_t packetSample[] = {
    0xa9, 0x6a, 0x6a, 0x6a, 0x56, 0x6a, 0x9a, 0x99, 0x6a, 0x6a, 0x56, 0x6a, 0x9a, 0x99, 0xaa, 0x9a, 0xa6, 0x55, 0xaa, 0x6a, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x99, 0x5a, 0xaa, 0x99, 0x5a, 0xa6, 0xaa, 0x96, 0x5d, 0xa1
};


void printArray(uint8_t arr[], int length)
{
    std::cout << "\n";
    for (int i = 0; i < length; i++)
    {
        std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << +arr[i] << " ";
        if (i > 0 && i % 16 == 0)
            std::cout << "\n";
    }
    std::cout << "\n";
}

void TestFramedData()
{
    char printBuffer[255];

    int encodedSize = sizeof(spiSample);
    uint8_t unframedData[sizeof(spiSample)] = {};
    SpiFramer::UnframeData(spiSample, sizeof(spiSample), unframedData, sizeof(unframedData));

    printArray(unframedData, sizeof(unframedData));

    EvoPacket packet = EvoPacket();
    EvoMessage message;

    for (int i = 0; i < encodedSize; i++)
    {
        auto result = packet.ProcessByte(unframedData[i]);
        if (result == EvoPacket::ProcessResult::MessageComplete)
        {
            bool validMessage = packet.GetMessage(&message);
            if (!validMessage)
            {
                printf("Bad message received.\n");
                continue;
            }
            printf("We have a packet:\n");

            message.ToEvoString(printBuffer);
            printf(printBuffer);
            packet.Reset();
        }
    }

    EvoMessage message2 = EvoMessage();
    message2.LoadFromManchester(packetSample, sizeof(packetSample));
}

void TestByteFraming() 
{
    char printBuffer[255];

    int encodedSize = sizeof(spiSample);
    BitRing br;
    br.Open(encodedSize);
    EvoPacket packet = EvoPacket();
    EvoMessage message;

    for (int i = 0; i < encodedSize; i++)
    {
        br.Write(8, spiSample[i]);

        if (br.GetBitCount() > 10)
        {
            uint16_t temp = br.Read(10);
            temp = reverseBits(temp >> 1);
            auto result = packet.ProcessByte(temp & 0xFF);
            if (result == EvoPacket::ProcessResult::MessageComplete)
            {
                bool validMessage = packet.GetMessage(&message);
                if (!validMessage)
                {
                    printf("Bad message received.\n");
                    continue;
                }
                printf("We have a packet:\n");

                message.ToEvoString(printBuffer);
                printf(printBuffer);
                packet.Reset();
            }
        }
    }

}
int main() 
{
    TestByteFraming();
    
}
