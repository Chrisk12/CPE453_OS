CC=gcc
CFLAGS=-I -Wall
OBJ = pipeit.o 

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)