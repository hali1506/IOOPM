linked_list: linked_list.c 
	gcc -Wall linked_list.c

test_linked_list: linked_list.c test_linked_list.c
	gcc -Wall -g test_linked_list.c linked_list.c -o  link_tests -lcunit

hash_table: hash_table.c 
	gcc -Wall hash_table.c -o hash_table

test_hash_table: hash_table.c test_hash_table.c linked_list.c
	gcc -Wall $^ -g -o hash_tests -lcunit

test_hash_table_coverage: hash_table.c test_hash_table.c 
	gcc --coverage test_hash_table.c hash_table.c -o tests -lcunit

test_hash_table_gdb: hash_table.c test_hash_table.c
	gcc -Wall -g test_hash_table.c hash_table.c -o tests -lcunit

test_iterator: test_iterator.c linked_list.c
	gcc -Wall -g $^ -o iterator_tests -lcunit

freq-count: freq-count.c hash_table.c linked_list.c
	gcc -Wall -g -pg $^ -o freq -lcunit

