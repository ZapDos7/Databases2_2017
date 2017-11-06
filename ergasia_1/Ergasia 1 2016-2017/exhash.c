#include "exhash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "BF.h"

#define MAX_RECORDS 2

char * ET_findTypeOfFile(int fd){
    char *ch;
    ch=(char*) malloc(3*sizeof(char));
    void * block;
    BF_ReadBlock(fd,0,&block);
    memcpy(ch,block,3);
    return ch;
}

void init_ET_bucket(int blockNumber,int fileDesc,int depth){
    void * block;
    int numberOfRecords=0;
    
    BF_ReadBlock(fileDesc,blockNumber,&block);
    memcpy(block,&depth,sizeof(int));printf("block number: %d ",blockNumber);
    memcpy(block+sizeof(int),&numberOfRecords,sizeof(int));
    BF_WriteBlock(fileDesc,blockNumber);
}
    

int IndexSize(int depth){       //2^depth
    int i,size;
    size=1;
    for(i=0;i<depth;i++)
        size=size*2;
        return size;
}


int EH_CreateIndex(char *fileName, char* attrName, char attrType, int attrLength, int depth) {
    /* Add your code here */
    int r,fd,i,offset,blockNumber;
    void *block,* indexBlock;
    char  typeOfFile[3];
    strcpy(typeOfFile,"ΕT");

    r=BF_CreateFile(fileName);
     if (r< 0) {
        BF_PrintError("Error creating file");
        r=-1;
        }
    else{                           
            
        fd=BF_OpenFile(fileName);

        /*grafei sto 1o block ta stoixeia pou dinontai san orismata sthn synarthsh*/

        BF_AllocateBlock(fd);                           //desmevei to 1o mplok
        BF_ReadBlock(fd,0,&block);
                
        memcpy( block,typeOfFile,3) ;                       //to eidos arxeiou
        memcpy( block+3,&attrType,1) ;                      //o typos tou kleidiou 'c' h 'i'
        memcpy( block+4,attrName,20) ;                      //to onoma tou kleidiou
        memcpy( block+24,&attrLength,sizeof(int)) ;             //to mhkos tou kleidiou
        memcpy( block+24+sizeof(int),&depth,sizeof(int)) ;          //to arxoko oliko vathos

        BF_WriteBlock(fd,0);
        
        /*ftiaxnei 2o block gia to evrethrio kai to arxikopoiei analoga me to vathos*/

        BF_AllocateBlock(fd);
        BF_ReadBlock(fd,1,&indexBlock);

        
        offset=0;
        int indexSize=IndexSize(depth); //vriskei to size tou evrethriou
        printf("depth is %d and size of index is: %d \n",depth,indexSize);
        for(i=0;i<indexSize;i++){
            BF_AllocateBlock(fd);                   //desmevw ena bucket sto opoio 8a deixnei ka8e 8esh evrethriou 
            blockNumber=BF_GetBlockCounter(fd)-1;           
            init_ET_bucket(blockNumber,fd,depth);           //arxikopoihsh tou bucket (topiko vathos=depth,ari8mos eggrafwn=0)
            memcpy(indexBlock+offset,&blockNumber,sizeof(int)); //sto evrethrio-indexBlock grafw ton ari8mo tou bucket sto opoio deixnei
            offset+=sizeof(int);
        }

        BF_WriteBlock(fd,1);

        BF_CloseFile(fd);
       // printf("TELOS");
    }
    return r;
}


EH_info* EH_OpenIndex(char *fileName) {
    /* Add your code here */
    int anagn=BF_OpenFile(fileName);
    void * block;
    EH_info *info;
    
    if (anagn<0){
        BF_PrintError("Error opening file");
        info=NULL;
    }

    if(strcmp("ET",ET_findTypeOfFile(anagn))!=0){   //an den einai ET file tote ektypwnei la8os
        BF_PrintError("Error not ET file");
        info=NULL;
    }
    else{
        BF_ReadBlock(anagn,0,&block);
        info=(EH_info *) malloc(sizeof(EH_info));
        info->fileDesc=anagn;
        memcpy(&(info->attrType),block+ 3,sizeof(char)); 
        memcpy(info->attrName,block+ 4,20);
        memcpy(&(info->attrLength),block+24,sizeof(int));
        memcpy(&(info->depth),block+24+sizeof(int),sizeof(char));
    }

    return info;
}



