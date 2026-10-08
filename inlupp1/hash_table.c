#include "hash_table.h"
#define No_Buckets 17
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "iterator.h"
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .ptr=(x) }

typedef struct entry entry_t;


struct entry
{
    elem_t key;       // holds the key
    elem_t value;   // holds the value
    entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
    entry_t buckets[No_Buckets];
    int size;
    ioopm_eq_function eq_fun;
    ioopm_hash_function hash_fun;
    ioopm_sort_function sort_fun;
};

int standard_hash(elem_t key){
    return key.i;
}

bool standard_eq(elem_t a, elem_t b){
    return a.i-b.i == 0;
}

int standard_sort(elem_t a, elem_t b){
    return a.i-b.i;
}

int adjust_hash_index(int index){
    return (index % No_Buckets + No_Buckets) % No_Buckets;
}
ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun, ioopm_sort_function sort)
{
    ioopm_hash_table_t *result = calloc(1, sizeof(ioopm_hash_table_t));
    if(eq_fun == NULL){
        eq_fun = standard_eq;
    }
    result->eq_fun = eq_fun;
    if(hash_fun == NULL){
        hash_fun = standard_hash;
    }
    result->hash_fun = hash_fun;
    if(sort == NULL){
        sort = standard_sort;
    }
    result->sort_fun = sort;
    return result;
}

static void entry_destroy(entry_t *entry)
{
    if (entry->next != NULL)
    {
        entry_destroy(entry->next);
    }
    free(entry);
}

static void free_keys(entry_t *entry)
{
    if (entry->next != NULL)
    {
        free_keys(entry->next);
    }
    free(entry->key.ptr);
}

void ioopm_hash_table_free_keys(ioopm_hash_table_t *ht)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        if (ht->buckets[i].next != NULL)
        {
            free_keys(ht->buckets[i].next);
        }
    }
    }

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        if (ht->buckets[i].next != NULL)
        {
            entry_destroy(ht->buckets[i].next);
            ht->buckets[i].next = NULL;
        }
    }
    ht->size = 0;
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
    ioopm_hash_table_clear(ht);
    free(ht);
}

static entry_t *find_previous_entry_for_key(ioopm_sort_function compare, entry_t *bucket, elem_t key)
{
    entry_t *cursor = bucket->next;
    while (true)
    {
        if (cursor == NULL)
        {
            return bucket;
        }
        else if (compare(cursor->key, key) >= 0)
        {
            return bucket;
        }
        else if (cursor->next == NULL)
        {
            return cursor;
        }
        bucket = cursor;
        cursor = cursor->next;  
    }
}

static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
    entry_t entry = {.key = key, .value = value, .next = next};
    entry_t *pointer = calloc(1, sizeof(entry_t));
    *pointer = entry;
    return pointer;
}


void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
    /// Calculate the bucket for this entry
    int bucket_index = adjust_hash_index(ht->hash_fun(key));
    /// Search for an existing entry for a key
    entry_t *entry = find_previous_entry_for_key(ht->sort_fun,&ht->buckets[bucket_index], key);
    entry_t *next = entry->next;

    /// Check if the next entry should be updated or not
    if (next != NULL && ht->sort_fun(next->key, key) == 0)
    {
        next->value = value;
    }
    else
    {
        entry->next = entry_create(key, value, next);
        ht->size++;
    }
}

elem_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
    entry_t *bucket = &ht->buckets[adjust_hash_index(ht->hash_fun(key))];
    entry_t *previous = find_previous_entry_for_key(ht->sort_fun, bucket, key);
    elem_t *result;
    if (previous->next != NULL && ht->sort_fun(previous->next->key,key) == 0)
    {
        result = &(previous->next->value);
    }
    else
    {
        // entry not found
        result = NULL;
    }

    return result;
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
    entry_t *previous = find_previous_entry_for_key(ht->sort_fun, &ht->buckets[adjust_hash_index(ht->hash_fun(key))], key);
    elem_t value = {.ptr = NULL};
    // Entry for key found
    if (previous->next != NULL && ht->sort_fun(previous->next->key, key) == 0)
    {
        // Entry not last in bucket
        if (previous->next->next != NULL)
        {
            entry_t *temp = previous->next->next;
            value = previous->next->value;
            free(previous->next);
            previous->next = temp;
        }
        else
        {
            value = previous->next->value;
            free(previous->next);
            previous->next = NULL;
        }
        ht->size--;
    }
    return value;
}

