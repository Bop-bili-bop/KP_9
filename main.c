#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "validation.h"
#include "function.h"

#define MAX_REGION_LEN 3

FILE *file;

typedef struct Record
{
    char regionName[MAX_REGION_LEN];
    float squareArea;
    float population;
}record;

int validateProgramFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        return 0;
    }
    char buffer[128] = "";
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return 0;
    }

    // Видаляємо переведення рядка
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }

    fclose(file);
    return strcmp(buffer, DESCRIPTOR) == 0; // Повертає 1, якщо дескриптор збігається
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
            return 0; // Користувач скасував вибір
        }

        if (choice < 1 || choice > fileCount) {
            printf("Invalid choice. Please select a valid number.\n");
        } else {
            break;
        }
    }

    // Копіюємо вибране ім'я файлу
    strcpy(selectedFile, fileList[choice - 1]);
    return 1;
}

int main() {
    char fileName[MAX_FILE_NAME];

    char selectedFile[MAX_FILE_NAME] = "";

    unsigned validFileCount = 0;
    createFile(fileName, fileCount);
    printf("Checking files...\n");
    for (int i = 0; i < fileCount; i++) {
        if (validateProgramFile(fileList[i])) {
            printf("%d. %s (valid)\n", validFileCount + 1, fileList[i]);
            strcpy(fileList[validFileCount], fileList[i]); // Зберігаємо тільки валідні файли
            validFileCount++;
        } else {
            printf("%d. %s (invalid)\n", i + 1, fileList[i]);
        }
    }

    if (validFileCount == 0) {
        printf("No valid program files found.\n");
        return 0;
    }

    // Дозволяємо користувачеві вибрати файл
    if (selectFileFromList(fileList, validFileCount, selectedFile)) {
        printf("You selected file: %s\n", selectedFile);
        // Логіка роботи з обраним файлом
    } else {
        printf("No file selected.\n");
    }

    getch();
    return 0;
}