#include <stdio.h>                                                              
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include "bit.h"

/*SETBIT WORKS AS FOLLOWS:
    *bitPlace is place of bit you want to set
    *byte is a byte represented by char data type
    *receives a byte who's bits are cleared out */

/*
typedef struct bits {
    int size;
    int nextBit; *place of bit in array *
    int nextByte;
    uint8_t *buff;
} bits; */

/*creates a new bitStream to write of given size */
bits *createbitstream(int size){
    bits *temp = (bits *)malloc(sizeof(bits));
    uint8_t *arr = calloc(size, sizeof(uint8_t));
    temp->buff = arr;
    temp->size = size;
    temp->nextBit = 7; /*starts at most significant bit*/
    temp->nextByte = 0; /*index int the buff arr*/
    temp->bytesRead = 0;
    return temp;
}

/*FUNCTIONALITY OF setbitstream()
    *ACCEPTS A BIT (0 or 1) to set
    *ACCEPTS A BITSTREAM BITS STRUCT
    *FUNCTION WILL SET BIT, INCREMENT BITPLACE UNTIL FINAL BIT
    AND THEN PROCEED TO THE NEXT BYTE IN THE BUFFER
    *returns 1 if buffer is full and needs to be written
*/

/*What I need to consider 
    *what if buffer doesn't fill
    *nextByte will hold number of bytes to write to file
    *if we finish reading codes and nextBit != 0 then we need to pad byte
*/

/*FUNCTIONALITY
    *EXTRACTS kth bit BIT, AND KEEPS TRACK OF CURRENT BITPLACE
    AND BYTEPLACE
    *returns -1 if full buffer
        &&&DOESN'T WORK AND I NEED TO ADJUST BECAUSE BIT LOST
    *YOU NEED TO SET bitstream->nextByte to the BEGINNING OF THE BUFFER
*/
int getbitstream(bits *bitstream){
    
    if(bitstream->nextByte == bitstream->size){
        bitstream->nextByte = 0; /*once all bits full, go to beginning*/
        bitstream->nextBit = 7;
        return -1; /*return -1 if full buffer*/
    }

    int tempByte = bitstream->buff[bitstream->nextByte];
    int bit = (tempByte
                        & (1<<(bitstream->nextBit))) >> bitstream->nextBit;
    
    bitstream->nextBit--;
    if(bitstream->nextBit < 0){ 
        bitstream->nextBit = 7;
        if(bitstream->nextByte < bitstream->size){
            bitstream->nextByte++;
            bitstream->bytesRead++;
        }
    }

    return bit;
    


}

void resetBits(bits *bitstream){
     int i = 0;
     for(i=0; i < bitstream->size; i++){
         bitstream->buff[i]=bitstream->buff[i] & 0;
      }
 }     

int setbitstream(bits *bitstream, int bit){
    /*set the bit of the current byte starting from 0 to 7*/
    bitstream->buff[bitstream->nextByte] = bitstream->buff[bitstream->nextByte]
                                            | (bit << bitstream->nextBit);
    bitstream->nextBit--; /*increment bit after setting it */

    /*once final bit reached, proceed to next byte and reset bitplace to 0 */
    if(bitstream->nextBit < 0){ 
        bitstream->nextBit = 7;
        if(bitstream->nextByte < bitstream->size){
            bitstream->nextByte++;
            bitstream->bytesRead++;
        }
        
        if(bitstream->nextByte == bitstream->size){
            bitstream->nextByte = 0; /*once all bits full, go to beginning*/
            bitstream->nextBit = 7;
            return 1; /*return true on full buffer */
        }
            /*fprintf(stderr, "setbitstream overflow of bytes! \n"); */
    }
    
    return 0;
}

/*FUNCTIONALITY:
    *Takes a bit* bitstream and returns a byte, incrementing to the next
    byte
    *NECESSARY THAT YOU CHECK bitstream->nextByte == bitstream->size
*/
uint8_t getbyte(bits *bitstream){
    uint8_t byte = bitstream->buff[bitstream->nextByte];
    bitstream->nextByte++;
    bitstream->bytesRead++;
    return byte;
}


/*FUNCTIONALITY*
    *TAKES 4 BYTES AND CREATES AN INTEGER
*/
uint32_t setinteger(uint8_t *bytes){
   /*integer = *integer | (1 << bitPlace); will set the bit at bitPlace to 1 */
   uint32_t integer = bytes[3] | ( (uint32_t)bytes[2] << 8 ) 
   | ( (uint32_t)bytes[1] << 16 ) | ( (uint32_t)bytes[0] << 24 );
    
   return integer;
}


#ifdef DEBUG
int main(void){
    /*unsigned char byte = byte & (~1); *to set a byte to all zeroes *
    assert(byte == NULL);
    setbit(4, &byte);
    setbit(5, &byte); */
    /*printf("%c", byte);*/
    /*set 'h' */
    bits *bitstream = createbitstream(6);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    assert(bitstream->buff[0] == 'h');

    /*set e */
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    assert(bitstream->buff[1] =='e');

    /*set l */
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    assert(bitstream->buff[2] =='l');

    /*set l */
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    assert(bitstream->buff[3] =='l');

    /*set o */
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    setbitstream(bitstream, 1);
    assert(bitstream->buff[4] =='o');

    /*set NULL */
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    setbitstream(bitstream, 0);
    
    if(setbitstream(bitstream, 0) == 1)
        printf("%s", bitstream->buff);
    assert(bitstream->buff[5] == '\0');
    assert(bitstream->nextByte == 0);
    assert(bitstream->nextBit == 7);
    assert(bitstream->bytesRead == 6);
    
    /*TEST getbyte*/
    uint8_t byte;
    byte = getbyte(bitstream);
    assert(byte == 'h');
    
    byte = getbyte(bitstream);
    assert(byte == 'e');

    byte = getbyte(bitstream);
    assert(byte == 'l');

    byte = getbyte(bitstream);
    assert(byte == 'l');

    byte = getbyte(bitstream);
    assert(byte == 'o');

    byte = getbyte(bitstream);
    assert(byte == '\0');

    assert(bitstream->nextByte == bitstream->size);

    /*TEST setInteger*/
    uint8_t arr[4];
    unsigned int ret;
    arr[0] = 128;
    arr[1] = 0;
    arr[2] = 0;
    arr[3] = 2;
    ret = setinteger(arr);
    printf("%d", ret);

    
    /*test getbitstream */
    bitstream->nextByte = 0;
    int bit = getbitstream(bitstream);
    assert(bit == 0);
    bit = getbitstream(bitstream);
    assert(bit == 1);

    bit = getbitstream(bitstream);
    assert(bit == 1);
    
    bit = getbitstream(bitstream);
    assert(bit == 0);

    bit = getbitstream(bitstream);
    assert(bit == 1);

    bit = getbitstream(bitstream);
    assert(bit == 0);


    bit = getbitstream(bitstream);
    assert(bit == 0);

    
    bit = getbitstream(bitstream);
    assert(bit == 0);
    assert(bitstream->nextByte = 1);
    assert(bitstream->nextBit = 7);


    return 0;
}
#endif
