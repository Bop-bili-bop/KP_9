#ifndef FUNCTION_H
#define FUNCTION_H

#include "validation.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_REGION_LEN 3
#define MAX_FILE_NAME 32
#define MAX_FILES 10

#define MAX_RECORD_IN_FILE 10
typedef struct Record
{
    char regionName[MAX_REGION_LEN];
    float squareArea;
    float population;
} record;
void createFile(char *fileName) {
    unsigned errorCatch = 0;
    FILE *file;
    do
    {
        errorCatch = 0;
        // Try creating the file
        file = fopen(fileName, "w");
        if (file == NULL) {
            printf("Error! File wasn't created.\n");
            errorCatch = 1;
        }
        else
        {
            // File created successfully
            printf("File created successfully with name %s\n", fileName);
            fprintf(file, "%s\n", DESCRIPTOR);
            errorCatch = 0; // Success, no error
        }
        fclose(file);
    } while (errorCatch != 0); // Retry if error occurred
}
void listExistingDatFiles(char files[][MAX_FILE_NAME], unsigned *fileCount)
{
    struct dirent *entry;
    DIR *dp = opendir(".");  // Open the current directory

    if (!dp) {
        printf("Failed to open directory.\n");
        return;
    }

    *fileCount = 0;
    while ((entry = readdir(dp)) != NULL) {
        struct stat fileStat;
        if (stat(entry->d_name, &fileStat) == 0) {
            if (S_ISREG(fileStat.st_mode)) {  // Check if it's a regular file
                // Check if the file has a .dat extension
                const char *ext = strrchr(entry->d_name, '.');
                if (ext && strcmp(ext, ".dat") == 0) {
                    if (isValidDescriptor(entry->d_name)) {
                        strncpy(files[*fileCount], entry->d_name, MAX_FILE_NAME_LEN);
                        (*fileCount)++;
                    }
                }
            }
        }
    }
    closedir(dp);
}
void chooseFile(char *selectedFile) {
    char files[100][MAX_FILE_NAME]; // Array to hold filenames
    unsigned fileCount = 0;

    listExistingDatFiles(files, &fileCount);

    if (fileCount == 0) {
        printf("No .dat files found.\n");
        return;
    }

    printf("Available .dat files:\n");
    for (int i = 0; i < fileCount; i++) {
        printf("%d. %s\n", i + 1, files[i]);
    }
    printf("Enter the number of the file you want to select or name of file you want to create:\n");
    int choice;
    if (scanf("%d", &choice) == 1 && choice > 0 && choice <= fileCount) {
        fflush(stdin);
        // User chose a file by number
        strncpy(selectedFile, files[choice - 1], MAX_FILE_NAME);
        printf("You selected file: %s", selectedFile);
        printf("\n");
    }
    else
    {
        do {
            validateInputFileName(selectedFile);
            strcat(selectedFile, ".dat");
            if(doesFileExist(selectedFile)) {
                printf("File already exists. Please select file from the list or enter another name\n");
            }
        } while (doesFileExist(selectedFile));
        createFile(selectedFile);
    }
}

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
    }
    printf("Error deleting file '%s': %s\n", fileName, strerror(errno));
    return 0;  // Return failure if the file could not be deleted

}
void addRecordToFile(const char *fileName, record *newRecord) {
    FILE *file = fopen(fileName, "a+");  // Відкриваємо файл для додавання
    if (file == NULL) {
        printf("Error opening file '%s' for writing: %s\n", fileName, strerror(errno));
    }
    else {
        char buffer[128];
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
    }  // Успішний запис
}
#endif // FUNCTION_H
