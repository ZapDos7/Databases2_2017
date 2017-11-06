#include "Auxiliary.h"
#include "BF.h"
#include "Sorted.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


// Record functions
Record* newRecord(int id, char *name[15], char surname[20], char city[25]) {
    Record* record = (Record *) malloc(sizeof (Record));
    record->id = id;
    strcpy(record->name, *name);
    strcpy(record->surname, surname);
    strcpy(record->city, city);
    return record;
}

void printRecord(Record record) {
    printf("Id: %d Name: %s Surname: %s City: %s \n", record.id, record.name, record.surname,record.city);
}

// Info functions
ES_info* newES_info(int type, int fileDesc, int fieldNo)
{
    ES_info* fileInfo = (ES_info *) malloc(sizeof (ES_info));
    fileInfo->type = type;
    fileInfo->fileDesc = fileDesc;
    fileInfo->fieldNo = fieldNo;
    return fileInfo;
}

void printES_info(ES_info* FileInfo){
    printf( "Type of file: %d FileDesc: %d fieldNo: %d\n", FileInfo->type, FileInfo->fileDesc, FileInfo->fieldNo);
}

// Block info functions

block_info* new_block_info(){
  block_info* info = (block_info *) malloc(sizeof(block_info));
  info->entries=0;
  return info;
}

// Other functions
int Unsorted_OpenFile(char* fileName)
{
  int fd=BF_OpenFile(fileName);
  if(fd<0){
    BF_PrintError("open file failed\n");
    return -1;
      }
      return fd;
}


int Block_BubbleSort(void* block, int field){

  block_info* info=new_block_info();
  memcpy(info,block,sizeof(block_info));

  Record record1, record2;
  bool swap=true;

  while(swap){
    // while still swaps take place
    swap=false;
    int i;
    for(i=0;i<info->entries-1;i++){
      memcpy(&record1,block+sizeof(block_info)+i*sizeof(Record),sizeof(Record));
      memcpy(&record2,block+sizeof(block_info)+(i+1)*sizeof(Record),sizeof(Record));

      if(field==0){

        if(record1.id>record2.id){
          //swap
          swap=true;
          memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
          memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

        }

      }else if(field==1){

        if(strcmp(record1.name,record2.name)>0){
          //swap
          swap=true;
          memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
          memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

        }else if(strcmp(record1.name,record2.name)==0){

          if(strcmp(record1.surname,record2.surname)>0){
            //swap
            swap=true;
            memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
            memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

          }else if(strcmp(record1.surname,record2.surname)==0){

            if(strcmp(record1.city,record2.city)>0){
              //swap
              swap=true;
              memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
              memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

            }else if(strcmp(record1.city,record2.city)==0){

              if(record1.id>record2.id){
                //swap
                swap=true;
                memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
                memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));
              }
            }
          }
        }
      }else if(field==2){

        if(strcmp(record1.surname,record2.surname)>0){
          //swap
          swap=true;
          memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
          memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

        }else if(strcmp(record1.surname,record2.surname)==0){

          if(strcmp(record1.name,record2.name)>0){
            //swap
            swap=true;
            memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
            memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

          }else if(strcmp(record1.name,record2.name)==0){

            if(strcmp(record1.city,record2.city)>0){
              //swap
              swap=true;
              memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
              memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

            }else if(strcmp(record1.city,record2.city)==0){

              if(record1.id>record2.id){
                //swap
                swap=true;
                memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
                memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));
              }
            }
          }
        }
      }else if(field==3){

        if(strcmp(record1.city,record2.city)>0){
          //swap
          swap=true;
          memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
          memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

        }else if(strcmp(record1.city,record2.city)==0){
          if(strcmp(record1.surname,record2.surname)>0){
            //swap
            swap=true;
            memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
            memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

          }else if(strcmp(record1.surname,record2.surname)==0){

            if(strcmp(record1.name,record2.name)>0){
              //swap
              swap=true;
              memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
              memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));

            }else if(strcmp(record1.name,record2.name)==0){

                if(record1.id>record2.id){
                  //swap
                  swap=true;
                  memcpy(block+sizeof(block_info)+i*sizeof(Record),&record2,sizeof(Record));
                  memcpy(block+sizeof(block_info)+(i+1)*sizeof(Record),&record1,sizeof(Record));
                }
              }
            }
          }
        }else{
          printf("wrong field number\n");
          return -1;
        }
     }
  }

 return 0;

}

