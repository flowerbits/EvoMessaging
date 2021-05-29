#pragma once

#include <stdlib.h>
#include <string.h>

class Bitstream {
private:
    uint8_t* data; // where bitstream is contained
    int size;   // size of the bitstream
    int position;
public:
    // returns data as a byte array
    uint8_t* ToByteArray() {
        return data;
    }
    /**************************/
    bool GetBit(uint8_t x, int y) {
        return (x >> (7 - y)) & 1;
    }
    int SetBit(int x, int i, bool v) {
        if (v) return x | (1 << (7 - i));
        return x & ~(1 << (7 - i));
    }
    /**************************/

    /// <summary>
    /// Open an existing data array
    /// </summary>
    /// <param name="bytes">Raw buffer</param>
    /// <param name="length">Length of buffer</param>
    void OpenBytes(uint8_t* bytes, int length) {
        position = 0;
        data = bytes;
        size = length;
    }

    /// <summary>
    /// Allocates space for a new bitstream
    /// </summary>
    /// <param name="maxSize">Number of bytes for the buffer</param>
    void Open(int maxSize) {
        position = 0;
        data = (uint8_t*)malloc(maxSize);
        memset(data, 0, maxSize);
        size = maxSize;
    }

    void FreeData() {
        free(data);
    }
    
    /// <summary>
    /// Writes specified number of bits from provided data
    /// </summary>
    void Write(int bits, int dat) {

        int ind = position + bits-1;
        while (dat) {
            data[ind / 8] = SetBit(data[ind / 8], ind % 8, dat & 1);
            dat /= 2;
            ind--;
        }
        position += bits;
    }

    /// <summary>
    /// Reads a number of bits from the stream
    /// </summary>
    int Read(int bits) {
        int dat = 0;
        for (int i = position; i < position + bits; i++) {
            dat = dat * 2 + GetBit(data[i / 8], i % 8);
        }

        position += bits;
        return dat;
    }

    void Reset() {
        position = 0;
        memset(data, 0, size);
    }

    int GetPosition()
    {
        return position;
    }

    void SetPosition(int index)
    {
        position = index;
    }
};
