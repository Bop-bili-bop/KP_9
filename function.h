#ifndef FUNCTION_H
#define FUNCTION_H
#include "validation.h"
#define MAX_FILE_NAME 32
#define MAX_FILES 10
#define DESCRIPTOR "YWhhbGFkemVuaWdnZXJkb250YnJlYWtteXByb2dyYW0="
unsigned fileCount = MAX_FILES;
char fileList[MAX_FILES][MAX_FILE_NAME] = {"ffff.dat"};
int addFileToList(char fileList[][MAX_FILE_NAME], unsigned *fileCount, const char *fileName) {
    if (*fileCount >= MAX_FILES) {
        printf("Error: Maximum number of files reached.\n");
        return 0;
    }
    strcpy(fileList[*fileCount], fileName);
    (*fileCount)++;
    return 1;
}

void createFile(char *fileName, unsigned *fileCount ) {
    unsigned errorCatch = 0;
    do {
        errorCatch = 0;
        validateInputFileName(fileName);
        printf("Validated file name: '%s'\n", fileName);
        if (strlen(fileName) + 4 < MAX_FILE_NAME)
        {
            strcat(fileName, ".dat");
        }
        else
        {
            printf("Error: File name is too long to append extension.\n");
            errorCatch = 1;
        }
        FILE *file = fopen(fileName, "w");
        if (file == NULL)
        {
            printf("Error! File wasn't created.\n");
            errorCatch = 1;
        }
        printf("File created successfully with name %s\n", fileName);
        fprintf(file, "%s\n", DESCRIPTOR);
        fclose(file);
        if (addFileToList(fileList, fileCount, fileName)) {
            printf("File created successfully with name %s\n", fileName);
            errorCatch = 0;
        }
        else {
            errorCatch = 1;
        }
    } while (errorCatch != 0);

}
#endif //FUNCTION_H
