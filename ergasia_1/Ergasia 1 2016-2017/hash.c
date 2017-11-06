#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "BF.h"
#include "hash.h"

#define MAX_RECORDS 2

char * HT_findTypeOfFile(int fd){
    char *ch;
    ch=(char*) malloc(3*sizeof(char));
    void * block;
    BF_ReadBlock(fd,0,&block);
    memcpy(ch,block,3);
    return ch;
}

void init_HT_bucket(int blockNumber,int fileDesc,int numOfBuckets){
    void * block;
    int numberOfRecords=0;
    
    BF_ReadBlock(fileDesc,blockNumber,&block);
    memcpy(block,&numOfBuckets,sizeof(int));printf("block number: %d ",blockNumber);
    memcpy(block+sizeof(int),&numberOfRecords,sizeof(int));
    BF_WriteBlock(fileDesc,blockNumber);
}

        
/*grafei sto katallhlo bucket thn eggrafh.
An xwraei thn pros8etei sto telos.
Alliws overflow buckets*/
void WriteToBucket(Record rec,int numberOfBlock,HT_info* header_info){
    void * bucket;
    int i;
    Record records[MAX_RECORDS];
    int numberOfRecords, numOfBuckets,offset;
    
    /* diavazei tis arxikes plhrofories tou bucket( depth kai number of records)*/

    BF_ReadBlock(header_info->fileDesc,numberOfBlock,&bucket);
    memcpy(&numOfBuckets,bucket,sizeof(int));
    memcpy(&numberOfRecords,bucket+sizeof(int),sizeof(int));
    printf("Bucket with number %d has %d records.\n", numberOfBlock,numberOfRecords);
    if(numberOfRecords<MAX_RECORDS){                //an to record xwraei na mpei sto bucket
        offset=2*sizeof(int)+numberOfRecords*sizeof(Record);
        memcpy(bucket+offset,&rec,sizeof(Record));
        numberOfRecords++;
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);
        printf("Record inserted at bucket with number %d\n \n", numberOfBlock);
    }

    /*An to bucket einai gemato kai o topikos deikths einai mikroteros tou synolikou,tote ayto shmainei pws yparxoun >1 deiktes-indexes pou deixnoun se ayto
    to bucket.Opote desmevw kainouria buckets gia tous deiktes aytous.Kanw anakatanomh twn eggrafwn aytou tou bucket kai telos pros8etw to record */

    else if(numOfBuckets<header_info->numBuckets){
        printf("2h synthiki");  getchar();
        for(i=0;i<MAX_RECORDS;i++){         //pairnw oles tis eggrafes aytou tou bucket
            offset=2*sizeof(int)+i*sizeof(Record);
            memcpy(&(records[i]),bucket+offset,sizeof(Record));
        }
        
        allocateBuckets(header_info,numberOfBlock);     // vriskei ola ta indexes se ayto to bucket kai desmeysh gia to ka8e ena kainourio bucket
        numberOfRecords=0;
        numOfBuckets=header_info->numBuckets;           
        memcpy(bucket,&numOfBuckets,sizeof(int));        //allazw
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));  //to plh8os twn eggrafwn pou exei ayto to bucket ginetai 0
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);
        printf("Bucket with number %d is full.\n", numberOfBlock);//getchar();
        printf("Indexes that point at this bucket will point to new buckets\n\n");
        for(i=0;i<MAX_RECORDS;i++)                //ksanakanw insert tis eggrafes aytou tou busket
            HT_InsertEntry(*header_info,records[i]);
        HT_InsertEntry(*header_info,rec);            //ksanadokimazw na kanw insert to rec

    }
    
    //Diaforetika an to bucket einai gemato kai  o topikos deikths einai idios me ton arxiko tote kanw double
    else if(numOfBuckets==header_info->numBuckets){
        printf("Bucket with number %d is full\n", numberOfBlock);
        allocateBuckets(header_info,numberOfBlock);             //ginetai diplasiasmos pinaka kai allocate new block sthn 8esh pou prepei na mpoun oi nees eggrafes
        for(i=0;i<MAX_RECORDS;i++){                             
            offset=2*sizeof(int)+i*sizeof(Record);
            memcpy(&(records[i]),bucket+offset,sizeof(Record)); //pairnw oles tis eggrafes aytou tou bucket
        }
        numberOfRecords=0;
        numOfBuckets=header_info->numBuckets;
        memcpy(bucket,&numOfBuckets,sizeof(int));
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));    //to plh8os twn eggrafwn pou exei ayto to bucket ginetai 0
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);

        for(i=0;i<MAX_RECORDS;i++)
        {
            HT_InsertEntry(*header_info,records[i]);         //ksanakanw insert tis eggrafes aytou tou busket
        }
        HT_InsertEntry(*header_info,rec);                //ksanadokimazw na kanw insert to rec
    }
}

