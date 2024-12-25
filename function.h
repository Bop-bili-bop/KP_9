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
#define FLOAT_MIN 1e-4
#define FLOAT_MAX 1e7
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
    closedir(dp);
}
void chooseFile(char *selectedFile) {
    char files[MAX_FILES][MAX_FILE_NAME];
    unsigned fileCount = 0;
    listExistingDatFiles(files, &fileCount);
    if (fileCount == 0) {
        printf("No supported files found.\n");
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
void readFile(char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error opening file '%s' for reading: %s\n", fileName, strerror(errno));
        return;
    }
    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *buffer = malloc(fileSize + 1); // +1 для термінального '\0'
    if (buffer == NULL) {
        printf("Error allocating memory.\n");
        fclose(file);
        return;
    }
    unsigned bytesRead = fread(buffer, 1, fileSize, file);
    if (bytesRead > fileSize) {
        printf("Error reading file.\n");
        free(buffer);
        fclose(file);
        return;
    }
    buffer[bytesRead] = '\0';
    char *start = buffer;
    char *lineEnd = strchr(start, '\n');
    if (lineEnd != NULL) {
        start = lineEnd + 1;
    }
    printf("File content (ignoring the descriptor):\n");
    do {
        lineEnd = strchr(start, '\n');
        if (lineEnd != NULL) {
            *lineEnd = '\0'; // Завершуємо рядок
        }
        printf("%s\n", start);
        if (lineEnd != NULL) {
            start = lineEnd + 1;
        }
        else {
            break;
        }
    } while (*start != '\0');
    free(buffer);
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
int countRecordsInFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return 0;
    }
    int recordCount = 0;
    char line[MAX_RECORD_LEN];
    // Підрахунок записів за маркерами "Record :"
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record") != NULL) {
            recordCount++;
        }
    }
    fclose(file);
    return recordCount;
}

void writeRecordsToFile(char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Failed to open file for writing.\n");
        return;
    }
    int existingRecordCount = countRecordsInFile(filename);
    int currentRecordIndex = existingRecordCount + 1;
    record records = {"", 0 ,0};
    printf("Writing record to file %s\n", filename);
    do {
        printf("Enter region name:\n");
        fgets (records.regionName, MAX_REGION_LEN + 1, stdin);
        fflush(stdin);
    } while (!validInputString(records.regionName, MAX_REGION_LEN));
    records.squareArea = validateFloatInput("Enter area of the region\n", FLOAT_MIN, FLOAT_MAX);
    records.population = validateFloatInput("Enter number of the population\n", FLOAT_MIN, FLOAT_MAX);
    fprintf(file, "Record #%d:\n", currentRecordIndex);
    fprintf(file, "region: %s\n", records.regionName);
    fprintf(file, "area: %f\n", records.squareArea);
    fprintf(file, "population: %f\n", records.population);
    fclose(file);
    printf("Record #%d successfully written in file %s\n", currentRecordIndex, filename);
}
void readAllRecords(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }

    char line[MAX_RECORD_LEN];
    int recordIndex = 0;
    printf("Records in the file '%s':\n", filename);
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File '%s' is empty or cannot be read.\n", filename);
        fclose(file);
        return;
    }
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            printf("%d) ", ++recordIndex); // Індекс запису
        }
        printf("%s", line); // Виводимо лінію
    }
    fclose(file);
    if (recordIndex == 0) {
        printf("No records found in the file.\n");
    }
}
void readSpecificRecord(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }
    int targetIndex = 0;
    int validInput = 0;
    printf("Enter the record number you want to read:\n");
    do {
        validInput = scanf("%d", &targetIndex);
        if(validInput != 1 || targetIndex <= 0 || targetIndex > 10){
            printf("Invalid input. Index should be [1;10]\n");
        }
        fflush(stdin);
    } while (validInput != 1 || targetIndex <= 0 || targetIndex > 10);
    char line[MAX_RECORD_LEN];
    int recordIndex = 0;
    int foundRecord = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            recordIndex++; // Підрахунок записів
            if (recordIndex == targetIndex) {
                foundRecord = 1;
            }
            else if (foundRecord) {
                break; // Завершення читання після вибраного запису
            }
        }

        if (foundRecord) {
            printf("%s", line); // Виводимо лінію обраного запису
        }
    }
    fclose(file);
    if (!foundRecord) {
        printf("Record %d not found in the file '%s'.\n", targetIndex, filename);
    }
}
void deleteRecord(char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }
    char tempFilename[MAX_FILE_NAME];
    snprintf(tempFilename, sizeof(tempFilename), "temp_%s", filename);
    FILE *tempFile = fopen(tempFilename, "w");
    if (!tempFile) {
        printf("Failed to create temporary file for writing.\n");
        fclose(file);
        return;
    }
    int targetIndex = 0;
    int validInput = 0;
    printf("Enter the record number you want to delete in %s:\n", filename);
    do {
        validInput = scanf("%d", &targetIndex);
        if(validInput != 1 || targetIndex <= 0 || targetIndex > 10){
            printf("Invalid input. Index should be [1;10]\n");
        }
        fflush(stdin);
    } while (validInput != 1 || targetIndex <= 0 || targetIndex > 10);
    char line[MAX_RECORD_LEN];
    int recordIndex = 0;
    int isDeleting = 0;
    int recordDeleted = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            recordIndex++;
            isDeleting = (recordIndex == targetIndex);
            if (isDeleting) {
                recordDeleted = 1;
            }
        }
        if (!isDeleting) {
            fprintf(tempFile, "%s", line);
        }
    }
    fclose(file);
    fclose(tempFile);
    if (recordDeleted) {
        remove(filename);
        rename(tempFilename, filename);
        printf("The %d record has been successfully deleted.\n", targetIndex);
    }
    else {
        remove(tempFilename);
        printf("Record %d not found in the file '%s'.\n", targetIndex, filename);
        return;
    }
    // Виведення оновленого списку записів
    printf("Updated records:\n");
    readAllRecords(filename);
}
/*
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
}*/

#endif // FUNCTION_H
