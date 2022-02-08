#ifndef BITH
#define BITH
#include <stdint.h>

typedef struct bits {
    int size;
    int nextBit; /*place of bit in array */
    int nextByte;
    uint8_t *buff;
    int bytesRead;
} bits;

/*accepts size of bitstream buffer */
bits *createbitstream(int size);

/*sets bit in buffer and increments to next bit and byte if needed */
int setbitstream(bits *bitstream, int bit);

uint8_t getbyte(bits *bitstream);

uint32_t setinteger(uint8_t *bytes);

int getbitstream(bits *bitstream);

void resetBits(bits *bitstream);

#endif
