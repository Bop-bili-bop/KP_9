#ifndef FUNCTION_H
#define FUNCTION_H

#include "validation.h"
#include <stdio.h>
#include <string.h>
#define MAX_REGION_LEN 3
#define MAX_FILE_NAME 32
#define MAX_FILES 10
#define DESCRIPTOR "YWhhbGFkemVuaWdnZXJkb250YnJlYWtteXByb2dyYW0="

typedef struct Record
{
    char regionName[MAX_REGION_LEN];
    float squareArea;
    float population;
} record;

unsigned fileCount = MAX_FILES;
char fileList[MAX_FILES][MAX_FILE_NAME] = {};

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Function to read the contents of a file and print to the console
void readFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file '%s' for reading: %s\n", fileName, strerror(errno));
        return;  // Exit if the file cannot be opened
    }

    char buffer[256];

    // Skip the descriptor line (first line)
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error: File is empty or could not read the descriptor line.\n");
        fclose(file);
        return;
    }

    // Now, read and print the rest of the file
    printf("Reading file '%s' contents (ignoring descriptor):\n", fileName);
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);  // Print each line read from the file, ignoring the descriptor
    }

    fclose(file);  // Close the file after reading
    printf("\nFile reading complete.\n");
}

// Function to delete a file
int deleteFile(const char *fileName) {
    if (remove(fileName) == 0) {
        printf("File '%s' deleted successfully.\n", fileName);
        return 1;  // Return success
    } else {
        printf("Error deleting file '%s': %s\n", fileName, strerror(errno));
        return 0;  // Return failure if the file could not be deleted
    }
}



int addFileToList(char fileList[][MAX_FILE_NAME], unsigned *fileCount, const char *fileName) {
    if (*fileCount >= MAX_FILES) {
        printf("Error: Maximum number of files reached.\n");
        return 0;
    }
    strcpy(fileList[*fileCount], fileName);
    (*fileCount)++;
    return 1;
}

void createFile(char *fileName, unsigned *fileCount) {
    unsigned errorCatch = 0;
    do {
        // Validate file name before anything else
        validateInputFileName(fileName);

        if (strlen(fileName) + 4 < MAX_FILE_NAME) {
            // Append ".dat" extension
            strcat(fileName, ".dat");
        } else {
            printf("Error: File name is too long to append extension.\n");
            errorCatch = 1;
            continue; // Skip the rest and retry with the original file name
        }

        // Try creating the file
        FILE *file = fopen(fileName, "w");
        if (file == NULL) {
            printf("Error! File wasn't created.\n");
            errorCatch = 1;
            continue; // Skip the rest and retry
        }

        // File created successfully, add it to the list
        if (addFileToList(fileList, fileCount, fileName)) {
            printf("File created successfully with name %s\n", fileName);
            fprintf(file, "%s\n", DESCRIPTOR);
            fclose(file);
            errorCatch = 0; // Success, no error
        } else {
            errorCatch = 1;
            fclose(file); // Ensure to close file even if not added to the list
        }

    } while (errorCatch != 0); // Retry if error occurred
}

int validateProgramFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return 0;  // File couldn't be opened
    }

    char buffer[128];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return 0;  // Error reading file
    }

    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    fclose(file);
    return strcmp(buffer, DESCRIPTOR) == 0; // Returns 1 if descriptor matches
}

int selectFileFromList(const char fileList[][MAX_FILE_NAME], unsigned fileCount, char *selectedFile) {
    int choice = 0;

    printf("\nAvailable valid program files:\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%d. %s\n", i + 1, fileList[i]);
    }

    while (1) {
        printf("Select a file by entering its number (or 0 to cancel): ");
        if (scanf("%d", &choice) != 1) {
            fflush(stdin);
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (choice == 0) {
            printf("Operation canceled.\n");
            return 0; // User canceled selection
        }

        if (choice < 1 || choice > fileCount) {
            printf("Invalid choice. Please select a valid number.\n");
        } else {
            break;
        }
    }

    // Copy selected file name
    strcpy(selectedFile, fileList[choice - 1]);
    return 1;
}

int addRecordToFile(const char *fileName, record *newRecord) {
    FILE *file = fopen(fileName, "a+");  // Відкриваємо файл для додавання
    if (file == NULL) {
        printf("Error opening file '%s' for writing: %s\n", fileName, strerror(errno));
        return 0;  // Якщо файл не відкритий, повертаємо помилку
    }

    // Перевіряємо наявність дескриптора, якщо його немає, записуємо його
    char buffer[128];
    if (fgets(buffer, sizeof(buffer), file) == NULL || strcmp(buffer, DESCRIPTOR) != 0) {
        // Якщо дескриптор відсутній, записуємо його на початок файлу
        fseek(file, 0, SEEK_SET);  // Переміщаємо курсор на початок файлу
        fprintf(file, "%s\n", DESCRIPTOR);  // Записуємо дескриптор
    }

    // Підраховуємо кількість вже записаних записів для правильного номерування
    fseek(file, 0, SEEK_SET);  // Переміщаємо курсор на початок файлу
    int recordCount = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "region:", 7) == 0) {
            recordCount++;  // Кожен раз, коли зустрічаємо "region:", збільшуємо лічильник
        }
    }

    // Записуємо новий запис з нумерацією
    fprintf(file, "Record %d:\n", recordCount + 1);  // Додаємо нумерацію
    fprintf(file, "region: %s\n", newRecord->regionName);
    fprintf(file, "area: %.2f\n", newRecord->squareArea);
    fprintf(file, "population: %.2f\n", newRecord->population);
    fprintf(file, "\n");  // Додаємо порожній рядок між записами для зручності

    fclose(file);  // Закриваємо файл після запису
    return 1;  // Успішний запис
}
#endif // FUNCTION_H
