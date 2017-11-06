#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

#include "Sorted.h"
#include "BF.h"
#include "Auxiliary.h"

#define fileName "HeapFile"

void insert_Entries(int filedesc) {

    FILE *stream;
    printf("Inserting Entries\n");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    stream = fopen("./datasets/1000.csv","r");
    printf("csv open: %s\n", (char *) stream);
    Record record;



    while ((read = getline(&line, &len, stream)) != -1) {
        line[read - 2] = 0;
        char *pch;

        pch = strtok(line, ",");
        record.id = atoi(pch);

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.name, pch, sizeof(record.name));

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.surname, pch, sizeof(record.surname));

        pch = strtok(NULL, ",");
        pch++;
        pch[strlen(pch) - 1] = 0;
        strncpy(record.city, pch, sizeof(record.city));



        assert(!Sorted_InsertEntry(filedesc, record));
    }
    free(line);
    fclose(stream);
    printf("dataset closed\n");
}


int main(int argc, char **argv) {

    int fd;
    int fieldNo=0;

    BF_Init();

    if(Sorted_CreateFile(fileName) == -1  )
        printf("create file failed\n");

    fd = Unsorted_OpenFile(fileName);
    if( fd == -1  )
        printf("open file failed\n");
    insert_Entries(fd);

    Sorted_CloseFile(fd);


    Sorted_checkSortedFile(fileName,fieldNo);


    Sorted_SortFile(fileName,fieldNo);


     char name[20];
     strcpy(name,"HeapFileSorted");


     if(Sorted_checkSortedFile(fileName,fieldNo) == -1  )printf("file not sorted by field %d!\n",fieldNo);
   //
   //
  //   Sorted_checkSortedFile(name,3);
  //   //get all entries with value
  //   char value[20];
  //   strcpy(value, "Keratsini");
  //   //fieldNo = 0;
  //   //int value = 11903588;
   //
  //   fd = Sorted_OpenFile(name);
  //   if( fd == -1  )
  //       printf("Error opening file!\n");
   //
  //  Sorted_GetAllEntries(fd,fieldNo,&value);
   //

printf("end of main\n");

    return EXIT_SUCCESS;
}
