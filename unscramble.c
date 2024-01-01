// On my honor:
    //
    // - I have not discussed the C language code in my program with
    // anyone other than my instructor or the teaching assistants
    // assigned to this course.
    //
    // - I have not used C language code obtained from another student,
    // the Internet, or any other unauthorized source, either modified
    // or unmodified.
    //
    // - If any C language code or documentation used in my program
    // was obtained from an authorized source, such as a text book or
    // course notes, that has been clearly noted with a proper citation
    // in the comments of my program.
    //
    // - I have not designed this program in such a way as to defeat or
    // interfere with the normal operation of the grading code.
    //
    // <Rishav Chakravarty>
    // <rishavc18@vt.edu>
#include "unscramble.h"
#include <stdlib.h>
#include <stdio.h>


// This function copies a block of bytes from one location in the buffer to another
void copy_block(char* buffer, uint16_t first_offset, uint16_t second_offset, uint16_t length) 
{
    for(uint16_t i = 0; i < length; i++) 
    {
        buffer[second_offset + i] = buffer[first_offset + i];
    }
}
// This function flips the nybbles (4-bit nibbles) within a byte
uint8_t flip_nybbles(uint8_t byte) {
    return ((byte & 0x0F) << 4) | ((byte & 0xF0) >> 4);
}

// This function blends bytes in the buffer using nybble flipping and XOR operations.
// The function iterates over each byte in the range of length. For each byte, it performs the following steps:
void blend_bytes(char* buffer, uint16_t length) 
{
    for(uint16_t i = 0; i < length; i++) 
    {
        char byte1 = buffer[i];
        // Reverse the nybbles in byte from the first offset
        char reversed_byte1 = ((byte1 & 0x0F) << 4) | ((byte1 & 0xF0) >> 4); 
        // XOR the reversed nybble with the byte from the second offset and store the result back in the second offset location
        buffer[i] = reversed_byte1 ^ buffer[i];
    }
}

// This function reads the data in clear text format and identifies words in the data
// The function iterates over each byte in the data buffer. It maintains a wordStart pointer that points to the start of a word. 
// When it encounters the start of a word, it sets the wordStart pointer to the current byte address.
void read_clear(uint8_t* data, size_t dataSize, FILE* output) {
    uint8_t* wordStart = NULL;
    for (size_t i = 0; i < dataSize; i++) {
        if ((wordStart == NULL) && ((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' && data[i] <= 'z'))) {
            wordStart = &data[i];
        } else if ((wordStart != NULL) && !((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' && data[i] <= 'z'))) {
            size_t wordLength = ((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' && data[i] <= 'z')) ? &data[i] - wordStart + 1 : &data[i] - wordStart;
            fprintf(output, "%-20.*s @ Offset: %02zx\n", (int)wordLength, (char*)wordStart, wordStart - data);
            wordStart = NULL;
        }
    }
}

// This function reads the data in fuzzy format from a file and performs the necessary unfuzzing steps to extract words and their offsets.
// currently doesnt work
void read_fuzzy(FILE* file, uint32_t size) 
{
    char* buffer = malloc(size * sizeof(char));
    uint32_t offset = 0;
    
    while (offset < size) 
    {
        uint8_t len;
        int n = fread(&len, sizeof(uint8_t), 1, file);
        if (n != 1) {
            printf("Failed to read length at offset %02x\n", offset);
            break;
        }
        offset += sizeof(uint8_t);
        
        len = flip_nybbles(len); // Unfuzz the length
        printf("Unfuzzed length: %d\n", len);

        n = fread(buffer, sizeof(char), len, file);
        if (n != len) {
            printf("Failed to read %d bytes at offset %02x\n", len, offset);
            break;
        }
        
        buffer[len] = '\0';
        
        printf("%s\t@ Offset: %02lx\n", buffer, offset - sizeof(uint8_t));
        
        uint16_t next_offset;
        n = fread(&next_offset, sizeof(uint16_t), 1, file);
    
        
        offset += len + sizeof(uint16_t); // update the offset

        uint16_t mask = (uint16_t)buffer[0] | ((uint16_t)buffer[0] << 8); // Create a two byte mask
        next_offset = next_offset ^ mask; // Unfuzz the next offset
        printf("Next offset: %04x\n", next_offset);
        
        int result = fseek(file, next_offset - offset, SEEK_CUR); // seek relatively to the current position
        if (result != 0) {
            printf("Failed to seek to offset %02x\n", next_offset);
            break;
        }
    }
    
    free(buffer);
}