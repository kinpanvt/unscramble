#ifndef UNSCRAMBLE_H // Check if UNSCRAMBLE_H is not defined
#define UNSCRAMBLE_H // Define UNSCRAMBLE_H to prevent multiple inclusions of this header

#include <stdint.h> // Include standard integer types for fixed-width integers like uint8_t
#include <stdio.h>  // Include standard I/O library for the FILE type used in function declarations

// Function declaration for unscrambling clear text
void unscrambleClearText(uint8_t *data, int size, FILE *outputFile);
// This line declares a function named unscrambleClearText. It takes three parameters: a pointer to uint8_t (representing an array of data), an integer representing the size of this data, and a pointer to a FILE object for the output file.

// Function declaration for unscrambling fuzzy text
void unscrambleFuzzyText(uint8_t *data, int size, FILE *outputFile);
// Similarly, this line declares a function named unscrambleFuzzyText. It has the same parameters as unscrambleClearText and is used for a different type of text unscrambling.

#endif // UNSCRAMBLE_H // End of the preprocessor conditional
// This line marks the end of the conditional compilation block. It ensures that the contents of this header file are only included once, even if the header file is included multiple times in the program.
