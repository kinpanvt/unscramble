#include "unscramble.h" // Include the custom header file 'unscramble.h'
#include <stdio.h> // Include standard input/output library for functions like fprintf
#include <stdint.h> // Include standard integer types library for fixed width integers like uint8_t

// Function to read a word from the data array, starting at a given offset
static void readWord(uint8_t *data, int offset, char *word, int wordLength) {
    for (int i = 0; i < wordLength; ++i) {
        word[i] = data[offset + i]; // Copy each character from data to word
    }
    word[wordLength] = '\0'; // Null-terminate the word
}

// Function to unscramble text that is considered 'clear' (not encrypted or obfuscated)
void unscrambleClearText(uint8_t *data, int size, FILE *outputFile) {
    // Check for valid inputs: data array, size of data, and output file pointer
    if (data == NULL || size <= 0 || outputFile == NULL) {
        return; // Return early if inputs are invalid
    }

    // Read the number of words and the offset of the first word record from the data
    int numWords = data[0] | (data[1] << 8); // Combine two bytes to form the number of words
    int offset = data[2] | (data[3] << 8); // Combine two bytes to get the starting offset

    // Iterate over each word
    for (int i = 0; i < numWords && offset < size; ++i) {
        int recordLength = data[offset]; // Length of the current record
        // Check if the record length is valid
        if (recordLength <= 0 || offset + recordLength > size) {
            break; // Break the loop if the record length is invalid
        }

        // Calculate the offset of the next word
        int nextWordOffset = data[offset + 1] | (data[offset + 2] << 8);
        if (nextWordOffset >= size) {
            break; // Break the loop if the next word offset is beyond the data size
        }

        // Create an array for the word, accounting for null terminator
        char word[recordLength - 3 + 1]; // -3 for length and offset bytes, +1 for null-terminator
        readWord(data, offset + 3, word, recordLength - 3); // Read the word from data

        // Write the word and its offset to the output file
        fprintf(outputFile, "%-20s @ Offset: %02x\n", word, offset + 3);

        offset = nextWordOffset; // Update the offset to the next word record
    }
}


// Function to flip the nybbles (4-bit parts) of a byte
static uint8_t flipNybbles(uint8_t byte) {
    return ((byte & 0x0F) << 4) | ((byte & 0xF0) >> 4); // Flip the lower and upper nybbles
}

// Function to unscramble text that is considered 'fuzzy' (slightly obfuscated)
void unscrambleFuzzyText(uint8_t *data, int size, FILE *outputFile) {
    // Check for valid inputs: data array, size of data, and output file pointer
    if (data == NULL || size <= 0 || outputFile == NULL) {
        fprintf(stderr, "Invalid input to unscrambleFuzzyText function.\n"); // Print error message
        return; // Return early if inputs are invalid
    }

    // Read the number of words from the data
    int numWords = data[0] | (data[1] << 8); // Combine two bytes to form the number of words
    int offset = 2; // Set the starting offset after numWords

    // Iterate over each word
    for (int i = 0; i < numWords && offset + 4 < size; ++i) {
        int recordLength = flipNybbles(data[offset]); // Flip nybbles of the record length byte
        // Check if the record length is valid
        if (offset + recordLength >= size || recordLength < 4) {
            break; // Break the loop if the record length is invalid
        }

        // Read the first letter of the word and create a mask
        uint8_t firstLetter = data[offset + 3]; // Assuming the first letter is the fourth byte in the record
        uint16_t mask = (uint16_t)(firstLetter | (firstLetter << 8));
        // Calculate the next word offset using a bitwise XOR with the mask
        uint16_t maskedOffset = (data[offset + 1] << 8) | data[offset + 2];
        uint16_t nextWordOffset = maskedOffset ^ mask;

        // Check if the next word offset is valid
        if (nextWordOffset >= size || nextWordOffset <= offset || nextWordOffset >= offset + recordLength) {
            break; // Break the loop if the next word offset is invalid
        }

        // Create an array for the word, accounting for null terminator
        int wordLength = recordLength - 4; // Subtract bytes for record length, next word offset, and first letter
        char word[wordLength + 1];

        // Read and unscramble each character of the word
        for (int j = 0; j < wordLength; ++j) {
            word[j] = data[offset + 4 + j] ^ firstLetter; // XOR each character with the first letter
        }
        word[wordLength] = '\0'; // Null-terminate the word

        // Write the word and its offset to the output file
        fprintf(outputFile, "%-20s @ Offset: %04x\n", word, offset + 4);

        offset = nextWordOffset; // Update the offset to the next word record
    }
}