char* File_MergeSort(char* filename1, char* filename2, int field){

  //open files
  int file1, file2;
  file1 = Unsorted_OpenFile(filename1);
  file2 = Unsorted_OpenFile(filename2);
  if(file1<0 || file2<0){
    printf("open file failed\n");
    if(BF_CloseFile(file1)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file2)<0){
      BF_PrintError("close file failed\n");
    }
    return NULL;
  }

  // get total block number
  int total1 = BF_GetBlockCounter(file1);
  int total2 = BF_GetBlockCounter(file2);

  if(total1<0 || total2<0){
    printf("get block counter failed\n");
    if(BF_CloseFile(file1)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file2)<0){
      BF_PrintError("close file failed\n");
    }
    return NULL;
  }


printf("File_MergeSort: create new file and total1:%d,total2:%d\n",total1,total2);
  // create new file
  // -> neo filename
  char newfilename[100]="";
  const char * str = filename1;
  int value;
  int a, count;
  for (a=0, count=0; str[a]; a++)
    count += (str[a] == 'n');
  sscanf(str,"%*[^0-9]%d",&value);
  for(a=0;a<count+1;a++)
    strcat(newfilename,"n");
  char buffer[7];
  snprintf(buffer,10,"%d",value/2+1);
  strcat(newfilename,buffer);
  printf("newfilename is:%s\n",newfilename);

  // dimiourgia arxeiou
  if(Sorted_CreateFile(newfilename)<0){
     printf("Create sorted file failed\n");
     return NULL;
   }
  int fd = Unsorted_OpenFile(newfilename);
  if(fd<0){
     printf("Open file failed\n");
     return NULL;
   }
  if(BF_AllocateBlock(fd)<0){
     BF_PrintError("allocate block failed\n");
     if(BF_CloseFile(fd)<0){
       BF_PrintError("close file failed\n");
     }
     if(BF_CloseFile(file1)<0){
       BF_PrintError("close file failed\n");
     }
     if(BF_CloseFile(file2)<0){
       BF_PrintError("close file failed\n");
     }
     return NULL;
   }

  //set counters
  int left1 = total1-1;
  int left2 = total2-1;
  int i=1; // block counter for new file

  Record record1, record2;
  int r1=0; // thesi eggrafis sto block tou protou arxeiou
  int r2=0; // thesi eggrafis sto block tou deuterou arxeiou
  int n=0;  // thesi eggrafis sto block tou neou arxeiou

  void* block1;
  void* block2;
  void* newblock;

  block_info* info1 =new_block_info();
  block_info* info2 = new_block_info();
  block_info* newinfo = new_block_info();

  // read first block from each file

  // read block of file 1
  if(BF_ReadBlock(file1,total1-left1,&block1)<0){
    BF_PrintError("read block failed\n");
    if(BF_CloseFile(fd)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file1)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file2)<0){
      BF_PrintError("close file failed\n");
    }
    return NULL;
  }
  memcpy(info1,block1,sizeof(block_info));

  // read block of file 2
  if(BF_ReadBlock(file2,total2-left2,&block2)<0){
    BF_PrintError("read block failed\n");
    if(BF_CloseFile(fd)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file1)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file2)<0){
      BF_PrintError("close file failed\n");
    }
    return NULL;
  }
  memcpy(info2,block2,sizeof(block_info));

  // read block of new file
  if(BF_ReadBlock(fd,i,&newblock)<0){
    BF_PrintError("read block failed \n");
    if(BF_CloseFile(fd)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file1)<0){
      BF_PrintError("close file failed\n");
    }
    if(BF_CloseFile(file2)<0){
      BF_PrintError("close file failed\n");
    }
    return NULL;
  }
  memcpy(newinfo,newblock,sizeof(block_info));


  int space_for_recs = (BLOCK_SIZE - sizeof(block_info))/sizeof(Record);


  //read all blocks and merge
  while(left1>0  || left2>0){ // oso yperxoyn block gia anagnosi


    // scan records
    //printf("File_MergeSort: scan records\n");
if(left1>0  && left2>0){ //oso kai ta dyo arxeia einai akoma gemata
      memcpy(&record1,block1+sizeof(block_info)+r1*sizeof(Record),sizeof(Record));
      memcpy(&record2,block2+sizeof(block_info)+r2*sizeof(Record),sizeof(Record));

      if(field==0){

        if(record1.id>record2.id){
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
          n++;
          space_for_recs--;
          r2++;
          info2->entries--;
          newinfo->entries++;
        }else{
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
          n++;
          space_for_recs--;
          r1++;
          info1->entries--;
          newinfo->entries++;
        }

      }else if(field==1){

        if(strcmp(record1.name,record2.name)>0){
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
          n++;
          space_for_recs--;
          r2++;
          info2->entries--;
          newinfo->entries++;
        }else if(strcmp(record1.name,record2.name)==0){


          if(strcmp(record1.surname,record2.surname)>0){
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
            n++;
            space_for_recs--;
            r2++;
            info2->entries--;
            newinfo->entries++;
          }else if(strcmp(record1.surname,record2.surname)==0){

            if(strcmp(record1.city,record2.city)>0){
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
              n++;
              space_for_recs--;
              r2++;
              info2->entries--;
              newinfo->entries++;
            }else if(strcmp(record1.city,record2.city)==0){

              if(record1.id>record2.id){
                memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
                n++;
                space_for_recs--;
                r2++;
                info2->entries--;
                newinfo->entries++;
              }else{
                 memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
                n++;
                space_for_recs--;
                r1++;
                info1->entries--;
                newinfo->entries++;
              }
            }else{
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
              n++;
              space_for_recs--;
              r1++;
              info1->entries--;
              newinfo->entries++;
            }
          }else{
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
            n++;
            space_for_recs--;
            r1++;
            info1->entries--;
            newinfo->entries++;
          }
        }else{
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
          n++;
          space_for_recs--;
          r1++;
          info1->entries--;
          newinfo->entries++;
        }
      }else if(field==2){

        if(strcmp(record1.surname,record2.surname)>0){
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
          n++;
          space_for_recs--;
          r2++;
          info2->entries--;
          newinfo->entries++;
        }else if(strcmp(record1.surname,record2.surname)==0){

          if(strcmp(record1.name,record2.name)>0){
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
            n++;
            space_for_recs--;
            r2++;
            info2->entries--;
            newinfo->entries++;
          }else if(strcmp(record1.name,record2.name)==0){

            if(strcmp(record1.city,record2.city)>0){
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
              n++;
              space_for_recs--;
              r2++;
              info2->entries--;
              newinfo->entries++;
            }else if(strcmp(record1.city,record2.city)==0){

              if(record1.id>record2.id){
                memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
                n++;
                space_for_recs--;
                r2++;
                info2->entries--;
                newinfo->entries++;
              }else{
                memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
                n++;
                space_for_recs--;
                r1++;
                info1->entries--;
                newinfo->entries++;
              }
            }else{
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
              n++;
              space_for_recs--;
              r1++;
              info1->entries--;
              newinfo->entries++;
            }
          }else{
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
            n++;
            space_for_recs--;
            r1++;
            info1->entries--;
            newinfo->entries++;
          }
        }else{
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
          n++;
          space_for_recs--;
          r1++;
          info1->entries--;
          newinfo->entries++;
        }
      }else if(field==3){

        if(strcmp(record1.city,record2.city)>0){
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
          n++;
          space_for_recs--;
          r2++;
          info2->entries--;
          newinfo->entries++;
        }else if(strcmp(record1.city,record2.city)==0){
          if(strcmp(record1.surname,record2.surname)>0){
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
            n++;
            space_for_recs--;
            r2++;
            info2->entries--;
            newinfo->entries++;
          }else if(strcmp(record1.surname,record2.surname)==0){

            if(strcmp(record1.name,record2.name)>0){
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
              n++;
              space_for_recs--;
              r2++;
              info2->entries--;
              newinfo->entries++;
            }else if(strcmp(record1.name,record2.name)==0){

                if(record1.id>record2.id){
                  memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
                  n++;
                  space_for_recs--;
                  r2++;
                  info2->entries--;
                  newinfo->entries++;
                }else{
                  memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
                  n++;
                  space_for_recs--;
                  r1++;
                  info1->entries--;
                  newinfo->entries++;
                }
              }else{
                memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
                n++;
                space_for_recs--;
                r1++;
                info1->entries--;
                newinfo->entries++;
              }
            }else{
              memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
              n++;
              space_for_recs--;
              r1++;
              info1->entries--;
              newinfo->entries++;
            }
          }else{
            memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
            n++;
            space_for_recs--;
            r1++;
            info1->entries--;
            newinfo->entries++;
          }
        }else{
          printf("wrong field number\n");
          return NULL;
        }
      }
      else{ // an ena apo ta dyo einai adeio

        if(left1==0){
          memcpy(&record2,block2+sizeof(block_info)+r2*sizeof(Record),sizeof(Record));
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record2,sizeof(Record));
          n++;
          space_for_recs--;
          r2++;
          info2->entries--;
          newinfo->entries++;
        }else if(left2==0){
          memcpy(&record1,block1+sizeof(block_info)+r1*sizeof(Record),sizeof(Record));
          memcpy(newblock+sizeof(block_info)+n*sizeof(Record),&record1,sizeof(Record));
          n++;
          space_for_recs--;
          r1++;
          info1->entries--;
          newinfo->entries++;
        }


      }
	 memcpy(newblock,newinfo,sizeof(block_info));
          if(BF_WriteBlock(fd,i)<0){
            BF_PrintError("write block failed\n");
            return NULL;
          }


        if(info1->entries==0 && left1>1){
          printf("File_MergeSort: read next block of file 1\n");
          left1--;
          r1=0;
            // read block of file 1
            if(BF_ReadBlock(file1,total1-left1,&block1)<0){
              BF_PrintError("read block failed\n");
              if(BF_CloseFile(fd)<0){
                BF_PrintError("close file failed\n");
              }
              if(BF_CloseFile(file1)<0){
                BF_PrintError("close file failed\n");
              }
              if(BF_CloseFile(file2)<0){
                BF_PrintError("close file failed\n");
              }
              return NULL;
            }
            memcpy(info1,block1,sizeof(block_info));


        }else if(info1->entries==0 && left1==1){
		left1=0;
    r1=0;
		printf("file 1 empty\n");
	}
	if(info2->entries==0 && left2>1){
          printf("File_MergeSort: read next block of file 2\n");
          left2--;
          r2=0;
          // read block of file 2
          if(BF_ReadBlock(file2,total2-left2,&block2)<0){
            BF_PrintError("read block failed\n");
            if(BF_CloseFile(fd)<0){
              BF_PrintError("close file failed\n");
            }
            if(BF_CloseFile(file1)<0){
              BF_PrintError("close file failed\n");
            }
            if(BF_CloseFile(file2)<0){
              BF_PrintError("close file failed\n");
            }
            return NULL;
          }
          memcpy(info2,block2,sizeof(block_info));
        }else if(info2->entries==0 && left2==1){
		left2=0;
    r2=0;
		printf("file 2 empty\n");

	}


