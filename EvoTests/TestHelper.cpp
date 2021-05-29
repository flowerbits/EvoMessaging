#include "pch.h"
#include "TestHelper.h"
#include <stdint.h>
#include "SpiFramer.h"
#include "EvoPacket.h"
#include "EvoMessage.h"

EvoMessage* TestHelper::GetMessageFromFramedData(uint8_t* framedData, int length)
{
    uint8_t *unframedData = new uint8_t[length];

    SpiFramer::UnframeData(framedData, length, unframedData, length);

    EvoPacket packet = EvoPacket();
    EvoMessage* message = new EvoMessage();

    for (int i = 0; i < length; i++)
    {
        auto result = packet.ProcessByte(unframedData[i]);
        if (result == EvoPacket::ProcessResult::MessageComplete)
        {
            bool validMessage = packet.GetMessage(message);
            if (!validMessage)
            {
                return message;
                packet.Reset();
                continue;
            }

            return message;
            packet.Reset();
        }
        else if (result >= EvoPacket::ProcessResult::Error)
            packet.Reset();

    }

    return NULL;
}