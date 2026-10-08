#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#define Delimiters "+-#@()[]{}.,:;!? \t\n\r"

static int cmpstringp(const void *p1, const void *p2)
{
  return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
  qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

int str_sort(elem_t a, elem_t b){
    return strcmp(a.ptr,b.ptr);
}

void process_word(char *word, ioopm_hash_table_t *ht)
{
  elem_t *freq_ptr = ioopm_hash_table_lookup(ht, (elem_t) {.ptr = word});
  int freq =
   freq_ptr ?
    (*freq_ptr).i:
    0;
   elem_t elem = (elem_t) {.ptr = strdup(word)};
  ioopm_hash_table_insert(ht, elem, (elem_t) {.i = freq + 1});
  if(freq > 0){
    free(elem.ptr);
  }
}

void process_file(char *filename, ioopm_hash_table_t *ht)
{
  FILE *f = fopen(filename, "r");
  while (true)
  {
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, f);


    if (feof(f))
    {
      free(buf);
      break;
    }

    for (char *word = strtok(buf, Delimiters);
         word && *word;
         word = strtok(NULL, Delimiters))
    {
      process_word(word, ht);
    }
    free(buf);
  }

  fclose(f);
}

int string_sum_hash(elem_t e)
{
  char *str = e.ptr;
  int result = 0;
  do
    {
      result += *str;
    }
  while (*++str != '\0');
  return result;
}

bool int_eq(elem_t e1, elem_t e2){
    return e1.i==e2.i;
}

int main(int argc, char **argv)
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(string_sum_hash, int_eq, str_sort);
  if (argc > 1)
  {
    for (int i = 1; i < argc; ++i)
    {
      process_file(argv[i], ht);
    }

    // FIXME: If the keys are returned as a list, transfer them into 
    // an array to use `sort_keys` (perhaps using an iterator?)

    size_t size = ioopm_hash_table_size(ht);
    char *keys[size];

    ioopm_list_t *list_keys =  ioopm_hash_table_keys(ht);
    ioopm_list_iterator_t *keys_iter = ioopm_list_iterator(list_keys);

    keys[0] = ioopm_iterator_current(keys_iter).value.ptr;
    for(int i = 1; i < size; i++){
        keys[i] = ioopm_iterator_next(keys_iter).value.ptr;
    }

    sort_keys(keys, size);

    for (int i = 0; i < size; ++i)
    {
      // FIXME: Update to match your own interface, error handling, etc.
      int freq = (*(ioopm_hash_table_lookup(ht, (elem_t) {.ptr = keys[i]}))).i;
      printf("\n%s: %d\n", keys[i], freq);
    }
  ioopm_linked_list_destroy(list_keys);
  ioopm_iterator_destroy(keys_iter);
  }
  else
  {
    puts("Usage: freq-count file1 ... filen");
  }

  // FIXME: Leaks memory! Use valgrind to find out where that memory is 
  // being allocated, and then insert code here to free it.
  ioopm_hash_table_free_keys(ht);
  ioopm_hash_table_destroy(ht);
}

