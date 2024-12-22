#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>  // for strcpy, strcat
#include "validation.h"
#include "function.h"

#define MAX_REGION_LEN 3

FILE *file;


int main() {
    char fileName[MAX_FILE_NAME];
    unsigned fileCount = 0; // Ensure fileCount is initialized

    char selectedFile[MAX_FILE_NAME] = "";

    unsigned validFileCount = 0;
    createFile(fileName, &fileCount);  // Pass address of fileCount
    printf("Checking files...\n");
    for (int i = 0; i < fileCount; i++) {
        if (validateProgramFile(fileList[i])) {
            printf("%d. %s (valid)\n", validFileCount + 1, fileList[i]);
            strcpy(fileList[validFileCount], fileList[i]); // Store only valid files
            validFileCount++;
        } else {
            printf("%d. %s (invalid)\n", i + 1, fileList[i]);
        }
    }

    if (validFileCount == 0) {
        printf("No valid program files found.\n");
        return 0;
    }

    // Allow user to select a file
    if (selectFileFromList(fileList, validFileCount, selectedFile)) {
        printf("You selected file: %s\n", selectedFile);

        record newRecord = {
            "NYC",       // Назва регіону
            789.45,      // Площа
            8000000.0    // Населення
        };
        if (addRecordToFile(fileName, &newRecord)) {
            printf("New record added successfully!\n");
        } else {
            printf("Failed to add new record.\n");
        }
        printf("1 to read 2 to delete");
        char choice = getch();
        printf("%c\n", choice);
        switch (choice) {
            case '1':
                printf("Read file");
                readFile(fileName);
                break;
            case '2':
                printf("Delete file");
                if (deleteFile(fileName)) {
                    printf("The file was successfully deleted.\n");
                }
                else {
                    printf("Failed to delete the file.\n");
                }
                break;
            default:
                printf("Invalid choice. Please select a valid number.\n");
                break;
        }
    } else {
        printf("No file selected.\n");
    }
    getch();  // Wait for user input before exiting
    return 0;
}
