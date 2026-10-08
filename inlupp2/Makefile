main: eventloop

C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
CUNIT_LINK     = -lcunit
OBJECTS 	   = linked_list.o utils.o hash_table.o cart.o database.o
CCOV	   	   = --coverage 

%.o: %.c
	$(C_COMPILER) $(C_OPTIONS) $^ -c

test_database: $(OBJECTS) test_database.o 
	$(C_COMPILER) $(C_OPTIONS) $^ -o $@ $(CUNIT_LINK) 

eventloop: $(OBJECTS) user_interface.o 
	$(C_COMPILER) $(C_OPTIONS) $^ -o $@ $(CUNIT_LINK)

test: test_database
	./test_database 

memtest: test_database
	valgrind --leak-check=full ./test_database

run: eventloop
	./eventloop

memrun: eventloop
	valgrind --leak-check=full ./eventloop

coverage: linked_list.c utils.c hash_table.c cart.c database.c test_database.c
	$(C_COMPILER) $(CCOV) $^ $(CUNIT_LINK)
	./a.out
	lcov --rc lcov_branch_coverage=1 --capture --directory . --output-file coverage.info
	genhtml --rc lcov_branch_coverage=1 coverage.info --output-directory out
	firefox out/index.html

clean:
	rm -f test_database a.out -r out *o *.gcda *gcno coverage.info eventloop
	rm -f -r out

.PHONY: clean coverage run memrun memtest test