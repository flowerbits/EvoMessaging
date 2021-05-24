#include "pch.h"
#include "EvoTypes.h"
#include "MessagePrinter.h"
#include <iostream>
#include <string>

//Test parsing the 1FC9 packet type which is a bind request and has an array of features.

uint8_t bindPayload[] = { 0x00, 0x23, 0x09, 0x58, 0x9D, 0xBF, 0x00, 0x30, 0xC9, 0x58, 0x9D, 0xBF, 0x00, 0x00, 0x08, 0x58, 0x9D, 0xBF, 0x00, 0x1F, 0xC9, 0x58, 0x9D, 0xBF };
std::string expectedResult = "[['00','2309','22:040383'],['00','30C9','22:040383'],['00','0008','22:040383'],['00','1FC9','22:040383']]";

TEST(Parsers, 1FC9) {
    EvoArray<BindFeature> array(bindPayload, sizeof(bindPayload));

    std::ostringstream result;
    MessagePrinter::PrintBindFeatureArray(result, &array);

    result.flush();
    EXPECT_TRUE(expectedResult == result.str());
}
