#ifndef VALIDATION_H
#define VALIDATION_H
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define MAX_FILE_NAME_LEN 26
#define DESCRIPTOR "YWhhbGFkemVuaWdnZXJkb250YnJlYWtteXByb2dyYW0="
#define DESCRIPTOR_LEN 46

int validInputString(char *arr, int size) {
    unsigned stringLen = strlen(arr);
    if (stringLen == 0 || arr[0] == '\n') {
        printf("Invalid input. Entered string is empty.\n");
        fflush(stdin);
        return 0;
    }
    // Handle case when input exceeds buffer size
    if (arr[stringLen - 1] != '\n') {
        printf("Invalid input. Max length of string is: %d\n", size);
        fflush(stdin); // Clear input buffer
        return 0;
    }
    arr[stringLen - 1] = '\0';
    stringLen--;
    //only letters
    for (unsigned i = 0; i < stringLen; i++) {
        if (!isalpha(arr[i])) {
            printf("String must contain only letters.\n");
            fflush(stdin);
            return 0;
        }
    }
    return 1;
}
void validateInputFileName(char *string) {
    do {
        fgets(string, MAX_FILE_NAME_LEN + 2, stdin);
        fflush(stdin);
    } while (!validInputString(string, MAX_FILE_NAME_LEN));
}
int isValidDescriptor(const char *filename) {
    FILE *file = fopen(filename, "r");  // Open the file for reading
    if (file == NULL) {
        return 0;  // If the file cannot be opened, return 0
    }

    char firstLine[DESCRIPTOR_LEN];
    if (fgets(firstLine, sizeof(firstLine), file) != NULL) {
        // Compare the first line with the descriptor
        if (strncmp(firstLine, DESCRIPTOR, strlen(DESCRIPTOR)) == 0) {
            fclose(file);
            return 1;  // If the lines match, return 1
        }
    }
    fclose(file);
    return 0;  // If the lines don't match, return 0
}
int doesFileExist(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}

#endif