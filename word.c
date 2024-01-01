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

#include "word.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>



// strlength function counts the number of charecters in the array
uint32_t strlength(const char* const word) {
    uint32_t count = 0;	// counts the number of chars, starts at 0.
    while (word[count] != '\0')	// loops until we reach the null terminator
	{
        count++;	// adds to count. Since the loop stops when it incounters the null
					// terminator, it is not included in the count.
    }
    return count;	// returns count.
}

// The strcopy function copies all the charecters from source to destination.
// void strcopy(char* const destination, const char* const source) 
// {
//     uint32_t source_length = strlength(source);	// Uses strlength to find the length of the source.
// 	uint32_t destination_length = strlength(destination);	// Uses strlength to find the length of the destination.
//     if (source_length >= destination_length) 	// checks for destination being too small.
// 	{
//         //printf ("Error: Destination too small!");	// Outputs an error message if destination is too small.
//         return;
// 	}
// 	uint32_t i = 0;	// temp position of the current char in struct
//     while (source[i] != '\0')	// Checks for the null terminator to end the loop
// 								// P.S. should also be able to use source_length or
// 								// destination_length here by implementing something like
// 								//	while (source_length != i) || while (destination_length != i)
// 								//{ destination[i] = source[i];
// 								// i++;}
// 	{
//         destination[i] = source[i];	// copies over the chars. how to check for errors?
//         i++;	// increases the position by 1
//     }
//     destination[i] = '\0';	// Adds the null terminator to the end of the string
// }

// The strcopy function copies all the charecters from source to destination.
void strcopy(char* const destination, const char* const source) {
    uint32_t i = 0; // temp position of the current char in struct
    while (source[i] != '\0')   // Checks for the null terminator to end the loop
    {
        destination[i] = source[i]; // P.S. should also be able to use source_length or
 								// destination_length here by implementing something like
								//	while (source_length != i) || while (destination_length != i)
 								//{ destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';  // Adds the null terminator to the end of the string
}

void init_word(struct Word *word)	// Initializes the Struct.
{
    (*word).word = NULL;	// initializes char* word to NULL.
    (*word).offset = 0;		// sets the offset to 0.
}

// update_word copies over the words from wrd using strcopy.
void update_word(struct Word* word, const char* const wrd, uint16_t offset) {
    if ((*word).word != NULL)	// If word is not NULL, we need to free it
	{
        free((*word).word);		// frees word
    }
    
	// strlength gives us the length of the word and the +1 adds space for the null terminator
	// assigns the memory allocated to word.
	// Note: strcopy copies over the null terminator, so we dont need to worry about that.
    (*word).word = (char*) malloc((strlength(wrd) + 1) * sizeof(char));
    strcopy((*word).word, wrd);	// Copies the  words using strcopy.
    (*word).offset = offset;	// Sets the offset correctly.
}

// displays the word(s) from struct and outputs it to a file.
void display_word(FILE* out, struct Word word) 
{
    fprintf(out, "%-20s @ Offset: %02x\n", (word).word, word.offset);	// prints to file using correct formatting
}

// frees the internal word pointer.
void free_word(struct Word word) {
    free(word.word);	// uses free to free memory space.
}

// Allocates word pointers to an array - words, initializes size and count
void init_list( struct Word_List *word_list, uint16_t size )
{
    (*word_list).words = (struct Word*) malloc(size * sizeof(struct Word));	// uses malloc to allocate enough memory to words.
    (*word_list).size = size;	// initializes size
    (*word_list).count = 0;	// initializes count.
}

// adds a word at the end of the array, increases the size if array is full.
void add_word(struct Word_List* list, struct Word new_word) 
{
    if ((*list).count == (*list).size)	// if the arrray is full.
	{
        (*list).size *= 2;	// doubles the size of the list.
		// using realloc to increase the size of the list with all the words in the same order.
        (*list).words = (struct Word*) realloc((*list).words, (*list).size * sizeof(struct Word));
    }
	// uses update_word to add the words at the end of the list.
    update_word(&((*list).words[(*list).count]), new_word.word, new_word.offset);
    (*list).count++;	// increases count after word has been added.
}

// Copies the word at index and returns it if it is valid.
struct Word get_word_at_index(struct Word_List list, uint16_t index)
{
    struct Word copy;	// copy is a struct that will copy the word at the given index.
	struct Word fail;	// if the index is not valid
    if (index == 0 || index < list.count)	// Index must be valid and must be less than count.
	// A valid index can be from 0 to any number less than count, last index would be null terminator.
	{
        init_word(&copy);	// if it is valid, we have to iniitialize it.
        update_word(&copy, list.words[index].word, list.words[index].offset);	// Copies the word to copy struct.
        return copy;	// returns word at index if it is valid.
    }
    init_word(&fail);	// initializes fail if the index is not vaild to print error.
    update_word(&fail, "Word Not Found", 0);	// updates word not found at index 0.
    return fail;	// returns fail.
}

// frees the memory, sets words, size, count.
void free_list(struct Word_List list) 
{
	// goes through word list (has to be uint16_t because each element in the list is of that type).
    for (uint16_t i = 0; i < list.count; i++) 
	{
        free_word(list.words[i]);	// uses free_word to empty the list,  memory/ pointer.
    }
    free(list.words);	// frees memory allocation for the list itself.
    list.words = NULL;	// sets the words to NULL.
    list.size = 0;		// resets size to 0.
    list.count = 0;		// resets count to 0.
}

// loops over the list and prints the words in the list
void display_words(FILE* out, struct Word_List list) 
{
    for (uint32_t i = 0; i < list.count; i++) {
        display_word(out, list.words[i]);	// Goes word by word and uses display_word to display them
    }
}

