#include "database.h"
#include "cart.h"
#include "linked_list.h"
#include "hash_table.h"
#include "iterator.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

struct merch
{
    char *name;
    char *description;
    int price;
    size_t stock;
    size_t available_stock;
    ioopm_list_t *locations;
};

struct shelf
{
    char *shelf;
    size_t quantity;
};

static bool str_eq_function(elem_t name1, elem_t name2)
{
    return !strcmp(name1.ptr, name2.ptr);
}

static bool ptr_eq_function(elem_t ptr1, elem_t ptr2)
{
    return ptr1.ptr == ptr2.ptr;
}

static bool int_eq_function(elem_t int1, elem_t int2)
{
    return int1.i == int2.i;
}

static int str_cmp_function(elem_t str1, elem_t str2)
{
    return strcmp(str1.ptr, str2.ptr);
}

static int int_cmp_function(elem_t a, elem_t b)
{
    return a.i - b.i;
}

static int string_sum_hash(elem_t e)
{
    char *str = e.ptr;
    int result = 0;
    do
    {
        result += *str;
    } while (*++str != '\0');
    return result;
}

static int int_hash(elem_t e)
{
    return e.i;
}

ioopm_hash_table_t *id_database_create()
{
    return ioopm_hash_table_create(string_sum_hash, int_eq_function, str_cmp_function);
}

ioopm_hash_table_t *merch_database_create()
{
    return ioopm_hash_table_create(int_hash, ptr_eq_function, int_cmp_function);
}

ioopm_hash_table_t *location_database_create()
{
    return ioopm_hash_table_create(string_sum_hash, str_eq_function, str_cmp_function);
}

char *merch_get_name(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        return merch->name;
    }
    return NULL;
}

char *merch_get_description(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        return merch->description;
    }
    return NULL;
}

bool merch_get_price(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name, int *price)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        *price = merch->price;
        return true;
    }
    return false;
}

bool merch_get_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name, int *stock)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        *stock = merch->stock;
        return true;
    }
    return false;
}

bool merch_get_available_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name, int *available_stock)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        *available_stock = merch->available_stock;
        return true;
    }
    return false;
}

ioopm_list_t *merch_get_locations(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (merch)
    {
        return merch->locations;
    }
    return NULL;
}

static int generate_id(ioopm_hash_table_t *id_db)
{
    int r = 0;
    while (ioopm_hash_table_has_value(id_db, int_elem(r)))
    {
        r = rand();
    }
    return r;
}

static void shelf_destroy(elem_t *shelf, void *extra)
{
    shelf_t *shelf_destroy = shelf->ptr;
    free(shelf_destroy->shelf);
    free(shelf_destroy);
}

static void merch_destroy(elem_t key, elem_t *value, void *extra)
{
    merch_t *merch = value->ptr;
    ioopm_list_t *locs = merch->locations;
    ioopm_linked_list_apply_to_all(locs, shelf_destroy, NULL);
    ioopm_linked_list_destroy(locs);
    free(merch->description);
    free(merch->name);
    free(merch);
}

void merch_database_destroy(ioopm_hash_table_t *merch_database)
{
    ioopm_hash_table_apply_to_all(merch_database, merch_destroy, NULL);
    ioopm_hash_table_destroy(merch_database);
}

void id_database_destroy(ioopm_hash_table_t *id_db)
{
    ioopm_hash_table_destroy(id_db);
}

void locations_database_destroy(ioopm_hash_table_t *locs_db)
{
    ioopm_hash_table_destroy(locs_db);
}

static merch_t *merch_create(char *name, char *description, int price)
{
    ioopm_list_t *locs_list = ioopm_linked_list_create(ptr_eq_function);

    merch_t merch = {.name = name,
                     .description = description,
                     .price = price,
                     .stock = 0,
                     .available_stock = 0,
                     .locations = locs_list};

    merch_t *pointer = calloc(1, sizeof(merch_t));
    *pointer = merch;

    return pointer;
}

