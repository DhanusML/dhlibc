CC = gcc
FLAGS = -std=c99 -Wall

OBJECTS = dhlibc_base.o dhlibc_extra.o dhlibc_extra_list.o

all: libcdh

libcdh: $(OBJECTS)
	@mkdir -p lib
	ar rcs ./lib/libcdh.a $(OBJECTS)


dhlibc_base.o: ./impl/dhlibc_base.c
	$(CC) $(FLAGS) -g -c ./impl/dhlibc_base.c -o dhlibc_base.o

dhlibc_extra.o: ./impl/dhlibc_extra.c
	$(CC) $(FLAGS) -g -c ./impl/dhlibc_extra.c -o dhlibc_extra.o

dhlibc_extra_list.o: ./impl/dhlibc_extra_list.c
	$(CC) $(FLAGS) -g -c ./impl/dhlibc_extra_list.c -o dhlibc_extra_list.o

test: test.c test_list.c $(OBJECTS)
	$(CC) $(FLAGS) -g test.c $(OBJECTS) -o test 
	$(CC) $(FLAGS) -g test_list.c $(OBJECTS) -o test_list

clean:
	rm -rf $(OBJECTS) test test_list libcdh.a lib

.phony: all clean test
