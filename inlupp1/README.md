# Usage
Compilation: To compile the program, use the terminal and type "make" followed by 
one of the specified rules in the Makefile. Note that there are separate rules 
for only compiling the code-files as well as the the code-files along with their 
C-Unit test files. 

Running the program: To run the program, simply enter the name of the compiled rule.
To run the freq-count, enter the desired text-file as command line argument.

# Design
A few design decisions:
    * Two types of error handling are used, the first one being options, which
    signal wether the program was succcessful when fetching a certain value. If the option 
    is not successful the desired action is dicraded. Another approach
    that is used is "Failure is Not an Option" as a way of preventing errors from
    happening in the first place.

    * The h-files are structured so that link_iter.h contains the type definitions
    and structs shared by the iterator and linked list. link_iter.h is therefore included 
    both by iterator.h and linked_list.h. Since the iterator is based on the linked list,
    iterator.h also includes linked_list.h. The declarations of most of the iterator
    and linked list functions are placed in the linked_list.c file. Lastly, hash_table.h 
    includes linked_list.h and link_iter.h and hash_table.c includes hash_table.h
    and iterator.h.

    * In the beginning of the program you have to choose which data type the keys
    are going to be, as well as the values. This is specified when you create the
    hashtable where you enter the compare function compatible with the 
    chosen key type and the equality function compatible with the chosen value type. 

    * At the creation of a list, a sentinel is inserted in the beginning. This is
    not something that has to be taken into consideration when using the functions
    for fetching and modifying the entries.

    *If the hash, compare-and eq-functions aren't specified when creating the 
    hash table, it is assumed that the keys and values are of int-types.

# Initial Profiling Results
The top functions for the text files small, 1k-long-words, 10k-word and 
16k-words are the same for each input:
    1. standard_sort, 
    2. adjust_hash_index, find_prevoius_entry_for_key, string_sum_hash
    3. string_eq

The reason that the same functions appear in the top three is that they are used
in every function treating this data structure in the freq-count implementation.
The functions sharing second place are always used together and could be refactored
into one component. Larger files created a larger proportion of sort-function calls
since the buckets were filled up to a larger extent. Another trend is that 
ioopm_hash_table_insert has twice the calls of ioopm_hash_table_lookup and 
ioopm_has_keys respectively. Which are called once for each word in the input file. 
The above-mentioned features all make sense since we use lookup twice for each word,
twice when reading the file if the word has been found previously, otherwise once,
and once per unique word when printing occurrences. It makes sense that our helper
functions are called more often than the more external ioopm functions since they
are higher-order functions. We also tried changing the number of buckets and saw a
significant change in the number of standard_sort calls, implying that the number of calls
correlates with the length of the buckets

The easiest way to speed up the program would be to continuously adapt the number of buckets
depending on the size of the hash table resulting in fewer comparisons between 
entries when inserted. Another way to make freq-count more efficient is by storing
the value from ioopm_hash_table_lookup in process_word rather than making two
separate function calls to find out if there is a value and then what that value 
is. 


