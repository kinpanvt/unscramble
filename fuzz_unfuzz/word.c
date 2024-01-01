#include "word.h" // Include the header file 'word.h' which defines the structures and function prototypes

// Implementation of utility functions

// Function to calculate the length of a string
uint32_t strlength(const char* const word) {
    uint32_t length = 0; // Initialize length variable
    while (word[length] != '\0') { // Loop until the null terminator is reached
        length++; // Increment length for each character in the string
    }
    return length; // Return the length of the string
}

// Function to copy a string from source to destination
void strcopy(char* const destination, const char* const source) {
    uint32_t i = 0; // Initialize index variable
    while (source[i] != '\0') { // Loop until the null terminator in the source string is reached
        destination[i] = source[i]; // Copy each character from source to destination
        i++; // Increment index
    }
    destination[i] = '\0'; // Copy the null terminator to the destination string
}

// Implementation of functions for the Word structure

// Function to initialize a Word structure
void init_word(struct Word *word) {
    word->word = NULL; // Set the word pointer to NULL
    word->offset = 0; // Set the offset to 0
}

// Function to update a Word structure with a new word and offset
void update_word(struct Word* word, const char* const wrd, uint16_t offset) {
    if (word->word != NULL) {
        free(word->word); // Free the previously allocated memory if it exists
    }
    // Allocate memory for the new word (+1 for the null terminator)
    word->word = (char*) malloc((strlength(wrd) + 1) * sizeof(char));
    strcopy(word->word, wrd); // Copy the new word into the allocated memory
    word->offset = offset; // Set the new offset
}

// Function to display a Word structure
void display_word(FILE* out, struct Word word) {
    // Print the word and its offset to the specified output file stream
    fprintf(out, "%-20s @ Offset: %02x\n", word.word, word.offset);
}

// Function to free the memory allocated for a Word structure
void free_word(struct Word word) {
    if (word.word != NULL) {
        free(word.word); // Free the memory allocated for the word string
        // Note: word.word is not set to NULL since 'word' is a copy and not the original struct
    }
}


// Implementation of functions for the Word_List structure

// Function to initialize a Word_List structure
void init_list(struct Word_List *word_list, uint16_t size) {
    // Allocate memory for the array of Word structures
    word_list->words = (struct Word*)malloc(size * sizeof(struct Word));
    word_list->size = size; // Set the allocated size
    word_list->count = 0; // Initialize the word count to 0
}

// Function to add a Word to a Word_List
void add_word(struct Word_List *word_list, struct Word word) {
    if (word_list->count == word_list->size) { // Check if the array is full
        word_list->size *= 2; // Double the size of the array
        // Reallocate memory for the array with the new size
        word_list->words = (struct Word*)realloc(word_list->words, word_list->size * sizeof(struct Word));
    }
    // Add the new word to the array and increment the count
    word_list->words[word_list->count++] = word;
}

// Function to get a Word at a specific index in a Word_List
struct Word get_word_at_index(struct Word_List word_list, uint16_t index) {
    if (index < word_list.count) { // Check if the index is within bounds
        return word_list.words[index]; // Return the word at the specified index
    } else {
        // If index is out of bounds, return a Word structure with "Word Not Found"
        struct Word not_found_word;
        init_word(&not_found_word); // Initialize the not_found_word
        update_word(&not_found_word, "Word Not Found", 0); // Set its value to "Word Not Found"
        return not_found_word;
    }
}

// Function to free the memory allocated for a Word_List
void free_list(struct Word_List word_list) {
    free(word_list.words); // Free the memory allocated for the array of Word structures
    // Note: Individual words are not freed here; this is likely handled elsewhere
}

// Function to display all Words in a Word_List
void display_words(FILE* out, struct Word_List word_list) {
    for (uint16_t i = 0; i < word_list.count; i++) {
        display_word(out, word_list.words[i]); // Display each word in the list
    }
}
