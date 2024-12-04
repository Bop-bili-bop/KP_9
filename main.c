#include <stdio.h>
#include <conio.h>
#include "validation.h"
#define MENU_VISUAL "MENU"
#define FILE_MODE_VISUAL "File mode:"
#define RECORDS_MODE_VISUAL "Records mode:"
#define CREATE_FILE_VISUAL "0 - create file"
#define READ_FILE_VISUAL "1 - read file"
#define WRITE_FILE_VISUAL "2 - delete file"
#define CREATE_RECORD_VISUAL "3 - create record"
#define READ_RECORD_VISUAL "4 - read record"
#define EDIT_RECORD_VISUAL "5 - edit record"
#define SORT_RECORD_VISUAL "6 - sort record"
#define PASTE_RECORD_VISUAL "7 - paste record"
#define DELETE_RECORD_VISUAL "8 - delete record"
#define EXIT_PROGRAM_VISUAL "9 - exit program"
#define INSPECT_FILE_HEADER "10 - inspect file header"


typedef struct record {
    char region[40];
    int population;
    float area;
}record;

FILE *file;

int main()
{
    const int menu_options = 10;
    char *menu[] = {
        CREATE_FILE_VISUAL,
        READ_FILE_VISUAL,
        WRITE_FILE_VISUAL,
        CREATE_RECORD_VISUAL,
        READ_RECORD_VISUAL,
        EDIT_RECORD_VISUAL,
        SORT_RECORD_VISUAL,
        PASTE_RECORD_VISUAL,
        DELETE_RECORD_VISUAL,
        EXIT_PROGRAM_VISUAL
    };
    char* file_name = "nigger.txt";
    char menu_choice = 0;
    printf("%25s\n", MENU_VISUAL);
    printf("%s", FILE_MODE_VISUAL);
    printf("%40s\n", RECORDS_MODE_VISUAL);
    for (int i = 0; i < menu_options; i++) {
        if (i < 3) {
            // Print file mode options in the left column
            printf("%-37s", menu[i]);
            // Pair with records mode option if it exists
            if (i + 3 < menu_options) {
                printf("%s\n", menu[i + 3]);
            } else {
                printf("\n"); // If no right-column option exists, move to the next line
            }
        } else if (i >= 9) {
            // Handle additional options (9 and 10) separately
            printf("%s\n", menu[i]);
        }
    }
    menu_choice = (char) getch();
    switch (menu_choice) {
        case '0':
            file = fopen(file_name, "w");
            break;
        case '1':
            break;
        case '2':
            break;
        case '3':
            break;
        case '4':
            break;
        case '5':
            break;
        case '6':
            break;
        case '7':
            break;
        case '8':
            break;
        default:
            printf("Invalid choice\n");
            break;
    }
    getch();
    return 0;
}