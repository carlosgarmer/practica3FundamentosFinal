CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic

# recompile if this header changes
HEADERS = library.h loop.h book.h

EXE = library
OBJ = $(EXE).o loop.o book.o

compile : $(EXE)

# compile all files ending in *.c
%.o: %.c $(HEADERS)
	@echo Compiling $<...
	$(CC) $(CFLAGS) -c -o $@ $<

# link binary
$(EXE): $(DEPS) $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LDLIBS)

clean:
	rm -f *.o core $(EXE)

run:
	./$(EXE)