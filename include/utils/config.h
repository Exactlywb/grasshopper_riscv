#ifndef _GRASSHOPPER_CONFIG_H__
#define _GRASSHOPPER_CONFIG_H__

#include <stddef.h>

#ifndef _EXIT_CODE
#define _EXIT_CODE 255
#endif

#define BLOCK_SIZE 16
#define NUM_ROUNDS 10

#define BIT_IN_BYTES 8

void *allocate_with_padding(size_t, size_t, size_t *);

static unsigned char InvS[0x100] = {
    0xA5, 0x2D, 0x32, 0x8F, 0x0E, 0x30, 0x38, 0xC0, // 00..07
    0x54, 0xE6, 0x9E, 0x39, 0x55, 0x7E, 0x52, 0x91, // 08..0F
    0x64, 0x03, 0x57, 0x5A, 0x1C, 0x60, 0x07, 0x18, // 10..17
    0x21, 0x72, 0xA8, 0xD1, 0x29, 0xC6, 0xA4, 0x3F, // 18..1F
    0xE0, 0x27, 0x8D, 0x0C, 0x82, 0xEA, 0xAE, 0xB4, // 20..27
    0x9A, 0x63, 0x49, 0xE5, 0x42, 0xE4, 0x15, 0xB7, // 28..2F
    0xC8, 0x06, 0x70, 0x9D, 0x41, 0x75, 0x19, 0xC9, // 30..37
    0xAA, 0xFC, 0x4D, 0xBF, 0x2A, 0x73, 0x84, 0xD5, // 38..3F
    0xC3, 0xAF, 0x2B, 0x86, 0xA7, 0xB1, 0xB2, 0x5B, // 40..47
    0x46, 0xD3, 0x9F, 0xFD, 0xD4, 0x0F, 0x9C, 0x2F, // 48..4F
    0x9B, 0x43, 0xEF, 0xD9, 0x79, 0xB6, 0x53, 0x7F, // 50..57
    0xC1, 0xF0, 0x23, 0xE7, 0x25, 0x5E, 0xB5, 0x1E, // 58..5F
    0xA2, 0xDF, 0xA6, 0xFE, 0xAC, 0x22, 0xF9, 0xE2, // 60..67
    0x4A, 0xBC, 0x35, 0xCA, 0xEE, 0x78, 0x05, 0x6B, // 68..6F
    0x51, 0xE1, 0x59, 0xA3, 0xF2, 0x71, 0x56, 0x11, // 70..77
    0x6A, 0x89, 0x94, 0x65, 0x8C, 0xBB, 0x77, 0x3C, // 78..7F
    0x7B, 0x28, 0xAB, 0xD2, 0x31, 0xDE, 0xC4, 0x5F, // 80..87
    0xCC, 0xCF, 0x76, 0x2C, 0xB8, 0xD8, 0x2E, 0x36, // 88..8F
    0xDB, 0x69, 0xB3, 0x14, 0x95, 0xBE, 0x62, 0xA1, // 90..97
    0x3B, 0x16, 0x66, 0xE9, 0x5C, 0x6C, 0x6D, 0xAD, // 98..9F
    0x37, 0x61, 0x4B, 0xB9, 0xE3, 0xBA, 0xF1, 0xA0, // A0..A7
    0x85, 0x83, 0xDA, 0x47, 0xC5, 0xB0, 0x33, 0xFA, // A8..AF
    0x96, 0x6F, 0x6E, 0xC2, 0xF6, 0x50, 0xFF, 0x5D, // B0..B7
    0xA9, 0x8E, 0x17, 0x1B, 0x97, 0x7D, 0xEC, 0x58, // B8..BF
    0xF7, 0x1F, 0xFB, 0x7C, 0x09, 0x0D, 0x7A, 0x67, // C0..C7
    0x45, 0x87, 0xDC, 0xE8, 0x4F, 0x1D, 0x4E, 0x04, // C8..CF
    0xEB, 0xF8, 0xF3, 0x3E, 0x3D, 0xBD, 0x8A, 0x88, // D0..D7
    0xDD, 0xCD, 0x0B, 0x13, 0x98, 0x02, 0x93, 0x80, // D8..DF
    0x90, 0xD0, 0x24, 0x34, 0xCB, 0xED, 0xF4, 0xCE, // E0..E7
    0x99, 0x10, 0x44, 0x40, 0x92, 0x3A, 0x01, 0x26, // E8..EF
    0x12, 0x1A, 0x48, 0x68, 0xF5, 0x81, 0x8B, 0xC7, // F0..F7
    0xD6, 0x20, 0x0A, 0x08, 0x00, 0x4C, 0xD7, 0x74  // F8..FF
};

#endif