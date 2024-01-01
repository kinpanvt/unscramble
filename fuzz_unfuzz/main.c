#include <stdio.h> // Include standard input/output library for functions like printf, fscanf, etc.
#include <stdlib.h> // Include standard library for functions like malloc, free, etc.
#include "word.h" // Include a custom header file named "word.h"
#include "unscramble.h" // Include another custom header file named "unscramble.h"

// Define a custom string comparison function
int my_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; // Increment pointer s1 to check next character
        s2++; // Increment pointer s2 to check next character
    }
    // Subtract the values at pointers to get the difference (standard strcmp behavior)
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

int main(int argc, char *argv[]) {
    // Check if the number of command line arguments is not equal to 3
    if (argc != 3) {
        // Print an error message to stderr and show usage format
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1; // Exit with an error status
    }

    // Open the input file in read mode
    FILE *inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        perror("Error opening input file"); // Print error if file opening fails
        return 1;
    }

    // Read the data size from the input file
    int dataSize;
    fscanf(inputFile, "%d\n", &dataSize);

    // Read the name of the data file from the input file
    char dataFileName[100];
    fscanf(inputFile, "%s\n", dataFileName);

    // Read the mode (clear or fuzzy) from the input file
    char mode[6];
    fscanf(inputFile, "%s\n", mode);

    // Close the input file as it's no longer needed
    fclose(inputFile);

    // Open the binary data file in read-binary mode
    FILE *binaryFile = fopen(dataFileName, "rb");
    if (!binaryFile) {
        perror("Error opening binary data file"); // Print error if file opening fails
        return 1;
    }

    // Allocate memory to store the data
    uint8_t *data = malloc(dataSize);
    // Read the data from the binary file into the allocated memory
    fread(data, sizeof(uint8_t), dataSize, binaryFile);
    // Close the binary file as it's no longer needed
    fclose(binaryFile);

    // Open the output file in write mode
    FILE *outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        perror("Error opening output file"); // Print error if file opening fails
        free(data); // Free the allocated memory before exiting
        return 1;
    }

    // Write metadata (data size, file name, mode) to the output file
    fprintf(outputFile, "Bytes in file:\t%d\n", dataSize);
    fprintf(outputFile, "Name of file:\t%s\n", dataFileName);
    fprintf(outputFile, "Clear|Fuzzy:\t%s\n", mode);

    // Check the mode and call the appropriate unscramble function
    if (my_strcmp(mode, "clear") == 0) {
        unscrambleClearText(data, dataSize, outputFile);
    } else {
        unscrambleFuzzyText(data, dataSize, outputFile);
    }

    // Close the output file
    fclose(outputFile);
    // Free the allocated memory for data
    free(data);
    return 0; // Return success status
}