int HT_CreateIndex( char *fileName, char attrType, char* attrName, int attrLength, int buckets) {
    void *block,* indexBlock;
    int i,offset;
    char  typeOfFile[3];//'HT\n'
    if (BF_CreateFile(fileName) < 0 ) //
    {
        BF_PrintError("Error creating file");
        return -1; // if error return -1;
    }

    int fileDesc = BF_OpenFile(fileName); //anagnwristiko arxeiou
    if (fileDesc<0)
    {
        BF_PrintError("Error opening file");
        return -1; // if error return -1;
    }

    if (BF_AllocateBlock(fileDesc) < 0 ); //header of bucket (to special) is allocated and = 0 and exei mege8os BLOCK_SIZE and is sto telos tou arxeiou =>
    {
        BF_PrintError("Error allocating block");
        return -1; // if error return -1;
    }

    int block_number=BF_GetBlockCounter(fileDesc)-1; //o ari8mos tou einai BF_getBlockCounter(fileDesc) - 1.
    if (block_number < 0)
    {
        BF_PrintError("Error on block number");
        return -1; // if error return -1;
    }

    if (BF_ReadBlock(fileDesc, 0, &block) < 0 ) //0 alla meta lew block_number, kanonika prepei block_number = 0 => ok
    {
        BF_PrintError("Error reading block");
        return -1;// if error return -1 & διαβασε το 1ο μπλοκ τ αρχειου
    }

        memcpy( block,typeOfFile,3) ;                       //to eidos arxeiou
        memcpy( block+3,&attrType,1) ;                      //o typos tou kleidiou 'c' h 'i'
        memcpy( block+4,attrName,20) ;                      //to onoma tou kleidiou
        memcpy( block+24,&attrLength,sizeof(int)) ;             //to mhkos tou kleidiou
        memcpy( block+24+sizeof(int),&buckets,sizeof(int)) ;          //to arxoko oliko vathos


    if (BF_WriteBlock(fileDesc, block_number) < 0 ) //γραψε στο αρχειο τα περιεχ του 0 (πρώτου) μπλοκ!, block_num=0
    {
        BF_PrintError("Error writing block");
        return -1; 
    }

    if (BF_AllocateBlock(fileDesc) < 0 )
    {
        BF_PrintError("Error allocating block");
        return -1; // if error return -1;
    }
    if (BF_ReadBlock(fileDesc, i, &block) < 0 ) /*ftiaxnei 2o block gia to evrethrio */
    {
        BF_PrintError("Error reading block");
        return -1; // if error return -1;
    }

    offset=0;

    for (int i = 1; i<=buckets; i++) //gia ka8e bucket allocate + readblock gia na tou bazw times
    {
        printf("%d \n", i); //elegxos aplws

        if (BF_AllocateBlock(fileDesc) < 0 )
        {
            BF_PrintError("Error allocating block");
            return -1; // if error return -1;
            break;
        }

        block_number=BF_GetBlockCounter(fileDesc)-1;

        init_HT_bucket(block_number,fileDesc, buckets); //arxikopoihsh tou bucket
        memcpy(indexBlock+offset,&block_number,sizeof(int)); //sto evrethrio-indexBlock grafw ton ari8mo tou bucket sto opoio deixnei
        offset+=sizeof(int);
    }
    if (BF_WriteBlock(fileDesc,i + 1) < 0 )
    {
        BF_PrintError("Error writing block");
        return -1; // if error return -1;
    }
    if (BF_CloseFile(fileDesc) < 0 );
    {
        BF_PrintError("Error closing file");
        return -1; // if error return -1;
    }
    return 0;
}

