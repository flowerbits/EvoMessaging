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
extern "C" {

#include "frame.h"
#include "message.h"
}

uint8_t spiSample[] = {
    0x65, 0x52, 0xB2, 0xB5, 0x4B, 0x2C, 0xD5, 0x35, 0x34, 0xB3, 0x35, 0x54, 0xB2, 0xCD, 0x53, 0x53, 0x4B, 0x33, 0x55, 0x4B, 0x35, 0x55, 0x52, 0xAC, 0xAB, 0x52, 0xCA, 0xB2, 0xAC, 0xAB, 0x2A, 0xD5, 0x55, 0x55, 0x4D, 0x35, 0x55, 0x95, 0x55, 0x73, 0x8A, 0xEC, 0x2C, 0x31, 0xFF, 0x96, 0x6F, 0x94, 0x1A, 0x6B, 0x71, 0x7B, 0xF5, 0x17, 0x87, 0xBB, 0x71, 0x5C,// 000  I-- - 13:031566 -- : ------ 13 : 031566 3EF0 003 0000FF
    0x65, 0x4A, 0xB2, 0xD4, 0xAD, 0x2B, 0x4D, 0x52, 0xB4, 0xB3, 0x4C, 0xD4, 0xB2, 0xCD, 0x4B, 0x2C, 0xCB, 0x33, 0x55, 0x4B, 0x35, 0x55, 0x52, 0xAC, 0xAB, 0x4A, 0xCA, 0xB2, 0xAD, 0x33, 0x33, 0x55, 0x95, 0x55, 0x5D, 0xC8, 0xA0, 0x8D, 0x54, 0x1C, 0x45, 0xB1, 0xF5, 0x34, 0xEC, 0x2F, 0x4F, 0x44, 0x03, 0x41, 0x3D, 0xE9, 0x94, 0x89, 0x67, 0xCE, 0x6E, 0x14,// 000 RQ-- - 34 : 059461 13:013390 --:------ 3EF0 001 00
};

uint8_t packetSample[] = {
    0xFF, 0x00, 0x33, 0x55, 0x53, 0xA9, 0x6A, 0xA5, 0x9A, 0x95, 0x65, 0x9A, 0x56, 0xA5, 0x9A, 0x95, 0x65, 0x9A, 0x56, 0xA5, 0x56, 0x55, 0xAA, 0xAA, 0xA5, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x65, 0x56, 0x35,
    0xFF, 0x00, 0x33, 0x55, 0x53, 0xAA, 0x5A, 0x6A, 0x6A, 0x56, 0x6A, 0x9A, 0x99, 0xA5, 0x9A, 0xA5, 0x9A, 0x9A, 0x56, 0xA5, 0x56, 0x55, 0xAA, 0xAA, 0xA9, 0xAA, 0xAA, 0x99, 0x66, 0x35,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
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

void TestPacketSample()
{
    printf("Test packet sample\n:");
    char printBuffer[255];

    EvoPacket packet = EvoPacket();
    EvoMessage message;

    for (int i = 0; i < sizeof(packetSample); i++)
    {
        auto result = packet.ProcessByte(packetSample[i]);
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

void TestFramedData()
{
    printf("Bulk framing results:\n");
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
    printf("Byte framing results:\n");
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
            printf("0x%02X, ", temp);
            if (result >= EvoPacket::ProcessResult::CollisionError)
            {
                packet.Reset();
                continue;
            }

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
                br.Reset();
            }
        }
    }

}

void shittyLoop()
{
    printf("EvoFW Parse results:\n");
    frame_init();
    msg_init();

    static struct message* rx = NULL;
    static struct message* tx = NULL;
    uint8_t byte;
    static char msgBuff[255];
    static uint8_t nRx = 0;

    for (int i = 0; i < sizeof( packetSample); i++)
    {

        frame_work();
        msg_work();
        frame_rx_byte(packetSample[i]);
        // Print RX messages
        if (rx) {
            // Do we still have outstanding text to send?
            if (nRx) {
                nRx -= printf(msgBuff, nRx);
            }

            if (!nRx) {
                nRx = msg_print(rx, msgBuff);
                if (!nRx)
                    msg_free(&rx);
            }
        }
        else {    // If we get a message now we'll start printing it next time
            rx = msg_rx_get();
        }
    }
}

#pragma warning (disable : 4146)

void printTemp(uint16_t temp) {
    int result = temp;
    if (temp >= 0x8000)
    {
        result = temp - 0x10000;
    }

    printf("%04X becomes %d\n",temp, result);
}
int main() 
{
    uint8_t data[] = { 0x07, 0xDC, 0XFD, 0xDC, 0x08, 0xCe };

    uint16_t temp1 = (data[0] << 8) | data[1] ;
    uint16_t temp2 = (data[2] << 8)  | data[3];
    uint16_t temp3 = (data[4] << 8)| data[5];

    printTemp(temp1);
    printTemp(temp2);
    printTemp(temp3);

    uint16_t tt1 = -uintmax_t(temp1);
    uint16_t tt2 = -uintmax_t(temp2);

    printf("%04X %04X\n", tt1, tt2);
    //shittyLoop();
    //TestPacketSample();
    //TestByteFraming();
    
}
