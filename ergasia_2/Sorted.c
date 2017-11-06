#include "Sorted.h"
#include "Auxiliary.h"
#include "BF.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int Sorted_CreateFile( char *fileName )
{

    if(BF_CreateFile(fileName)<0){
      BF_PrintError("create file failed\n");
      return -1;
    }

    int fd=BF_OpenFile(fileName);
    if(fd<0){
      BF_PrintError("open file failed\n");
      return -1;
    }

    ES_info* Info=newES_info(0,-1,0);
    void* block;
    if(BF_AllocateBlock(fd)<0){
      BF_PrintError("allocate block failed\n");
      return -1;
    }
    if(BF_ReadBlock(fd,0,&block)<0){
      BF_PrintError("read block failed\n");
      return -1;
    }
    memcpy(block,Info,sizeof(ES_info));
    if(BF_WriteBlock(fd,0)<0){
      BF_PrintError("write block failed\n");
      return -1;
    }

    if(BF_CloseFile(fd)<0){
      BF_PrintError("close file failed\n");
      return -1;
    }

return 0;

}


int Sorted_OpenFile( char *fileName )
{

      int fd=BF_OpenFile(fileName);
      if(fd<0){
        BF_PrintError("open file failed\n");
        return -1;
      }

      ES_info* Info=newES_info(0,0,0);
      void* block;

      if(BF_ReadBlock(fd,0,&block)<0){
        BF_PrintError("read block failed\n");
        return -1;
      }
      memcpy(Info,block,sizeof(ES_info));

      if(Info->type==0){
        printf("file not sorted\n");
        if(BF_CloseFile(fd)<0){
          BF_PrintError("close file failed\n");
          return -1;
        }
        return -1;
      }

  return fd;

}


int Sorted_CloseFile( int fileDesc )
{

  if(BF_CloseFile(fileDesc)<0){
    BF_PrintError("close file failed\n");
    return -1;
  }
  return 0;

}


int Sorted_InsertEntry( int fileDesc, Record record )
{

  int total = BF_GetBlockCounter(fileDesc);
  if(total<1){
    BF_PrintError("get block number failed\n");
    if(BF_CloseFile(fileDesc)<0){
      BF_PrintError("close file failed\n");
      return -1;
    }
    return -1;
  }

  void* block;
  block_info* info = new_block_info();

  if(total==1){

    if(BF_AllocateBlock(fileDesc)<0){
      BF_PrintError("wrong total block number\n");
      return -1;
    }

    total++;
    info->entries=1;

    if(BF_ReadBlock(fileDesc,total-1,&block)<0){
      BF_PrintError("read block failed\n");
      return -1;
    }

    memcpy(block,info,sizeof(block_info));
    memcpy(block+sizeof(block_info),&record,sizeof(Record));

    if(BF_WriteBlock(fileDesc,total-1)<0){
      BF_PrintError("write block failed\n");
      return -1;
    }

  }else{

        if(BF_ReadBlock(fileDesc,total-1,&block)<0){
          BF_PrintError("read block failed\n");
          return -1;
        }

        memcpy(info,block,sizeof(block_info));

        int space = BLOCK_SIZE - sizeof(block_info) - info->entries*sizeof(Record);

        if(space>sizeof(Record)){

          memcpy(block+sizeof(block_info)+info->entries*sizeof(Record),&record,sizeof(Record));
          info->entries++;
          memcpy(block,info,sizeof(block_info));

          if(BF_WriteBlock(fileDesc,total-1)<0){
            BF_PrintError("write block failed\n");
            return -1;
          }

        }else{

          if(BF_AllocateBlock(fileDesc)<0){
            BF_PrintError("allocate block failed\n");
            return -1;
          }

          total++;
          info->entries=1;

          if(BF_ReadBlock(fileDesc,total-1,&block)<0){
            BF_PrintError("read block failed\n");
            return -1;
          }

          memcpy(block,info,sizeof(block_info));
          memcpy(block+sizeof(block_info),&record,sizeof(Record));

          if(BF_WriteBlock(fileDesc,total-1)<0){
            BF_PrintError("write block failed\n");
            return -1;
          }

        }
      }

return 0;

}

