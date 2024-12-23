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
#endif // FUNCTION_H