int EH_CloseIndex(EH_info* header_info) {
    /* Add your code here */
    int anagn=BF_CloseFile(header_info->fileDesc);
    if (anagn<0){
        BF_PrintError("Error closing file");
        anagn=-1;
    }
    free(header_info);
    return anagn;
}

int EH_InsertEntry(EH_info* header_info, Record record) {
    /* Add your code here */
    int fd,hash;
    int fieldInteger=-1;
    void * block;
    fd=header_info->fileDesc;
    int blockToWrite;

    int available = BF_GetBlockCounter(fd) - 1; //bres poio block einai to available g eggrafi
    if (available < 0) return -1; //check

    printf("total depth is %d\n\n",header_info->depth);
    if (header_info->attrType=='i')
    {
        fieldInteger = record.id;
    }
    else if (header_info->attrType=='c')
    {
        if (strcmp(header_info->attrName, "name")==0)
        {
            fieldInteger = atoi(record.name);
        }
        else if (strcmp(header_info->attrName, "surname")==0)
        {
            fieldInteger = atoi(record.surname);
        }
        else if (strcmp(header_info->attrName, "city")==0)
        {
            fieldInteger = atoi(record.city);
        }
    }    
    hash=hashFunction(fieldInteger,header_info->depth);//vrhkame thn 8esh tou index 
    if (BF_ReadBlock(fd,1,&block)<0)
    {
        return -1;
    }
    memcpy(&blockToWrite,block+hash*sizeof(int),sizeof(int));//vriskei ton ari8mo tou block sto opoio deixnei to index
    WriteToBucket(record,blockToWrite,header_info);
    if (BF_WriteBlock(fd, available) < 0 ) //μετά γράφεις apo to block sto file me fileDesc
    {
        return -1;
    }
}


int EH_GetAllEntries(EH_info header_info, void *value) {
    /* Add your code here 
    int fileDescrip = header_info.fileDesc;
    int bathos = header_info.depth; //bathos
    int megethos = IndexSize(bathos); //megethos index
    for (int i = 1; i<=bathos; i++ ) //gia ka8e depth
    {

    } */
    return -1;

}


int HashStatistics(char* filename) {
/*H HashStatistics τυπώνει τα παρακάτω:
α) Το πόσα blocks έχει ένα αρχείο,
β) Το ελάχιστο, το μέσο και το μέγιστο πλήθος εγγραφών που έχει κάθε bucket ενός αρχείου */
    int fileDesc = BF_OpenFile(filename);
    int n = BF_GetBlockCounter(fileDesc);
    printf("File named %s has %d blocks in it.\n", filename, n); //la9os giati 8eloume OLA oxi mono ta FREE
//    printf("%s\n", ); //xrhsh numofRecs
    return -1;

}

