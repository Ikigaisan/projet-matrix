CC=clang
CFLAGS=-Wall -Werror
LCUNIT=-lcunit
OBJECTS=objects
HELP=help
SRC=src

# Détection de l'OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    FILE_OBJ=$(OBJECTS)/file_MAC.o
else
    FILE_OBJ=$(OBJECTS)/file.o
endif

main: main.o matrix.o vector.o
	$(CC) -o $@ $(OBJECTS)/main.o $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(FILE_OBJ) -lm

generator_matrix: matrix.o vector.o
	$(CC) $(CFLAGS) -o $@ $(HELP)/generator_matrix.c $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(FILE_OBJ) -lm
	./$@

generator_vector: matrix.o vector.o
	$(CC) $(CFLAGS) -o $@ $(HELP)/generator_vector.c $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(FILE_OBJ) -lm
	./$@

main.o: $(SRC)/main.c headers/vector.h headers/matrix.h headers/file.h
	$(CC) $(CFLAGS) -o $(OBJECTS)/$@ -c $<

vector.o: $(SRC)/vector.c
	$(CC) $(CFLAGS) -o $(OBJECTS)/$@ -c $<

matrix.o: $(SRC)/matrix.c
	$(CC) $(CFLAGS) -o $(OBJECTS)/$@ -c $<

test: tests/tests_basic_op.c vector.o matrix.o
	$(CC) $(CFLAGS) -o test tests/tests_basic_op.c $(OBJECTS)/vector.o $(OBJECTS)/matrix.o $(FILE_OBJ) $(LCUNIT) -lm
	./test
	$(CC) $(CFLAGS) -o test_file tests/tests_file.c $(OBJECTS)/vector.o $(OBJECTS)/matrix.o $(FILE_OBJ) $(LCUNIT) -lm
	./test_file

.PHONY: clean

clean:
	rm -f objects/main.o
	rm -f objects/vector.o
	rm -f objects/matrix.o
	rm -f main
	rm -f generator_matrix
	rm -f generator_vector
	rm -f test_file
	rm -f test.bin
	rm -f vector.bin
	rm -f test
	rm -f double.bin
	rm -f matrix.bin