void merch_insert(ioopm_hash_table_t *merch_database, ioopm_hash_table_t *id_database, char *name, char *description, int price)
{
    if (ioopm_hash_table_has_key(id_database, ptr_elem(name)))
    {
        return;
    }
    char *name_dup = strdup(name);
    char *desc_dup = strdup(description);
    merch_t *merch = merch_create(name_dup, desc_dup, price);
    int id = generate_id(id_database);
    ioopm_hash_table_insert(merch_database, int_elem(id), ptr_elem(merch));
    ioopm_hash_table_insert(id_database, ptr_elem(name_dup), int_elem(id));
}

void merch_remove(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name)
{
    merch_t *merch = merch_lookup(merch_db, id_db, name);
    if (!merch || merch->available_stock != merch->stock)
    {
        return;
    }

    int id = ioopm_hash_table_lookup(id_db, ptr_elem(name))->i;
    ioopm_hash_table_remove(id_db, ptr_elem(name));
    merch_destroy(int_elem(id), ioopm_hash_table_lookup(merch_db, int_elem(id)), NULL);
    ioopm_hash_table_remove(merch_db, int_elem(id));
}

merch_t *merch_lookup(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name)
{
    elem_t *elem_id = ioopm_hash_table_lookup(id_db, ptr_elem(name));
    if (!elem_id)
    {
        return NULL;
    }
    int id = elem_id->i;
    elem_t *merch_elem = ioopm_hash_table_lookup(merch_db, int_elem(id));
    assert(merch_elem);
    return merch_elem->ptr;
}

static shelf_t *shelf_create(char *shelf_name)
{
    shelf_t shelf_temp = {.shelf = shelf_name, .quantity = 0};
    shelf_t *shelf = calloc(1, sizeof(shelf_t));
    *shelf = shelf_temp;
    return shelf;
}

void merch_shelf_insert(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db,
                        ioopm_hash_table_t *locs_db, char *shelf_name, char *merch)
{
    merch_t *merch_to_update = merch_lookup(merch_db, id_db, merch);
    if (!merch_to_update)
    {
        return;
    }
    char *merch_name = merch_to_update->name;
    char *shelf_name_dup = strdup(shelf_name);
    shelf_t *shelf = shelf_create(shelf_name_dup);
    ioopm_linked_list_append(merch_to_update->locations, ptr_elem(shelf));
    ioopm_hash_table_insert(locs_db, ptr_elem(shelf_name_dup), ptr_elem(merch_name));
}

void merch_replenish(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch, char *shelf, int increase)
{
    merch_t *merch_to_replenish = merch_lookup(merch_db, id_db, merch);

    if (!merch_to_replenish)
    {
        return;
    }

    ioopm_list_t *locs_list = merch_to_replenish->locations;
    ioopm_list_iterator_t *locs_iter = ioopm_list_iterator(locs_list);
    shelf_t *current_shelf = ioopm_iterator_current(locs_iter).value.ptr;

    while (strcmp(current_shelf->shelf, shelf) != 0)
    {
        current_shelf = ioopm_iterator_next(locs_iter).value.ptr;
    }

    current_shelf->quantity += increase;
    merch_to_replenish->stock += increase;
    merch_to_replenish->available_stock += increase;

    ioopm_iterator_destroy(locs_iter);
}

static int cmpstringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

static void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmpstringp);
}

char **merch_name_sort(ioopm_hash_table_t *id_database)
{
    size_t size = ioopm_hash_table_size(id_database);
    char **keys = calloc(size, sizeof(char *));

    ioopm_list_t *keys_list = ioopm_hash_table_keys(id_database);
    if (!ioopm_linked_list_is_empty(keys_list))
    {
        ioopm_list_iterator_t *keys_iter = ioopm_list_iterator(keys_list);

        keys[0] = ioopm_iterator_current(keys_iter).value.ptr;

        for (int i = 1; i < size; i++)
        {
            keys[i] = ioopm_iterator_next(keys_iter).value.ptr;
        }
        ioopm_iterator_destroy(keys_iter);

        sort_keys(keys, size);
    }
    ioopm_linked_list_destroy(keys_list);
    return keys;
}

