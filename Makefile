# Compilateur et options
CC = clang
CFLAGS = -Wall -Werror
LCUNIT = -lcunit

# Répertoires
SRC = src
OBJECTS = objects
HEADERS = headers
HELP = help
TESTS = tests




main: $(OBJECTS)/main.o $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(OBJECTS)/file.o $(OBJECTS)/vector_threads.o
	$(CC) -o $@ $^ -pthread -lm 


generator_matrix: $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(OBJECTS)/file.o
	$(CC) $(CFLAGS) -o $@ $(HELP)/generator_matrix.c $^ -lm
	./$@

generator_vector: $(OBJECTS)/matrix.o $(OBJECTS)/vector.o $(OBJECTS)/file.o
	$(CC) $(CFLAGS) -o $@ $(HELP)/generator_vector.c $^ -lm
	./$@

$(OBJECTS)/main.o: $(SRC)/main.c $(HEADERS)/vector.h $(HEADERS)/matrix.h $(HEADERS)/file.h | $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS)/vector.o: $(SRC)/vector.c | $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS)/matrix.o: $(SRC)/matrix.c | $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS)/file.o: $(SRC)/file.c | $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -c $<


$(OBJECTS)/vector_threads.o: $(SRC)/vector_threads.c $(HEADERS)/vector_threads.h | $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJECTS):
	mkdir -p $(OBJECTS)


test: $(OBJECTS)/vector.o $(OBJECTS)/matrix.o $(OBJECTS)/file.o
	$(CC) $(CFLAGS) -o test $(TESTS)/tests_basic_op.c $^ $(LCUNIT) -lm
	./test
	$(CC) $(CFLAGS) -o test_file $(TESTS)/tests_file.c $^ $(LCUNIT) -lm
	./test_file

test_adv: $(OBJECTS)/vector.o $(OBJECTS)/matrix.o
	$(CC) $(CFLAGS) -o test_adv $(TESTS)/tests_adv_op.c $^ $(LCUNIT) -lm
	./test_adv

clean:
	rm -f $(OBJECTS)/*.o
	rm -f main generator_matrix generator_vector test test_file test_adv temp
	rm -f *.bin

.PHONY: clean test debug
