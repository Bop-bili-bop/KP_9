#ifndef VALIDATION_H
#define VALIDATION_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILE_NAME_LEN 26

int validInputString(char *arr, int size) {
    unsigned stringLen = strlen(arr);

    if (stringLen == 0 || arr[0] == '\n') {
        printf("Invalid input. Entered string is empty.\n");
        return 0;
    }

    // Handle case when input exceeds buffer size
    if (arr[stringLen - 1] != '\n') {
        printf("Invalid input. Max length of string is: %d\n", size);
        while (getchar() != '\n' && !feof(stdin)); // Clear input buffer
        return 0;
    }

    // Remove trailing newline
    arr[stringLen - 1] = '\0';
    stringLen--;

    // Check if the string contains only letters
    for (unsigned i = 0; i < stringLen; i++) {
        if (!isalpha(arr[i])) {
            printf("String must contain only letters.\n");
            return 0;
        }
    }

    return 1;
}
void validateInputFileName(char *string) {
    do {
        printf("Enter file name (max %d characters):\n", MAX_FILE_NAME_LEN);
        fgets(string, MAX_FILE_NAME_LEN + 2, stdin);
        printf("Raw input: '%s'\n", string);
    } while (!validInputString(string, MAX_FILE_NAME_LEN));
}
#endif