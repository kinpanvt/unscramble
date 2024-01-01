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
#include "word.h"
#include <stdio.h>
#include <stdlib.h>

// The main function, expects ./scrambles <inputfile> <outputfile>
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: ./scrambles <inputfile> <outputfile>\n");
        return 1;
    }
    // creates an output file to write the output
    FILE *output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Failed to open output file\n");
        return 1;
    }
    // Opens the command file or read the parameters for the function
    FILE *commandFile = fopen(argv[1], "r");
    if (!commandFile) {
        printf("Failed to open command file");
        return 1;
    }
    // datasize is the size of our data
    int dataSize;
    // Reads the commands
    fscanf(commandFile, "%d", &dataSize);

    // checks to make sure data is correctly allocated
    uint8_t *data = malloc(dataSize);
    if (!data) {
        printf("Failed to allocate memory for data");
        fclose(commandFile);
        return 1;
    }

    // name of the input file
    char dataFilename[256];
    fscanf(commandFile, "%s", dataFilename);

    FILE *dataFile = fopen(dataFilename, "rb");
    if (!dataFile) {
        printf("Failed to open data file");
        free(data);
        fclose(commandFile);
        return 1;
    }

    fread(data, 1, dataSize, dataFile);

    // reads the data, and prints the required lines for output
    char mode[256];
    fscanf(commandFile, "%s", mode);
    fprintf(output, "Bytes in file:\t%d\n", dataSize);  // dataSize is of int type
    fprintf(output, "Name of file:\t%s\n", dataFilename);

    if (mode[0] == 'c' || mode[0] == 'C') { 
        fprintf(output, "Clear|Fuzzy:\tclear\n"); 
    } else if (mode[0] == 'f' || mode[0] == 'F') { 
        fprintf(output, "Clear|Fuzzy:\tfuzzy\n"); 
    }
    // prints clear or fuzzy depending on the file
    if (mode[0] == 'c') {
        read_clear(data, dataSize, output);
        
    } else if (mode[0] == 'f') {
        read_fuzzy(dataFile, dataSize); // Adjust to match your implementation
    } else {
        printf("Invalid mode: %s\n", mode);
    }

    fclose(dataFile);
    fclose(commandFile);
    fclose(output);

    free(data);

    return 0;
}