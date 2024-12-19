#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to get the decimal value of a Roman numeral character
int romanToDecimal(char c) {
    // Convert Roman numeral character to its decimal value
    switch (toupper(c)) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
        default: return 0;
    }
}

// Function to check if a string is a valid Roman numeral
int isValidRoman(const char *roman) {
    for (int i = 0; roman[i] != '\0'; i++) {
        if (romanToDecimal(roman[i]) == 0) {
            return 0; // Invalid character found
        }
    }
    return 1;
}

// Function to convert a Roman numeral string to a decimal number
int convertRomanToDecimal(const char *roman) {
    int total = 0, prevValue = 0;
    // Loop through the Roman numeral string from right to left
    for (int i = strlen(roman) - 1; i >= 0; i--) {
        int currentValue = romanToDecimal(roman[i]);
        // Add or subtract based on the order of values
        if (currentValue >= prevValue) {
            total += currentValue;
        } else {
            total -= currentValue;
        }
        prevValue = currentValue;
    }
    return total;
}

// Function to convert a number to words
void numberToWords(int number, char *words) {
    const char *ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    const char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    const char *thousands[] = {"", "Thousand", "Million", "Billion"};

    char temp[512] = "";
    int chunkCount = 0;

    while (number > 0) {
        int chunk = number % 1000;
        number /= 1000;

        if (chunk > 0) {
            char chunkWords[256] = "";

            if (chunk / 100 > 0) {
                sprintf(chunkWords + strlen(chunkWords), "%s Hundred ", ones[chunk / 100]);
            }

            chunk %= 100;

            if (chunk >= 10 && chunk <= 19) {
                sprintf(chunkWords + strlen(chunkWords), "%s ", teens[chunk - 10]);
            } else {
                if (chunk / 10 > 0) {
                    sprintf(chunkWords + strlen(chunkWords), "%s ", tens[chunk / 10]);
                }
                if (chunk % 10 > 0) {
                    sprintf(chunkWords + strlen(chunkWords), "%s ", ones[chunk % 10]);
                }
            }

            if (chunkCount > 0) {
                sprintf(chunkWords + strlen(chunkWords), "%s ", thousands[chunkCount]);
            }

            // Prepend chunkWords to temp to ensure proper order
            char newTemp[512] = "";
            sprintf(newTemp, "%s%s", chunkWords, temp);
            strcpy(temp, newTemp);
        }

        chunkCount++;
    }

    // Remove trailing space
    size_t len = strlen(temp);
    if (len > 0 && temp[len - 1] == ' ') {
        temp[len - 1] = '\0';
    }

    strcpy(words, temp);
}

// Function to process a single line of input
void processLine(const char *line, FILE *outputFile) {
    char token[100];
    int total = 0, value;
    int validTokens = 0;

    // Create a copy of the line to tokenize
    char lineCopy[256];
    strcpy(lineCopy, line);

    // Tokenize the line using space as a delimiter
    char *ptr = strtok(lineCopy, " ");
    while (ptr != NULL) {
        if (isValidRoman(ptr)) {
            // Convert valid Roman numeral to decimal
            value = convertRomanToDecimal(ptr);
            total += value;
            validTokens++;
        } else if (strchr("+-*/", *ptr)) {
            // Ignore operators
        } else {
            // Invalid token found
            fprintf(outputFile, "Invalid input: %s\n", line);
            return;
        }
        ptr = strtok(NULL, " ");
    }

    // If there are no valid Roman numerals, mark the line as invalid
    if (validTokens == 0) {
        fprintf(outputFile, "Invalid input: %s\n", line);
        return;
    }

    // Convert the total to words
    char resultInWords[512] = "";
    numberToWords(total, resultInWords);

    // Write the result in words to the output file
    fprintf(outputFile, "%s\n", resultInWords);
}


// Function to read and process the input file
void processFile(const char *inputFileName, const char *outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r");
    if (!inputFile) {
        perror("Failed to open input file");
        return;
    }

    FILE *outputFile = fopen(outputFileName, "w");
    if (!outputFile) {
        perror("Failed to open output file");
        fclose(inputFile);
        return;
    }

    char line[256];
    // Read each line from the input file and process it
    while (fgets(line, sizeof(line), inputFile)) {
        // Remove newline character if present
        line[strcspn(line, "\n")] = '\0';
        processLine(line, outputFile);
    }

    fclose(inputFile);
    fclose(outputFile);
}

int main() {
    // Define the input and output file names
    const char *inputFileName = "Input.txt";
    const char *outputFileName = "Output.txt";

    // Process the input file and write to the output file
    processFile(inputFileName, outputFileName);

    return 0;
}