void merch_edit(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *old_name, char *new_name, char *new_desc, int price)
{
    merch_t *merch = merch_lookup(merch_db, id_db, old_name);
    if (!merch)
        return;

    char *new_name_dup = strdup(new_name);
    char *new_desc_dup = strdup(new_desc);

    elem_t *elem_id = ioopm_hash_table_lookup(id_db, ptr_elem(old_name));
    assert(elem_id);
    int id = elem_id->i;

    ioopm_hash_table_remove(id_db, ptr_elem(old_name));

    free(merch->description);
    free(merch->name);

    merch->name = new_name_dup;
    merch->description = new_desc_dup;
    merch->price = price;

    ioopm_hash_table_insert(id_db, ptr_elem(new_name_dup), int_elem(id));
}

char *shelf_get_name(shelf_t *shelf)
{
    if (!shelf)
    {
        return NULL;
    }
    return shelf->shelf;
}

bool shelf_get_quantity(shelf_t *shelf, int *quantity)
{
    if (!shelf)
    {
        return false;
    }
    *quantity = shelf->quantity;
    return true;
}

static int cmpshelf(const void *s1, const void *s2)
{
    shelf_t *const *shelf1 = s1;
    shelf_t *const *shelf2 = s2;
    return strcmp((*shelf1)->shelf, (*shelf2)->shelf);
}

static void sort_shelfs(shelf_t **shelfs, size_t no_shelfs)
{
    qsort(shelfs, no_shelfs, sizeof(shelf_t *), cmpshelf);
}

shelf_t **merch_show_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name)
{
    merch_t *merch = merch_lookup(merch_db, id_db, merch_name);
    if (!merch)
    {
        return NULL;
    }

    ioopm_list_t *stock = merch->locations;

    if (ioopm_linked_list_is_empty(stock))
    {
        return NULL;
    }

    size_t size = ioopm_linked_list_size(stock);
    shelf_t **shelfs = calloc(size, sizeof(shelf_t *));
    ioopm_list_iterator_t *iter = ioopm_list_iterator(stock);

    shelfs[0] = ioopm_iterator_current(iter).value.ptr;

    for (int i = 1; i < size; i++)
    {
        shelfs[i] = ioopm_iterator_next(iter).value.ptr;
    }

    ioopm_iterator_destroy(iter);

    sort_shelfs(shelfs, size);
    return shelfs;
}

void stock_reservation(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, char *name, int cart_id, size_t quantity)
{
    merch_t *merch_to_reserve = merch_lookup(merch_db, id_db, name);
    if (merch_to_reserve && merch_to_reserve->available_stock >= quantity)
    {
        char *merch_name = merch_to_reserve->name;
        merch_to_reserve->available_stock -= quantity;
        cart_add_to(cart_db, cart_id, merch_name, quantity);
    }
}

void stock_restoration(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name, size_t quantity)
{
    merch_t *merch_to_reserve = merch_lookup(merch_db, id_db, name);
    assert(merch_to_reserve);
    merch_to_reserve->available_stock += quantity;

}

void stock_decrease(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *locs_db, char *name, int quantity)
{
    merch_t *merch = merch_lookup(merch_db, id_db, name);
    if (!merch)
    {
        return;
    }
    ioopm_list_t *locs = merch->locations;
    ioopm_list_iterator_t *locs_iter = ioopm_list_iterator(locs);
    option_t locs_option = ioopm_iterator_current(locs_iter);
    int list_index = 0;

    while (locs_option.success && quantity > 0)
    {
        shelf_t *shelf = locs_option.value.ptr;
        int shelf_quantity = shelf->quantity;

        if (quantity >= shelf_quantity)
        {
            quantity -= shelf_quantity;
            locs_option = ioopm_iterator_next(locs_iter);
            ioopm_hash_table_remove(locs_db, ptr_elem(shelf->shelf));
            ioopm_linked_list_remove(locs, list_index);
            shelf_destroy(&ptr_elem(shelf), NULL);
            merch->stock -= shelf_quantity;
            list_index++;
        }

        else
        {
            shelf->quantity -= quantity;
            merch->stock -= quantity;
            quantity = 0;
            locs_option = ioopm_iterator_next(locs_iter);
        }
    }
    ioopm_iterator_destroy(locs_iter);
}
