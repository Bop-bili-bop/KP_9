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
#define MAX_RECORD_LEN 512
#define MENU_KEY_CHOOSE_FILE '0'
#define MENU_KEY_CREATE_FILE '1'
#define MENU_KEY_OPEN_FILE '2'
#define MENU_KEY_DELETE_FILE '3'
#define MENU_KEY_CREATE_RECORD '4'
#define MENU_KEY_READ_RECORD '5'
#define MENU_KEY_EDIT_RECORD '6'
#define MENU_KEY_SORT_RECORD '7'
#define MENU_KEY_INSERT_RECORD '8'
#define MENU_KEY_DELETE_RECORD '9'
typedef struct record
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
        file = fopen(fileName, "w");
        if (file == NULL) {
            printf("Error! File wasn't created.\n");
            errorCatch = 1;
        }
        else
        {
            printf("File created successfully with name %s\n", fileName);
            fprintf(file, "%s\n", DESCRIPTOR);
            errorCatch = 0;
        }
        fclose(file);
    } while (errorCatch != 0);
}
void createNewFileByName(char *selectedFile) {
    do {
        printf("Enter name of file you want to create:\n");
        validateInputFileName(selectedFile);
        strcat(selectedFile, ".dat");
        if(doesFileExist(selectedFile)) {
            printf("File already exists. Please enter another name\n");
        }
    } while (doesFileExist(selectedFile));
    createFile(selectedFile);
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
        if (stat(entry->d_name, &fileStat) == 0)
        {
            if (S_ISREG(fileStat.st_mode))
            {  // Check if it's a regular file
                // Check if the file has a .dat extension
                const char *ext = strrchr(entry->d_name, '.');
                if (ext && strcmp(ext, ".dat") == 0)
                {
                    if (isValidDescriptor(entry->d_name))
                    {
                        strncpy(files[*fileCount], entry->d_name, MAX_FILE_NAME_LEN);
                        (*fileCount)++;
                    }
                }
            }
        }
    }
    if (*fileCount == 0) {
        printf("No files found.\n");
    }
    closedir(dp);
}
void chooseFile(char *selectedFile) {
    char files[MAX_FILES][MAX_FILE_NAME];
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
    int choice = 0;
    int validInput = 0;
    do {
        printf("Enter the number of the file you want to select:\n");
        validInput = scanf("%d", &choice);
        if (validInput != 1 || choice < 0 || choice > fileCount) {
            printf("Invalid choice.\n");
        }
        else {
            strncpy(selectedFile, files[choice - 1], MAX_FILE_NAME);
            printf("You selected file: %s", selectedFile);
            printf("\n");
        }
        fflush(stdin);
    }while (validInput != 1 || choice < 0 || choice > fileCount);
}
void readFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file '%s' for reading: %s\n", fileName, strerror(errno));
        return;
    }
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        printf("Error: File is empty or could not read the descriptor line.\n");
        fclose(file);
        return;
    }
    printf("Reading file '%s' contents (ignoring descriptor):\n", fileName);
    while (fgets(buffer, sizeof(buffer), file)) {
        printf("%s", buffer);
    }
    fclose(file);
    printf("\nFile reading complete.\n");
}
int deleteFile(const char *fileName) {
    if (remove(fileName) == 0) {
        printf("File '%s' deleted successfully.\n", fileName);
        return 1;
    }
    printf("Error deleting file '%s': %s\n", fileName, strerror(errno));
    return 0;
}

int readRecordsFromFile(const char *filename, record **records) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file for reading.\n");
        return 0;
    }

    record *recordList = calloc(100, sizeof(record));  // Allocate memory for 100 records
    int recordCount = 0;
    char line[MAX_RECORD_LEN];
    // Read records line by line
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Record", 6) == 0) {
            // Parse record fields: region, area, population
            fgets(line, sizeof(line), file);  // Read region
            sscanf(line, "region: %s", recordList[recordCount].regionName);

            fgets(line, sizeof(line), file);  // Read area
            sscanf(line, "area: %f", &recordList[recordCount].squareArea);

            fgets(line, sizeof(line), file);  // Read population
            sscanf(line, "population: %f", &recordList[recordCount].population);

            recordCount++;
        }
    }
    fclose(file);
    *records = recordList;
    return recordCount;
}

// Function to write records back to the file
void writeRecordsToFile(const char *filename, record *records, int recordCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file for writing.\n");
        return;
    }

    // Write all records back to the file
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record %d:\n", i + 1);
        fprintf(file, "region: %s\n", records[i].regionName);
        fprintf(file, "area: %.2f\n", records[i].squareArea);
        fprintf(file, "population: %.2f\n", records[i].population);
    }

    fclose(file);
}

