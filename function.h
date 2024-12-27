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
    DIR *dp = opendir(".");
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
            {  // if regular file
                // has a .dat extension
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
        if (validInput != 1 || choice <= 0 || choice > fileCount) {
            printf("Invalid choice.\n");
        }
        else {
            strncpy(selectedFile, files[choice - 1], MAX_FILE_NAME);
            printf("You selected file: %s", selectedFile);
            printf("\n");
        }
        fflush(stdin);
    }while (validInput != 1 || choice <= 0 || choice > fileCount);
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
    if (bytesRead == 0) {
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
    if (file == NULL) {
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
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }

    int existingRecordCount = countRecordsInFile(filename);
    if (existingRecordCount >= MAX_RECORD_IN_FILE) {
        printf("Cannot add more records. The file already contains the maximum number of records (%d).\n", MAX_RECORD_IN_FILE);
        fclose(file);
        return;
    }
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
    if (file == NULL) {
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
    if (file == NULL) {
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
    if (file == NULL) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }
    char tempFilename[MAX_FILE_NAME];
    snprintf(tempFilename, sizeof(tempFilename), "temp_%s", filename);//&&&
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
void sortRecordsInFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }
    char line[MAX_RECORD_LEN];
    record records[MAX_RECORD_IN_FILE];
    int recordCount = 0;
    // Пропускаємо дескриптор
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File is empty or cannot be read.\n");
        fclose(file);
        return;
    }
    // Зчитуємо записи
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            // Читаємо запис
            record currentRecord;
            fgets(line, sizeof(line), file); // Читаємо region
            sscanf(line, "region: %s", currentRecord.regionName);
            fgets(line, sizeof(line), file); // Читаємо area
            sscanf(line, "area: %f", &currentRecord.squareArea);
            fgets(line, sizeof(line), file); // Читаємо population
            sscanf(line, "population: %f", &currentRecord.population);

            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);

    if (recordCount <= 1) {
        printf("Not enough records found to sort.\n");
        return;
    }

    // Вибір сортування
    char sortOrder = validateCharInput("descending order: enter 1\nascending order: enter 2\n",
                                    '1','2', '1');
    char sortField = validateCharInput("0 - to sort by region\n1 - to sort by area\n2 - to sort by population\n",
                                '1','2', '0');

    printf("sortOrder: %c; field: %c\n", sortOrder, sortField);

    // Сортування
    for (int i = 0; i < recordCount - 1; i++) {
        for (int j = 0; j < recordCount - i - 1; j++) {
            int compare = 0;
            switch (sortField) {
                case '0': // Сортування за regionName
                    compare = strcmp(records[j].regionName, records[j + 1].regionName);
                    break;
                case '1': // Сортування за squareArea
                    compare = (records[j].squareArea > records[j + 1].squareArea) ? 1 : -1;
                    break;
                case '2': // Сортування за population
                    compare = (records[j].population > records[j + 1].population) ? 1 : -1;
                    break;
                default:
                    printf("Error\n");
                    break;
            }
            if ((sortOrder == 1 && compare > 0) || (sortOrder == 2 && compare < 0)) {
                record temp = records[j];
                records[j] = records[j + 1];
                records[j + 1] = temp;
            }
        }
    }

    // Виведення до і після
    printf("before:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %.2f\n   Population: %.2f\n",
               i + 1, records[i].regionName, records[i].squareArea, records[i].population);
    }

    // Запис відсортованих записів у файл
    file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open file '%s' for writing.\n", filename);
        return;
    }

    fprintf(file, "%s\n", DESCRIPTOR); // Записуємо дескриптор
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record #%d:\n", i + 1);
        fprintf(file, "region: %s\n", records[i].regionName);
        fprintf(file, "area: %.2f\n", records[i].squareArea);
        fprintf(file, "population: %.2f\n", records[i].population);
    }
    fclose(file);

    printf("after:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %.2f\n   Population: %.2f\n",
               i + 1, records[i].regionName, records[i].squareArea, records[i].population);
    }

    printf("Records have been sorted and saved to file '%s'.\n", filename);
}
void editRecordInFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }

    char line[MAX_RECORD_LEN];
    record records[MAX_RECORD_IN_FILE];
    int recordCount = 0;

    // Пропускаємо дескриптор
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File '%s' is empty or cannot be read.\n", filename);
        fclose(file);
        return;
    }

    // Зчитуємо записи
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            // Читаємо запис
            record currentRecord;
            fgets(line, sizeof(line), file); // Читаємо region
            sscanf(line, "region: %s", currentRecord.regionName);
            fgets(line, sizeof(line), file); // Читаємо area
            sscanf(line, "area: %f", &currentRecord.squareArea);
            fgets(line, sizeof(line), file); // Читаємо population
            sscanf(line, "population: %f", &currentRecord.population);

            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);

    if (recordCount == 0) {
        printf("No records found to edit.\n");
        return;
    }

    // Виводимо записи для користувача
    printf("Choose the record you want to edit:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %.2f\n   Population: %.2f\n",
               i + 1, records[i].regionName, records[i].squareArea, records[i].population);
    }

    // Обираємо запис для редагування
    int recordNumber = 0, validInput = 0;
    do {
        printf("Enter the number of the record you want to edit: ");
        validInput = scanf("%d", &recordNumber);
        if (validInput != 1 || recordNumber < 1 || recordNumber > recordCount) {
            printf("Invalid record number.\n");
            fflush(stdin);
        }
    }while(validInput != 1 || recordNumber < 1 || recordNumber > recordCount);

    record *selectedRecord = &records[recordNumber - 1];

    // Редагування полів
    printf("Enter the new region: ");
    scanf("%s", selectedRecord->regionName);
    printf("Enter the new area: ");
    scanf("%f", &selectedRecord->squareArea);
    printf("Enter the new population: ");
    scanf("%f", &selectedRecord->population);

    printf("The record has been successfully changed.\n");

    // Записуємо змінені записи назад у файл
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file '%s' for writing.\n", filename);
        return;
    }

    fprintf(file, "%s\n", DESCRIPTOR); // Записуємо дескриптор
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record #%d:\n", i + 1);
        fprintf(file, "region: %s\n", records[i].regionName);
        fprintf(file, "area: %.2f\n", records[i].squareArea);
        fprintf(file, "population: %.2f\n", records[i].population);
    }
    fclose(file);

    // Виводимо оновлені записи
    printf("Updated records:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %f\n   Population: %f\n",
               i + 1, records[i].regionName, records[i].squareArea, records[i].population);
    }
}
void insertRecordIntoFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file '%s' for reading.\n", filename);
        return;
    }

    char line[MAX_RECORD_LEN];
    record records[MAX_RECORD_IN_FILE];
    int recordCount = 0;

    // Пропускаємо дескриптор
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File '%s' is empty or cannot be read.\n", filename);
        fclose(file);
        return;
    }

    // Зчитуємо записи
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Record :") || strstr(line, "Record #")) {
            record currentRecord;
            fgets(line, sizeof(line), file); // Читаємо region
            sscanf(line, "region: %s", currentRecord.regionName);
            fgets(line, sizeof(line), file); // Читаємо area
            sscanf(line, "area: %f", &currentRecord.squareArea);
            fgets(line, sizeof(line), file); // Читаємо population
            sscanf(line, "population: %f", &currentRecord.population);

            records[recordCount++] = currentRecord;
        }
    }
    fclose(file);

    // Вивід існуючих записів для зручності
    printf("Current records:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %f\n   Population: %f\n",
               i + 1, records[i].regionName, records[i].squareArea, records[i].population);
    }
    int insertPosition;
    printf("Enter the position where you want to insert the new record (1 to %d): ", recordCount);
    scanf("%d", &insertPosition);

    // Перевірка на коректність позиції
    if (insertPosition < 1 || insertPosition > recordCount) {
        printf("Invalid position. Aborting insertion.\n");
        return;
    }
    // Введення нового запису
    printf("Enter the new record you want to insert in %d position\n", recordCount);
    record newRecord;
    printf("Enter the region: ");
    scanf("%s", newRecord.regionName);
    printf("Enter the area: ");
    scanf("%f", &newRecord.squareArea);
    printf("Enter the population: ");
    scanf("%f", &newRecord.population);
    // Вибір позиції для вставки


    // Зсув записів і вставка нового запису
    for (int i = recordCount; i >= insertPosition; i--) {
        records[i] = records[i - 1];
    }
    records[insertPosition - 1] = newRecord;
    recordCount++;

    // Запис оновленого масиву у файл
    file = fopen(filename, "w");
    if (file == NULL) {
        printf("Failed to open file '%s' for writing.\n", filename);
        return;
    }

    fprintf(file, "%s\n", DESCRIPTOR); // Записуємо дескриптор
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "Record #%d:\n", i + 1);
        fprintf(file, "region: %s\n", records[i].regionName);
        fprintf(file, "area: %f\n", records[i].squareArea);
        fprintf(file, "population: %f\n", records[i].population);
    }
    fclose(file);

    // Вивід оновлених записів
    printf("Updated records:\n");
    for (int i = 0; i < recordCount; i++) {
        printf("%d) Region: %s\n   Area: %f\n   Population: %f\n",
               i , records[i].regionName, records[i].squareArea, records[i].population);
    }
}
#endif // FUNCTION_H
