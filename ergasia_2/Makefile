
OBJS1	= Sorted.o Auxiliary.o main.o 
OUT1	= execES
CC	= gcc
FLAGS	= -c -Wall

UNAME := $(shell uname -p)
ifeq ($(UNAME),i686)
	LIB = BF_32.a -lm
else
	LIB = BF_64.a -lm
endif




# OBJECTS



# EXTERNAL SORT
Sorted.o: Sorted.c Sorted.h
	$(CC) $(FLAGS) Sorted.h Sorted.c

Auxiliary.o: Auxiliary.c Auxiliary.h
	$(CC) $(FLAGS) Auxiliary.h Auxiliary.c



# test ES

main.o: main.c
	$(CC) $(FLAGS) main.c
t1: $(OBJS1)
	$(CC) $(OBJS1) $(LIB) -o $(OUT1)



# clean up
clean:
	rm -f $(OBJS1) $(OUT1)
