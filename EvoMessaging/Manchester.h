#pragma once
#include <stdint.h>

static uint8_t const man_encode[16] = {
  0xAA, 0xA9, 0xA6, 0xA5,  0x9A, 0x99, 0x96, 0x95,
  0x6A, 0x69, 0x66, 0x65,  0x5A, 0x59, 0x56, 0x55
};
//#define MAN_ENCODE(_i) pgm_read_byte( man_encode+(_i) )
#define MAN_ENCODE(_i) (man_encode[_i]) 

// Convert little-endian 4 bits to 2-bit big endian
static uint8_t const man_decode[16] = {
  0xF, 0xF, 0xF, 0xF, 0xF, 0x3, 0x2, 0xF,
  0xF, 0x1, 0x0, 0xF, 0xF, 0xF, 0xF, 0xF
};

#define MAN_DECODE(_i) ( man_decode[_i] )

static inline int manchester_code_valid(uint8_t code) {
    return (MAN_DECODE((code >> 4) & 0xF) != 0xF) && (MAN_DECODE((code) & 0xF) != 0xF);
}

static inline uint8_t manchester_decode(uint8_t byte) {
    uint8_t decoded;

    decoded = MAN_DECODE((byte) & 0xF);
    decoded |= MAN_DECODE((byte >> 4) & 0xF) << 2;

    return decoded;
}

static inline uint8_t manchester_encode(uint8_t value) {
    return MAN_ENCODE(value & 0xF);
}