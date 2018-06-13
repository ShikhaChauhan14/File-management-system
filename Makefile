CC = g++
CFLAGS = -std=gnu++11 -g -Wall
OBJ = main.o createDisk.o root.o file.o directory.o helper.o
all : $(OBJ) disk.h
	$(CC) $(CFLAGS) -o disk ${OBJ}

%.o : %.cpp disk.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean :
	rm -rf *o disk