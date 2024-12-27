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
    if (arr[stringLen - 1] != '\n') {
        printf("Invalid input. Max length of string is: %d\n", size);
        fflush(stdin);
        return 0;
    }
    arr[stringLen - 1] = '\0';
    stringLen--;
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
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }
    char firstLine[DESCRIPTOR_LEN];
    if (fgets(firstLine, sizeof(firstLine), file) != NULL) {
        if (strncmp(firstLine, DESCRIPTOR, strlen(DESCRIPTOR)) == 0) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}
int doesFileExist(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1;
    }
    return 0;
}
float validateFloatInput(char *prompt, float min, float max) {
    float input = 0;
    int correctInput = 0;
    do {
        printf("%s", prompt);
        correctInput = scanf("%f", &input);
        if (correctInput != 1 || input < min || input > max) {
            printf("Invalid input.\n");
        }
        fflush(stdin);
    } while (correctInput != 1 || input < min || input > max);
    return input;
}
char validateCharInput(char *prompt, char choice1, char choice2, char choice3)
{
    char userChoice = 0;
    do
    {
        printf("%s", prompt);
        userChoice = getch();
        switch(userChoice) {
            case '1':
                printf("Your choice:%c\n", choice1);
                break;
            case '2':
                printf("Your choice:%c\n", choice2);
                break;
            case '0':
                printf("Your choice:%c\n", choice3);
                break;
            default:
                printf("Invalid input.\n");
                fflush(stdin);
                userChoice = 3;
                break;
        }
    }
    while (userChoice == 3);
    return userChoice;
}
int readSkipDescriptor(char *line, FILE *file) {
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File is empty or cannot be read.\n");
        fclose(file);
        return 0;
    }
    return 1;
}
#endif