if(space_for_recs==0 ){
printf("space for recs%d\n",space_for_recs);
printf("total1:%d,total2:%d\n",total1,total2);
			printf("iiiiiiiiiiiiiiiiiiii:%d\n",i);
	if (left1>0 || left2>0){
          printf("File_MergeSort: new block for new file \n");
          space_for_recs = (BLOCK_SIZE - sizeof(block_info))/sizeof(Record);
          memcpy(newblock,newinfo,sizeof(block_info));
          if(BF_WriteBlock(fd,i)<0){
            BF_PrintError("write block failed\n");
            return NULL;
          }
          i++;
		printf("new allocate i:%d\n",i);
		n=0;
          if(BF_AllocateBlock(fd)<0){
             BF_PrintError("allocate block failed\n");
             if(BF_CloseFile(fd)<0){
               BF_PrintError("close file failed\n");
		return NULL;
             }
             if(BF_CloseFile(file1)<0){
               BF_PrintError("close file failed\n");
		return NULL;
		}
             if(BF_CloseFile(file2)<0){
               BF_PrintError("close file failed\n");
		return NULL;
		}
             return NULL;
           }
           // read block of new file
           if(BF_ReadBlock(fd,i,&newblock)<0){
             BF_PrintError("read block failed\n");
             if(BF_CloseFile(fd)<0){
               BF_PrintError("close file failed\n");
		return NULL;
             }
             if(BF_CloseFile(file1)<0){
               BF_PrintError("close file failed\n");
				return NULL;
		}
             if(BF_CloseFile(file2)<0){
               BF_PrintError("close file failed\n");
			return NULL;
             }
             return NULL;
           }
           memcpy(newinfo,newblock,sizeof(block_info));
		newinfo->entries=0;

        }

	}
    }

    PrintRecordsOfFile(fd);
    if(BF_CloseFile(fd)<0 || BF_CloseFile(file1)<0 || BF_CloseFile(file2)<0){
	BF_PrintError("close file failed\n");
	 return NULL;
	}
	 char* k=newfilename;
    return k;


  }




void PrintRecordsOfFile(int fileDesc){
  int total = BF_GetBlockCounter(fileDesc);
  if(total<0){
    BF_PrintError("get block counter failed\n");
  }
  int i;
  for(i=0;i<total;i++){
printf("PrintRecordsOfFile: block no %d apo ta %d\n",i+1,total);
    void* block;
    block_info* info = new_block_info();
    if(BF_ReadBlock(fileDesc,i,&block)){
      BF_PrintError("read block failed\n");
    }
    memcpy(info,block,sizeof(block_info));
    int j;
    Record record;
    for(j=0;j<info->entries;j++){
      memcpy(&record,block+sizeof(block_info)+j*sizeof(Record),sizeof(Record));
      printRecord(record);
    }
  }
}
