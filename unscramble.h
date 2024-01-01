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
#ifndef UNSCRAMBLE_H
#define UNSCRAMBLE_H

#include <stdint.h>
#include <stddef.h>
#include "word.h"

uint8_t flip_nybbles(uint8_t byte);
void copy_block(char* buffer, uint16_t first_offset, uint16_t second_offset, uint16_t length);
void blend_bytes(char* buffer, uint16_t length);
uint16_t create_mask(uint8_t byte);

void read_clear(uint8_t *data, size_t dataSize, FILE* output);
void read_fuzzy(FILE* file, uint32_t size);

#endif