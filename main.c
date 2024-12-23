#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "function.h"

#define MAX_REGION_LEN 3

FILE *file;

int main() {
    char fileName[MAX_FILE_NAME];
    unsigned fileCount = MAX_FILES;
    char fileList[MAX_FILES][MAX_FILE_NAME] = {};
    record record1 = {"ua", 4.666, 5.5455};
    listExistingDatFiles(fileList, &fileCount);
    chooseFile(fileName);
    readFile(fileName);
    int recordIndex;
    printf("Enter the number of the record to delete (starting from 1): ");
    scanf("%d", &recordIndex);
    recordIndex -= 1;
    deleteRecordFromFile(fileName, recordIndex);
    sortRecordsInFile(fileName, 1, 1);
    readFile(fileName);
    getch();  // Wait for user input before exiting
    return 0;
}
