// Record struct and functions
#ifndef AUXILIARY
#define AUXILIARY



typedef struct {
    int id;
    char name[15];
    char surname[20];
    char city[25];

}Record;

Record* newRecord(int id, char *name[15], char surname[20], char city[25]);

void printRecord(Record record);

// Info struct and functions

typedef struct {
    int type;                           /* ο τύπος του αρχείου: 0 αταξινόμητο,1 ταξινομημένο */
    int fileDesc;          /* αναγνωριστικός αριθμός ανοίγματος αρχείου από το επίπεδο block */
    int fieldNo;                 /* αριθμός του πεδίου βάσει του οποίου γίνεται η ταξινόμηση */
} ES_info;

ES_info* newES_info( int type, int fileDesc, int fieldNo);

void printES_info(ES_info* FileInfo);

// Block info struct and functions

typedef struct {
    int entries;                 /* αριθμός εγγραφών που έχουν εισαχθεί στο μπλοκ */
} block_info;

block_info* new_block_info();

// Other auxiliary functions

int Unsorted_OpenFile(char* fileName);    /* ανοίγει οποιοδήποτε αρχείο */

int Block_BubbleSort(void* block1, int field); // 0 error sorting, 1 successful sort

char* File_MergeSort(char* filename1, char* filename2, int field); // 0 error sorting, 1 successful sort

void PrintRecordsOfFile(int fileDesc);

#endif
