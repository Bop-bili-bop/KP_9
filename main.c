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
    /*unsigned fileCount = MAX_FILES;
    char fileList[MAX_FILES][MAX_FILE_NAME] = {};*/
    record recordsList[4] = {
        {"ua", 4.666, 5.5455},
        {"uk", 3323.4, 5.444}
    };
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
                deleteFile(fileName);
                break;
            case MENU_KEY_CREATE_RECORD:

                break;
            case MENU_KEY_READ_RECORD:

                break;
            case MENU_KEY_EDIT_RECORD:

                break;
            case MENU_KEY_SORT_RECORD:

                break;
            case MENU_KEY_INSERT_RECORD:

                break;
            case MENU_KEY_DELETE_RECORD:

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
