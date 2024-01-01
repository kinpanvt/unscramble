#ifndef WORD_H // Check if WORD_H is not defined
#define WORD_H // Define WORD_H to prevent multiple inclusions of this header file

#include <assert.h> // Include assert for diagnostic tests
#include <stdlib.h> // Include standard library for general utilities like memory allocation
#include <stdint.h> // Include standard integer types for fixed-width integers like uint16_t
#include <stdio.h>  // Include standard I/O library for FILE*

// Definition of the Word structure
struct Word {
    char* word;       // Pointer to a dynamically allocated string
    uint16_t offset;  // An unsigned short integer to store the offset
};

// Definition of the Word_List structure
struct Word_List {
    struct Word* words;  // Pointer to an array of Word structures
    uint16_t size;       // An unsigned short integer to store the allocated size of the array
    uint16_t count;      // An unsigned short integer to store the current count of Words in the list
};

// Declaration of utility functions

// Function to calculate the length of a string
uint32_t strlength(const char* const word);
// Takes a constant pointer to a constant character (string) and returns its length as an unsigned 32-bit integer.

// Function to copy a string from source to destination
void strcopy(char* const destination, const char* const source);
// Takes two constant pointers to characters, one for the destination where the string will be copied to, and one for the source from which the string will be copied.

// Declaration of word functions

// Function to initialize a Word structure
void init_word(struct Word *word);
// Takes a pointer to a Word structure and initializes it.

// Function to update a Word structure with a new word and offset
void update_word(struct Word* word, const char* const wrd, uint16_t offset);
// Takes a pointer to a Word structure, a constant pointer to a constant character (string) and an unsigned short integer offset, then updates the Word structure with these values.

// Function to display a Word
void display_word(FILE* out, struct Word word);
// Takes a pointer to a FILE and a Word structure, then displays the Word to the specified FILE stream.

// Function to free the memory allocated to a Word
void free_word(struct Word word);
// Takes a Word structure and frees the memory allocated for the word string in it.

// Declaration of word list functions

// Function to initialize a Word_List structure
void init_list(struct Word_List *word_list, uint16_t size);
// Takes a pointer to a Word_List structure and an unsigned short integer size, then initializes the Word_List with the specified size.

// Function to add a Word to a Word_List
void add_word(struct Word_List *word_list, struct Word word);
// Takes a pointer to a Word_List structure and a Word structure, then adds the Word to the Word_List.

// Function to get a Word at a specific index in a Word_List
struct Word get_word_at_index(struct Word_List word_list, uint16_t index);
// Takes a Word_List structure and an unsigned short integer index, then returns the Word at the specified index in the Word_List.

// Function to free the memory allocated to a Word_List
void free_list(struct Word_List word_list);
// Takes a Word_List structure and frees the memory allocated for the words array in it.

// Function to display all Words in a Word_List
void display_words(FILE* out, struct Word_List word_list);
// Takes a pointer to a FILE and a Word_List structure, then displays all the Words in the Word_List to the specified FILE stream.

#endif // End of the preprocessor conditional
// Marks the end of the conditional compilation block to ensure the contents of this header file are only included once.