char * decimalToBinary(int n){      //epistrefei to binary me 7 bits
    int i,j,resultOfMod;
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
           resultOfMod = n%2;
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


/*kanei hashing me to 128 kai epistrefei san apotelesma to dekadiko ari8mo pou antistoixei sta prwta depth bits tou apotelesmatos tou hashing*/
int hashFunction(int n,int depth){      
    char *toBinary,*resultOfHash;
    int hash,result;
    hash=n%128;                     //kanei hash me to 128 
    resultOfHash=(char*) malloc(depth * sizeof(char));
    toBinary=decimalToBinary(hash);             
    strncpy(resultOfHash,toBinary,depth);               //pairnei ta depth prwta bits
    
    result=BinaryToDecimal(resultOfHash);
    return result;
    
    }

/* diplasiazei to evrethrio.
Ftiaxnetai enaw pinakas akeraiwn me mege8os diplasio tou evrethriou.
Stis arties 8eseis (i=0,2,4,6 klp)  tou pinaka apo8hkevontai oi palies times tou evrethriou  (ari8moi twn blocks pou deixnei ka8e index) 
kai stis perittes 8eseis apo8hkevontai oi nees times tou evrethrou.
ka8e fora pou ginetai diplasiasmos,ka8e neo index  deixnei sto block pou edeixne to prohgoumeno tou index ,an to block ayto den einai to block sto
opoio dokimas8hke prin na ginei eisagwgh eggrafhs.An to block sto opoio deixnei ena index i einai ayto sto opoio egine h eisagwgh prin tote to neo 
index i+1  deixnei se ena kainourio block.
Sth synexeia apo8hkevetai o pinakas aytos sto block tou evrethriou*/

void doubleIndex(EH_info* header_info,int numberOfChangedBlock){
    int i,j,numberOfBlock;
    void* indexBlock;
    int *indexArray;
    printf("DOUBLE index block\n\n");

    indexArray=(int*)malloc(IndexSize(header_info->depth)*2*sizeof(int));   //se ayton ton pinaka ftiaxnw to kainourio evrethrio
    BF_ReadBlock(header_info->fileDesc,1,&indexBlock);
    i=0;
    j=0;    
    while(j<IndexSize(header_info->depth)){                 //edw gemizetai o pinakas indexArray                        
        memcpy(&numberOfBlock,indexBlock+j*sizeof(int),sizeof(int));    //pairnoume apo to block tou evrethriou ton ari8mo tou block (numberOfBlock)  pou antistoixei sto index i   
        indexArray[i]=numberOfBlock;                    //apo8hkevetai h prohgoumenh  timh numberOfBlock sthn 8esh i tou pinaka             
        if(indexArray[i]==numberOfChangedBlock){            //an to block einai ayto sto opoio h eisagwgh den petyxe prin   
            BF_AllocateBlock(header_info->fileDesc);        //desmevetai neo block  
            indexArray[i+1]=BF_GetBlockCounter(header_info->fileDesc)-1;//apo8hkevetai o ari8mos tou block pou desmeythke sthn 8esh i+1
            init_ET_bucket(indexArray[i+1],header_info->fileDesc,header_info->depth+1);
        }
        else{
            indexArray[i+1]=numberOfBlock;          //sthn 8esh i+1 apo8hkevetai h timh numberOfBlock
        }
        i+=2;
        j++;
    }

    header_info->depth++;                   //ayksanetai to depth
    for(i=0;i<IndexSize(header_info->depth);i++)        //apo8hkevetai o pinakas sto block tou evrethriou       
        memcpy(indexBlock+i*sizeof(int),&(indexArray[i]),sizeof(int));
        
    BF_WriteBlock(header_info->fileDesc,1);
    
}

/*grafei sto katallhlo bucket thn eggrafh.
An xwraei thn pros8etei sto telos.
Alliws an to bucket einai gemato kai to depth tou bucket einai mikrotero tou olikou tote shmainei oti yparxoun kai alla indexes pou
deixnoun se ayto to bucket.Opote,ta nea indexes 8a deixnoun se nea buckets kai oi eggrafes tou prohgoumenou bucket  anakatanemontai 
kai sto telos prosti8etai ksana h eggrafh 
Alliws an to bucket einai gemato kai to depth tou bucket einai iso tou olikou depth tote shmainei oti den yparxoun kai alla indexes pou
deixnoun se ayto to bucket,opote to evrethrio prepei na diplasiastei.Afou diplasiastei anakatanemontai oi eggrafes tou bucket kai
prosti8etai ksana h eggrafh*/

void WriteToBucket(Record rec,int numberOfBlock,EH_info* header_info){
    void * bucket;
    int i;
    Record records[MAX_RECORDS];
    int numberOfRecords, depthOfBucket,offset;
    

    /* diavazei tis arxikes plhrofories tou bucket( depth kai number of records)*/

    BF_ReadBlock(header_info->fileDesc,numberOfBlock,&bucket);
    memcpy(&depthOfBucket,bucket,sizeof(int));
    memcpy(&numberOfRecords,bucket+sizeof(int),sizeof(int));
    printf("Bucket with number %d has depth   %d and total depth is %d and number of records is %d \n", numberOfBlock,depthOfBucket,header_info->depth,numberOfRecords);
    if(numberOfRecords<MAX_RECORDS){                //an to record xwraei na mpei sto bucket
        offset=2*sizeof(int)+numberOfRecords*sizeof(Record);
        memcpy(bucket+offset,&rec,sizeof(Record));
        numberOfRecords++;
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);
        printf("Record inserted at bucket with number %d\n \n", numberOfBlock);


    }


    /*An to bucket einai gemato kai o topikos deikths einai mikroteros tou synolikou,tote ayto shmainei pws yparxoun >1 deiktes-indexes pou deixnoun se ayto
    to bucket.Opote desmevw kainouria buckets gia tous deiktes aytous kai to va8os ginetai oso to header_info->depth.Kanw anakatanomh twn eggrafwn aytou tou
     bucket kai telos pros8etw to record */
        

    else if(depthOfBucket<header_info->depth){
        printf("2h synthiki");  getchar();
        for(i=0;i<MAX_RECORDS;i++){         //pairnw oles tis eggrafes aytou tou bucket
            offset=2*sizeof(int)+i*sizeof(Record);
            memcpy(&(records[i]),bucket+offset,sizeof(Record));
        }
        
        allocateBuckets(header_info,numberOfBlock);     // vriskei ola ta indexes se ayto to bucket kai desmeysh gia to ka8e ena kainourio bucket
        numberOfRecords=0;
        depthOfBucket=header_info->depth;           
        memcpy(bucket,&depthOfBucket,sizeof(int));        //allazw to depth tou bucket=header_info->depth
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));  //to plh8os twn eggrafwn pou exei ayto to bucket ginetai 0
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);
        printf("Bucket with number %d is full, depth of bucket is %d and total depth is %d\n", numberOfBlock,depthOfBucket,header_info->depth);//getchar();
        printf("Indexes that point at this bucket will point to new buckets\n\n");
        for(i=0;i<MAX_RECORDS;i++)                //ksanakanw insert tis eggrafes aytou tou busket
            EH_InsertEntry(header_info,records[i]);
        EH_InsertEntry(header_info,rec);            //ksanadokimazw na kanw insert to rec

    }
    
    //Diaforetika an to bucket einai gemato kai  o topikos deikths einai idios me ton arxiko tote kanw double
    else if(depthOfBucket==header_info->depth){
        printf("Bucket with number %d is full, depth of bucket is %d and total depth is %d\n", numberOfBlock,depthOfBucket,header_info->depth);
        doubleIndex(header_info,numberOfBlock);             //ginetai diplasiasmos pinaka kai allocate new block sthn 8esh pou prepei na mpoun oi nees eggrafes
        for(i=0;i<MAX_RECORDS;i++){                             
            offset=2*sizeof(int)+i*sizeof(Record);
            memcpy(&(records[i]),bucket+offset,sizeof(Record)); //pairnw oles tis eggrafes aytou tou bucket
        }
        numberOfRecords=0;
        depthOfBucket=header_info->depth;
        memcpy(bucket,&depthOfBucket,sizeof(int));          //to depth tou bucket ginetai oso to header_info->depth
        memcpy(bucket+sizeof(int),&numberOfRecords,sizeof(int));    //to plh8os twn eggrafwn pou exei ayto to bucket ginetai 0
        BF_WriteBlock(header_info->fileDesc,numberOfBlock);
        

        for(i=0;i<MAX_RECORDS;i++)
            EH_InsertEntry(header_info,records[i]);         //ksanakanw insert tis eggrafes aytou tou busket

        EH_InsertEntry(header_info,rec);                //ksanadokimazw na kanw insert to rec
    }
}

/*desmevei buckets stis 8eseis tou evrethriou pou prin edeixnan se kapoio allo gemato bucket*/
void allocateBuckets(EH_info* header_info,int numberOfChangedBlock){    
    int j,numberOfBlock;
    void* indexBlock;
    BF_ReadBlock(header_info->fileDesc,1,&indexBlock);

    j=0;
    while(j<IndexSize(header_info->depth)){
        memcpy(&numberOfBlock,indexBlock+j*sizeof(int),sizeof(int));
        if(numberOfBlock==numberOfChangedBlock){
            BF_AllocateBlock(header_info->fileDesc);    
            numberOfBlock=BF_GetBlockCounter(header_info->fileDesc)-1;
            init_ET_bucket(numberOfBlock,header_info->fileDesc,header_info->depth);
            memcpy(indexBlock+j*sizeof(int),&numberOfBlock,sizeof(int));
            printf("for index %d we allocate new bucket with number %d \n",j,numberOfBlock);
        }
        j++;
    }
    BF_WriteBlock(header_info->fileDesc,1);
}