void Sorted_SortFile( char *fileName, int fieldNo )
{

//for every block create new file
int fileDesc= Unsorted_OpenFile(fileName);

int files_left = BF_GetBlockCounter(fileDesc)-1;

if(files_left<1){
  BF_PrintError("wrong total block number\n");
  if(BF_CloseFile(fileDesc)<0){
    BF_PrintError("close file failed\n");
    return;
  }
  return;
}

char array[files_left][100];
printf("files left: %d\n",files_left);
int i;

void* block;
//bubble sort every block file
for(i=1;i<files_left+1;i++){

  if(BF_ReadBlock(fileDesc,i,&block)<0){
    BF_PrintError("read block failed\n");
    return;
  }
  if(Block_BubbleSort(block,fieldNo)<0){
    printf("bubble sort failed\n");
  }

  char buffer[7];
printf("5.1 FILES LEFT %d, i=%d\n", files_left,i);
int temp = files_left;
  snprintf(buffer,10,"%d",i);
  printf("buffer= %s\n",buffer);
printf("5.2 FILES LEFT %d, i=%d, temp = %d\n", files_left,i,temp);
files_left = temp;
  char newname[100] = "b";

  strcat(newname,buffer);
  printf("newname %s\n",newname);

  if(Sorted_CreateFile(newname)<0){
    printf("create file failed\n");
    return;
  }

  int fd =Unsorted_OpenFile(newname);
  if(fd<0){
    BF_PrintError("open file failed\n");
    return;
  }
  void* temp_block;
  if(BF_AllocateBlock(fd)<0){
    BF_PrintError("allocate block failed\n");
    return;
  }
  if(BF_ReadBlock(fd,1,&temp_block)<0){
    BF_PrintError("read block failed\n");
    return;
  }

  memcpy(temp_block,block,BLOCK_SIZE);

  if(BF_WriteBlock(fd,1)<0){
    BF_PrintError("write block failed\n");
    return;
  }
  PrintRecordsOfFile(fd);

  if(BF_CloseFile(fd)<0){
    BF_PrintError("close file failed\n");
    return;
  }
printf("newname %s\n",newname);
  strcpy(array[i-1],newname);
printf("9 array[%d] %s\n",i-1,array[i-1]);

}
int l;
for(l=0;l<files_left;l++){
	printf("ARRAY[%d]=%s\n",l,array[l]);
}

//merge block files

int files_to_merge = files_left;

while(files_left>1){

  int j;
  int array_size;
  if(files_to_merge%2==0){
    array_size=files_to_merge/2;
}else {
    array_size=(files_to_merge/2)+1;
  }
  char array2[array_size][100];
printf("array_size = %d\n",array_size );
  if (array_size%2!=0){
    strcpy(array2[array_size-1],array[files_to_merge-1]);
  }
    for(j=1;j<files_to_merge;j=j+2){
printf("j=%d\n",j);
printf("array[j-1]=%s , array[j]=%s\n", array[j-1], array[j]);
      char* newfile=File_MergeSort(array[j-1], array[j],fieldNo);
      if(newfile==NULL){
          printf("merge file failed\n");
          return;
      }
printf("newfile = %s\n",newfile );
      strcpy(array2[j/2],newfile);
      printf("j/2=%d array2[j/2]=%s\n",j/2,array2[j/2] );
    }
    printf("files to merge: %d, files left: %d\n",files_to_merge,files_left);
    if(files_to_merge%2==0){
    files_to_merge=files_to_merge/2;
    files_left=files_to_merge;
  }else {
      if(files_to_merge==2){
        strcpy(array2[0],array[0]);
        printf("array2[0]=%s,array[0]=%s\n",array2[0],array[0]);
      }else{
	strcpy(array2[files_to_merge/2],array[files_left-1]);
      files_to_merge=(files_to_merge/2)+1;
      files_left=files_to_merge;
    }
    }
    printf("files to merge: %d, files left: %d\n",files_to_merge,files_left);

    memcpy(array,array2,files_to_merge*sizeof(char[100]));
	int l;
for(l=0;l<files_left;l++){
	printf("ARRAY[%d]=%s\n",l,array[l]);
}

printf("end of while, files to merge: %d\n",files_to_merge);

}
printf("right after while\n");
 char new_file[50];
 strcpy(new_file,fileName);
 strcat(new_file,"Sorted");
printf("new_file: %s\n",new_file);

if(rename(array[0],new_file)!=0){
  printf("rename file failed\n");
  return;
}
printf("file renamed\n");

int fd=BF_OpenFile(fileName);
        if(fd<0){
          BF_PrintError("open file failed\n");
          return;
        }


ES_info* info=newES_info( 1,-1,fieldNo);
void* block1;

if(BF_ReadBlock(fd,0,&block1)<0){
    BF_PrintError("read block failed\n");
    return;
  }
memcpy(block1,info,sizeof(ES_info));
if(BF_WriteBlock(fd,0)<0){
    BF_PrintError("write block failed\n");
    return;
  }

  if(BF_CloseFile(fd)<0){
    BF_PrintError("close file failed\n");
    return;
  }




}

int Sorted_checkSortedFile( char *fileName, int fieldNo )
{

        int fd=BF_OpenFile(fileName);
        if(fd<0){
          BF_PrintError("open file failed\n");
          return -1;
        }

        ES_info* Info=newES_info(0,0,0);
        void* block;

        if(BF_ReadBlock(fd,0,&block)<0){
          BF_PrintError("read block failed\n");
          return -1;
        }
        memcpy(Info,block,sizeof(ES_info));

        if(Info->type==0 && Info->fieldNo!=fieldNo){
          printf("file not sorted by %d \n",fieldNo);
          if(BF_CloseFile(fd)<0){
            BF_PrintError("close file failed\n");
            return -1;
          }
          return -1;
        }
        printf("file sorted \n");
      return 0;
}

void Sorted_GetAllEntries( int fileDesc, int* fieldNo, void *value )
{

}
