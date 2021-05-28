#include <stdlib.h>
#include <string.h>
#include <stdint.h>

class BitRing {
private:
    uint8_t* data;
    int bufferSize;
    int readPosition;
    int writePosition;
public:

    bool GetBit(uint8_t x, int y) {
        return (x >> (7 - y)) & 1;
    }

    int SetBit(int x, int i, bool v) {
        if (v) return x | (1 << (7 - i));
        return x & ~(1 << (7 - i));
    }

    /// <summary>
    /// Allocates space for a new bitstream
    /// </summary>
    /// <param name="maxSize">Number of bytes for the buffer</param>
    void Open(int maxSize) {
        OpenBytes((uint8_t*)malloc(maxSize), maxSize);
    }

    void OpenBytes(uint8_t* buffer, int maxSize)
    {
        bufferSize = maxSize * 8;
        readPosition = 0;
        writePosition = 0;
        data = buffer;
        memset(buffer, 0, maxSize);
    }

    void Reset()
    {
      readPosition = 0;
      writePosition = 0;
      memset(data, 0, bufferSize/8);
    }

    void FreeData() {
        free(data);
    }

    /// <summary>
    /// Writes specified number of bits to the ring buffer
    /// </summary>
    void Write(int bits, int dat) {
        int ind = writePosition + bits - 1;

        if (ind >= bufferSize)
        {
            ind = bufferSize - 1;
            //Write the last bits to beginning of the stream first.
            int ind2 = bufferSize - ind;
            while (ind2>=0) {
                data[ind2 / 8] = SetBit(data[ind2 / 8], ind2 % 8, dat & 1);
                dat /= 2;
                ind2--;
            }
        }
        while (ind >= writePosition) {
            data[ind / 8] = SetBit(data[ind / 8], ind % 8, dat & 1);
            dat /= 2;
            ind--;

            
        }
        writePosition += bits;
        writePosition %= bufferSize;
    }

    /// <summary>
    /// Reads a number of bits from the ring buffer
    /// </summary>
    int Read(int bits) {
        int dat = 0;
        int end = readPosition + bits;
        int remainder = end - bufferSize;
        
        if (remainder > 0) {

            for (int i = readPosition; i < bufferSize; i++) {
                dat = dat * 2 + GetBit(data[i / 8], i % 8);
            }

            end = remainder;
            readPosition = 0;
            bits -= remainder;
        }

        for (int i = readPosition; i < end; i++) {
            dat = dat * 2 + GetBit(data[i / 8], i % 8);
        }

        readPosition += bits;
        readPosition %= bufferSize;
        return dat;
    }

    int GetBitCount() {
        int retValue = writePosition - readPosition;
        if (retValue < 0)
            retValue += bufferSize;
        return retValue;
    }
};