int ioopm_hash_table_size(ioopm_hash_table_t *ht)
{
    return ht->size;
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
    return !ioopm_hash_table_size(ht);
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
    ioopm_list_t *values_list = ioopm_linked_list_create(ht->eq_fun);
    int count = 0;
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *entry = ht->buckets[i].next;
        while (entry != NULL)
        {
            ioopm_linked_list_append(values_list, entry->key);
            entry = entry->next;
            count++;
        }
    }
    return values_list;
}

ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
    ioopm_list_t *values_list = ioopm_linked_list_create(ht->eq_fun);
    int count = 0;
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *entry = ht->buckets[i].next;
        while (entry != NULL)
        {
            ioopm_linked_list_append(values_list, entry->value);
            entry = entry->next;
            count++;
        }
    }
    return values_list;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
    size_t size = ioopm_hash_table_size(ht);
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    ioopm_list_t *values = ioopm_hash_table_values(ht);
    ioopm_list_iterator_t *values_iter = ioopm_list_iterator(values);
    ioopm_list_iterator_t *keys_iter = ioopm_list_iterator(keys);
    bool result = true;
    option_t next_value = ioopm_iterator_current(values_iter);
    option_t next_key = ioopm_iterator_current(keys_iter);
    for (int i = 0; i < size && result; ++i)
    {
        if(next_value.success && next_key.success){
            result = pred(next_key.value, next_value.value, arg);
        }
        else{
            printf("internal error, size field likely greater than number of contained elements");
        }
        next_value = ioopm_iterator_next(values_iter);
        next_key = ioopm_iterator_next(keys_iter);
    }
    ioopm_linked_list_destroy(values);
    ioopm_linked_list_destroy(keys);
    ioopm_iterator_destroy(values_iter);
    ioopm_iterator_destroy(keys_iter);
    return result;
}


bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg)
{
    size_t size = ioopm_hash_table_size(ht);
    ioopm_list_t *keys = ioopm_hash_table_keys(ht);
    ioopm_list_t *values = ioopm_hash_table_values(ht);
    ioopm_list_iterator_t *keys_iter = ioopm_list_iterator(keys);
    ioopm_list_iterator_t *values_iter = ioopm_list_iterator(values);
    bool result = false;
    option_t next_value = ioopm_iterator_current(values_iter);
    option_t next_key = ioopm_iterator_current(keys_iter);
    for (int i = 0; i < size && !result; ++i)
    {
        if(next_value.success && next_key.success){
            result = pred(next_key.value, next_value.value, arg);
        }
        else{
            printf("%ld \n", size);
            printf("internal error, size field likely greater than number of contained elements \n");
        }
        next_value = ioopm_iterator_next(values_iter);
        next_key = ioopm_iterator_next(keys_iter);
    }
    ioopm_linked_list_destroy(values);
    ioopm_linked_list_destroy(keys);
    ioopm_iterator_destroy(values_iter);
    ioopm_iterator_destroy(keys_iter);
    return result;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{

    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *entry = ht->buckets[i].next;
        while (entry != NULL)
        {
            apply_fun(entry->key, &entry->value, arg);
            entry = entry->next;
        }
    }
}


bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
    return ioopm_hash_table_lookup(ht, key) != NULL;
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
    for (int i = 0; i < No_Buckets; i++)
    {
        entry_t *bucket = &ht->buckets[i];
        while (bucket->next != NULL)
        {
            bucket = bucket->next;
            if (ht->eq_fun(bucket->value, value))
            {
                return true;
            }
        }
    }
    return false;
}