HT_info* HT_OpenIndex(char *fileName) {//h OpenIndex anoigei to arxeio k grafei ekei & update info
    int desc = BF_OpenFile(fileName);
    void * block;
    HT_info *information;
    if (desc < 0) 
    {
        BF_PrintError("Error opening file");
        information = NULL;
    }

    if(strcmp("HT",HT_findTypeOfFile(desc))!=0){   //an den einai HT file tote ektypwnei la8os
        BF_PrintError("Error not HT file"); //Αν το αρχείο που δόθηκε για άνοιγμα δεν αφορά αρχείο κατακερματισμού, τότε αυτό επίσης θεωρείται σφάλμα.
        return NULL;
    }
    if (BF_ReadBlock(desc, 1, &block) < 0 )
        {
            information = NULL;
        }
    else
    {
        BF_ReadBlock(desc,0,&block);
        information=(HT_info *) malloc(sizeof(HT_info));
        information->fileDesc=desc;
        memcpy(&(information->attrType),block+ 3,sizeof(char)); 
        memcpy(information->attrName,block+ 4,20);
        memcpy(&(information->attrLength),block+24,sizeof(int));
        memcpy(&(information->numBuckets),block+24+sizeof(int),sizeof(char));
    }
    return information;
}

int HT_CloseIndex( HT_info* header_info ) { 
    int a = BF_CloseFile(header_info->fileDesc); 
    if (a == 0)
    {
        free(header_info);
        return a;
    }
    else
    {
        BF_PrintError("Error closing file");
        return -1;
    }
}

int HT_InsertEntry(HT_info header_info, Record record) { //hash=>πού πρέπει να γράψεις, check overflow & ό,τι αυξήσεις/επεκτάσεις χρειάζεται
    /* Add your code here */
    void * block;
    int blockToWrite;
    int fd = header_info.fileDesc;
    int hash;
    int available = BF_GetBlockCounter(fd) - 1; //bres poio block einai to available g eggrafi
    if (available < 0) return -1; //check
    int fieldInteger=-1;

    printf("total number of buckets is %ld\n\n",header_info.numBuckets);
    if (header_info.attrType=='i')
    {
        fieldInteger = record.id;
    }
    else if (header_info.attrType=='c')
    {
        if (strcmp(header_info.attrName, "name")==0)
        {
            fieldInteger = atoi(record.name);
        }
        else if (strcmp(header_info.attrName, "surname")==0)
        {
            fieldInteger = atoi(record.surname);
        }
        else if (strcmp(header_info.attrName, "city")==0)
        {
            fieldInteger = atoi(record.city);
        }
    }
    hash=hashFunction(fieldInteger,header_info.numBuckets);//vrhkame thn 8esh tou index 
    if (BF_ReadBlock(header_info.fileDesc, available, &block) < 0 )
    {
        return -1; //read said block me #available
    }
    memcpy(&blockToWrite,block+hash*sizeof(int),sizeof(int));//vriskei ton ari8mo tou block sto opoio deixnei to index
    WriteToBucket(record,blockToWrite,&header_info);
    if (BF_WriteBlock(header_info.fileDesc, available) < 0 ) //μετά γράφεις apo to block sto file me fileDesc
    {
        return -1;
    }

    return 0;
}

