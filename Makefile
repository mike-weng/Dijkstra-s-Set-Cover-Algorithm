# Makefile


OBJ     = main.o graph.o heap.o set.o
SRC     = main.c graph.c heap.c set.c
EXE     = assn2
CC      = g++
CFLAGS  = -Wall -m32 -O2

assn2:   $(OBJ) Makefile
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ)

clean:
	rm -f $(OBJ) $(EXE)

clobber: clean
	rm -f $(EXE)

usage: $(EXE)
	./$(EXE)

main.o: main.c graph.h heap.h set.h Makefile
graph.o: graph.c graph.h
heap.o: heap.c heap.h
set.o: set.c set.h heap.h set.h
 
