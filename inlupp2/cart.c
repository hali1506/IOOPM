#include "cart.h"
#include "hash_table.h"
#include "linked_list.h"
#include "iterator.h"
#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct cart
{
    int id;
    ioopm_list_t *reservations;
};

struct reservation
{
    char *name;
    int quantity;
};

static bool ptr_eq(elem_t e1, elem_t e2)
{
    return e1.ptr == e2.ptr;
}

static int int_cmp_function(elem_t a, elem_t b)
{
    return a.i - b.i;
}

static int int_hash(elem_t e)
{
    return e.i;
}

ioopm_hash_table_t *cart_database_create()
{
    return ioopm_hash_table_create(int_hash, ptr_eq, int_cmp_function);
}

cart_t *cart_lookup(ioopm_hash_table_t *cart_db, size_t cart_id)
{
    elem_t *cart_elem = ioopm_hash_table_lookup(cart_db, int_elem(cart_id));
    if (cart_elem)
    {
        return cart_elem->ptr;
    }
    return NULL;
}

static void reservation_destroy(elem_t *reservation, void *extra)
{
    assert(reservation);
    reservation_t *good = reservation->ptr;
    free(good);
}

static void cart_destroy(elem_t key, elem_t *value, void *extra)
{
    cart_t *cart = value->ptr;
    assert(cart);
    ioopm_list_t *reservs = cart->reservations;
    ioopm_linked_list_apply_to_all(reservs, reservation_destroy, NULL);
    ioopm_linked_list_destroy(reservs);
    free(cart);
}

void cart_remove(ioopm_hash_table_t *cart_db, ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, int cart_id)
{
    cart_t *cart = cart_lookup(cart_db, cart_id);
    if (!cart)
    {
        return;
    }

    ioopm_list_t *reservs_list = cart->reservations;

    ioopm_list_iterator_t *reservs_iter = ioopm_list_iterator(reservs_list);
    option_t reserv_option = ioopm_iterator_current(reservs_iter);

    while (reserv_option.success)
    {
        reservation_t *reserv = reserv_option.value.ptr;
        char *name = reserv->name;
        int quantity = reserv->quantity;
        stock_restoration(merch_db, id_db, name, quantity);
        reserv_option = ioopm_iterator_next(reservs_iter);
    }

    ioopm_iterator_destroy(reservs_iter);

    cart_destroy(int_elem(cart_id), &ptr_elem(cart), NULL);
    ioopm_hash_table_remove(cart_db, int_elem(cart_id));
}

void cart_database_destroy(ioopm_hash_table_t *cart_db)
{
    ioopm_hash_table_apply_to_all(cart_db, cart_destroy, NULL);
    ioopm_hash_table_destroy(cart_db);
}

void cart_create(ioopm_hash_table_t *cart_db, size_t cart_id)
{
    cart_t *new_cart = calloc(1, sizeof(cart_t));
    ioopm_list_t *empty_reservs = ioopm_linked_list_create(ptr_eq);
    new_cart->id = cart_id;
    new_cart->reservations = empty_reservs;
    ioopm_hash_table_insert(cart_db, int_elem(cart_id), ptr_elem(new_cart));
}

void cart_add_to(ioopm_hash_table_t *cart_db, int cart_id, char *name, size_t quantity)
{
    cart_t *cart = cart_lookup(cart_db, cart_id);
    if (!cart)
    {
        return;
    }

    ioopm_list_t *reservs_list = cart->reservations;

    ioopm_list_iterator_t *reservs_iter = ioopm_list_iterator(reservs_list);
    option_t reserv_option = ioopm_iterator_current(reservs_iter);

    while (reserv_option.success)
    {
        reservation_t *reserv = reserv_option.value.ptr;
        if (!strcmp(reserv->name, name))
        {
            reserv->quantity += quantity;
            ioopm_iterator_destroy(reservs_iter);
            return;
        }
        reserv_option = ioopm_iterator_next(reservs_iter);
    }

    ioopm_iterator_destroy(reservs_iter);
    reservation_t reserv = {.name = name, .quantity = quantity};
    reservation_t *reserv_alloc = calloc(1, sizeof(reservation_t));
    *reserv_alloc = reserv;
    ioopm_linked_list_append(reservs_list, ptr_elem(reserv_alloc));
}

bool cart_remove_from(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, char *name, int cart_id, size_t quantity)
{
    cart_t *cart = cart_lookup(cart_db, cart_id);
    if (!cart)
    {
        return false;
    }

    ioopm_list_t *reservs_list = cart->reservations;

    ioopm_list_iterator_t *reservs_iter = ioopm_list_iterator(reservs_list);
    option_t reserv_option = ioopm_iterator_current(reservs_iter);

    while (reserv_option.success)
    {
        reservation_t *reserv = reserv_option.value.ptr;
        if (!strcmp(reserv->name, name) && reserv->quantity >= quantity)
        {
            reserv->quantity -= quantity;
            stock_restoration(merch_db, id_db, name, quantity);
            ioopm_iterator_destroy(reservs_iter);
            return true;
        }
        reserv_option = ioopm_iterator_next(reservs_iter);
    }

    ioopm_iterator_destroy(reservs_iter);
    return false;
}

int cart_calculate(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, int cart_id)
{
    cart_t *cart = cart_lookup(cart_db, cart_id);
    if (!cart)
    {
        return 0;
    }

    ioopm_list_t *reservs_list = cart->reservations;

    ioopm_list_iterator_t *reservs_iter = ioopm_list_iterator(reservs_list);
    option_t reserv_option = ioopm_iterator_current(reservs_iter);
    int total = 0;

    while (reserv_option.success)
    {
        reservation_t *reserv = reserv_option.value.ptr;

        char *name = reserv->name;
        int quantity = reserv->quantity;

        int price_res = 0;
        bool price = merch_get_price(merch_db, id_db, name, &price_res);

        if (price)
        {
            total += (price_res * quantity);
        }

        reserv_option = ioopm_iterator_next(reservs_iter);
    }

    ioopm_iterator_destroy(reservs_iter);
    return total;
}

void cart_checkout(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *locs_db,
                   ioopm_hash_table_t *cart_db, int cart_id)
{
    cart_t *cart = cart_lookup(cart_db, cart_id);
    if (!cart)
    {
        return;
    }

    ioopm_list_t *reservs = cart->reservations;

    ioopm_list_iterator_t *reservs_iter = ioopm_list_iterator(reservs);
    option_t reserv_option = ioopm_iterator_current(reservs_iter);

    while (reserv_option.success)
    {
        reservation_t *reserv = reserv_option.value.ptr;

        char *name = reserv->name;
        int quantity = reserv->quantity;

        stock_decrease(merch_db, id_db, locs_db, name, quantity);

        reserv_option = ioopm_iterator_next(reservs_iter);
    }

    ioopm_iterator_destroy(reservs_iter);
    cart_destroy(int_elem(cart_id), &ptr_elem(cart), NULL);
    ioopm_hash_table_remove(cart_db, int_elem(cart_id));
}