int HT_GetAllEntries(HT_info header_info, void *value) {
    /* Add your code here */

    return -1;

}


int HashStatistics(char* filename) {
    /* Add your code here 

    η HashStatistics τυπώνει τα παρακάτω:
α) Το πόσα blocks έχει ένα αρχείο,
β) Το ελάχιστο, το μέσο και το μέγιστο πλήθος εγγραφών που έχει κάθε bucket ενός αρχείου,
γ) Το μέσο αριθμό των blocks που έχει κάθε bucket, και
δ) Το πλήθος των buckets που έχουν μπλοκ υπερχείλισης, και πόσα μπλοκ είναι αυτά για κάθε bucket.
    int fileDesc = BF_OpenFile(filename);
    int n = BF_GetBlockCounter(fileDesc);
    printf("File named %s has %d blocks in it.\n", filename, n); //la9os giati 8eloume OLA oxi mono ta FREE
//    printf("%s\n", ); //xrhsh numofRecs */
    return -1;
    
}

char * decimalToBinary(int n){      //epistrefei to binary me 7 bits
    int i,j;
    char * toBinary;
    toBinary=(char*) malloc(8 * sizeof(char));
    
    if(n==0){           //an einai 0 epistrefei 7 mhdenika bits
        strcpy(toBinary,"0000000");
        return toBinary;
    }
    i=6;
    while(n>0)          //vriskei ta bits kai ta topo8etei ston pinaka apo to telos pros thn arxh
        {
           toBinary[i]=(char) (((int)'0')+ n % 2  );
           i--;
           n=n/2;
        }
    toBinary[7]='\0';
    j=0;
    while(j<=i){            //gemizei me mhdenika sthn arxh ton pinaka 
        toBinary[j]='0';
        j++;
    }
    return toBinary;
}

int BinaryToDecimal(char * binary){
    int i,number,power;
    number=0;
    power=1;
    i=strlen(binary)-1;
    while(i>=0){
        number+=(((int)(binary[i])-'0') * power);
        power=power * 2;
        i--;
    }
    return number;
}

/*kanei hashing me to 128 kai epistrefei san apotelesma to dekadiko ari8mo pou antistoixei sta prwta buckets bits tou apotelesmatos tou hashing*/
int hashFunction(int n,int numOfBuckets){      
    char *toBinary,*resultOfHash;
    int hash,result;
    hash=n%128;                     //kanei hash me to 128 
    resultOfHash=(char*) malloc(numOfBuckets * sizeof(char));
    toBinary=decimalToBinary(hash);             
    strncpy(resultOfHash,toBinary,numOfBuckets); //pairnei ta buckets prwta bits
    result=BinaryToDecimal(resultOfHash);
    return result;
    }

/*desmevei buckets stis 8eseis tou evrethriou pou prin edeixnan se kapoio allo gemato bucket*/
void allocateBuckets(HT_info* header_info,int numberOfChangedBlock){
    int j,numberOfBlock;
    void* indexBlock;
    BF_ReadBlock(header_info->fileDesc,1,&indexBlock);
    j=0;
    while(j<header_info->numBuckets){
        memcpy(&numberOfBlock,indexBlock+j*sizeof(int),sizeof(int));
        if(numberOfBlock==numberOfChangedBlock){
            BF_AllocateBlock(header_info->fileDesc);    
            numberOfBlock=BF_GetBlockCounter(header_info->fileDesc)-1;
            init_HT_bucket(numberOfBlock,header_info->fileDesc,header_info->numBuckets);
            memcpy(indexBlock+j*sizeof(int),&numberOfBlock,sizeof(int));
            printf("for index %d we allocate new bucket with number %d \n",j,numberOfBlock);
        }
        j++;
    }
    BF_WriteBlock(header_info->fileDesc,1);
}
