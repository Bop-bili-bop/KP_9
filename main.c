#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "function.h"

#define MAX_REGION_LEN 3
#define ESC 27

FILE *file;

int main() {
    char fileName[MAX_FILE_NAME];
    char choice = 0;
    do {
        printMenuOptions();
        choice = getch();
        switch (choice) {
            case MENU_KEY_CHOOSE_FILE:
                chooseFile(fileName);
                break;
            case MENU_KEY_CREATE_FILE:
                createNewFileByName(fileName);
                break;
            case MENU_KEY_OPEN_FILE:
                readFile(fileName);
                break;
            case MENU_KEY_DELETE_FILE:
                deleteFile(fileName);//logic??
                break;
            case MENU_KEY_CREATE_RECORD:
                writeRecordsToFile(fileName);
                break;
            case MENU_KEY_READ_RECORD:
                char readRecordChoice = 0;
                do {
                    printf("Press 1 to read all records in %s\n"
                           "Press 2 to read record with specific index in %s\n"
                           "Press any other key to exit this menu\n", fileName, fileName);
                    readRecordChoice = getch();
                    switch (readRecordChoice) {
                        case '1':
                            readAllRecords(fileName);
                            break;
                        case '2':
                            readSpecificRecord(fileName);
                            break;
                        default:
                            printf("Exiting...\n");
                            readRecordChoice = 0;
                            break;
                    }
                }while (readRecordChoice != 0);
                break;
            case MENU_KEY_EDIT_RECORD:

                break;
            case MENU_KEY_SORT_RECORD:

                break;
            case MENU_KEY_INSERT_RECORD:

                break;
            case MENU_KEY_DELETE_RECORD:
                deleteRecord(fileName);
                break;
            case ESC:
                printf("Exiting program...\n");
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    } while (choice != ESC);
    return 0;
}