// Function to delete a record by its index
void deleteRecordFromFile(const char *filename, int recordIndex) {
    record *records = NULL;
    int recordCount = readRecordsFromFile(filename, &records);
    if (recordCount == 0) {
        printf("No records found to delete.\n");
        return;
    }

    // Validate the record index
    if (recordIndex < 0 || recordIndex >= recordCount) {
        printf("Invalid record index.\n");
        free(records);
        return;
    }
    // Shift all records after the deleted one to the left
    for (int i = recordIndex; i < recordCount - 1; i++) {
        records[i] = records[i + 1];
    }
    // Update the record count
    recordCount--;
    // Rewrite the file with the updated records
    writeRecordsToFile(filename, records, recordCount);
    free(records);
    printf("Record deleted successfully.\n");
}

void editRecordInFile(const char *filename, int recordIndex) {
    record *records = NULL;
    int correctInput = 0;
    int recordCount = readRecordsFromFile(filename, &records);
    if (recordCount == 0) {
        printf("No records found to edit.\n");
        return;
    }

    // Перевірка валідності індексу
    if (recordIndex < 0 || recordIndex >= recordCount) {
        printf("Invalid record index.\n");
        free(records);
        return;
    }

    // Редагування вибраного запису
    printf("Editing record %d:\n", recordIndex + 1);
    printf("Current region: %s\n", records[recordIndex].regionName);
    printf("Enter new region: ");
    do {
        correctInput = scanf("%s", records[recordIndex].regionName);
        if (correctInput != 1) {
            printf("Invalid input.\n");
        }
    } while (correctInput != 1);

    printf("Current area: %.2f\n", records[recordIndex].squareArea);
    printf("Enter new area: ");
    do {
        correctInput = scanf("%f", &records[recordIndex].squareArea);;
        if (correctInput != 1) {
            printf("Invalid input.\n");
        }
    } while (correctInput != 1);

    printf("Current population: %.2f\n", records[recordIndex].population);
    printf("Enter new population: ");
    do {
        correctInput = scanf("%f", &records[recordIndex].population);
        if (correctInput != 1) {
            printf("Invalid input.\n");
        }
    } while (correctInput != 1);
    // Перезапис файлу
    writeRecordsToFile(filename, records, recordCount);
    free(records);
    printf("Record edited successfully.\n");
}

int compareRecords(const void *a, const void *b, int field, int order) {
    const record *recA = (const record *)a;
    const record *recB = (const record *)b;
    float result = 0;

    switch (field) {
        case 0:  // Сортування за назвою регіону
            result = strcmp(recA->regionName, recB->regionName);
            break;
        case 1:  // Сортування за площею
            result = recA->squareArea - recB->squareArea;
            break;
        case 2:  // Сортування за населенням
            result = recA->population - recB->population;
            break;
        default:
            printf("Error.\n");
            break;
    }

    return order == 1 ? -result : result;  // Інверсія для спадання
}
void sortRecordsInFile(const char *filename, int field, int order) {
    record *records = NULL;
    int recordCount = readRecordsFromFile(filename, &records);
    if (recordCount == 0) {
        printf("No records found to sort.\n");
        return;
    }

    // Сортування записів
    qsort(records, recordCount, sizeof(record),
        compareRecords);

    // Перезапис файлу
    writeRecordsToFile(filename, records, recordCount);
    free(records);
    printf("Records sorted successfully.\n");
}


void printMenuOptions() {
    printf("%c - choose a file to work with\n"
               "%c - create a file\n"
               "%c - open a file\n"
               "%c - delete a file\n"
               "%c - create a record\n"
               "%c - read a record\n"
               "%c - edit a record\n"
               "%c - sort a record\n"
               "%c - insert a record\n"
               "%c - delete a record\n"
               "ESC - exit the program\n"
               ,MENU_KEY_CHOOSE_FILE
               ,MENU_KEY_CREATE_FILE
               ,MENU_KEY_OPEN_FILE
               ,MENU_KEY_DELETE_FILE
               ,MENU_KEY_CREATE_RECORD
               ,MENU_KEY_READ_RECORD
               ,MENU_KEY_EDIT_RECORD
               ,MENU_KEY_SORT_RECORD
               ,MENU_KEY_INSERT_RECORD
               ,MENU_KEY_DELETE_RECORD);
}
#endif // FUNCTION_H
