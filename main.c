#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "function.h"

#define MAX_REGION_LEN 3
#define ESC 27

FILE *file;

int main() {
    char fileName[MAX_FILE_NAME] = "";
    char choice = 0;
    printf("This is a file manager.\nAt first you should choose a file you want to work with or create one\n");
    do {
        printMenuOptions();
        choice = getch();
        switch (choice) {
            case MENU_KEY_CHOOSE_FILE:
                system("cls");
                chooseFile(fileName);
                break;
            case MENU_KEY_CREATE_FILE:
                system("cls");
                createNewFileByName(fileName);
                break;
            case MENU_KEY_OPEN_FILE:
                system("cls");
                readFile(fileName);
                break;
            case MENU_KEY_DELETE_FILE:
                system("cls");
                deleteFile(fileName);
                break;
            case MENU_KEY_CREATE_RECORD:
                system("cls");
                writeRecordsToFile(fileName);
                break;
            case MENU_KEY_READ_RECORD:
                system("cls");
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
                system("cls");
                editRecordInFile(fileName);
                break;
            case MENU_KEY_SORT_RECORD:
                system("cls");
                readAllRecords(fileName);
                sortRecordsInFile(fileName);
                break;
            case MENU_KEY_INSERT_RECORD:
                system("cls");
                insertRecordIntoFile(fileName);
                break;
            case MENU_KEY_DELETE_RECORD:
                system("cls");
                deleteRecord(fileName);
                break;
            case ESC:
                system("cls");
                break;
            default:
                system("cls");
                printf("Invalid choice\n");
                break;
        }
    } while (choice != ESC);
    return 0;
}
