#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include "database.h"
#include "cart.h"
#include <stdio.h>
#include <string.h>

int init_suite(void)
{
    // Change this function if you want to do something *before* you
    // run a test suite
    return 0;
}

int clean_suite(void)
{
    // Change this function if you want to do something *after* you
    // run a test suite
    return 0;
}

void test_create_destroy()
{
    ioopm_hash_table_t *db = merch_database_create();
    CU_ASSERT_PTR_NOT_NULL(db);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 0);
    merch_database_destroy(db);
}

void test_insert_once()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *test = "HEJ";
    int stock = 0;

    CU_ASSERT_PTR_NULL(merch_get_name(db, id_db, test));
    CU_ASSERT_PTR_NULL(merch_get_description(db, id_db, test));
    CU_ASSERT_PTR_NULL(merch_get_locations(db, id_db, test));

    CU_ASSERT_PTR_NULL(merch_get_name(db, id_db, name));
    CU_ASSERT_PTR_NULL(merch_get_description(db, id_db, name));
    CU_ASSERT_FALSE(merch_get_price(db, id_db, name, &price));
    CU_ASSERT_FALSE(merch_get_stock(db, id_db, name, &stock));
    CU_ASSERT_FALSE(merch_get_available_stock(db, id_db, name, &stock));
    CU_ASSERT_PTR_NULL(merch_get_locations(db, id_db, name));

    merch_insert(db, id_db, name, desc, price);

    CU_ASSERT_TRUE(ioopm_hash_table_has_key(id_db, ptr_elem("bibeln")));
    CU_ASSERT_TRUE(ioopm_hash_table_has_key(db, *ioopm_hash_table_lookup(id_db, ptr_elem("bibeln"))));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    merch_t *merch = merch_lookup(db, id_db, name);
    CU_ASSERT(ioopm_hash_table_has_value(db, ptr_elem(merch)));

    id_database_destroy(id_db);
    merch_database_destroy(db);
}

void test_insert_existing()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;

    merch_insert(db, id_db, name, desc, price);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(id_db), 1);

    merch_insert(db, id_db, name, desc, price);

    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(id_db), 1);

    id_database_destroy(id_db);
    merch_database_destroy(db);
}

void test_merch_lookup()
{
    ioopm_hash_table_t *merch_db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;

    merch_insert(merch_db, id_db, name, desc, price);

    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(merch_db, *ioopm_hash_table_lookup(id_db, ptr_elem(name)))->ptr,
                    merch_lookup(merch_db, id_db, name));

    CU_ASSERT_STRING_EQUAL(merch_get_name(merch_db, id_db, name), name);
    CU_ASSERT_STRING_EQUAL(merch_get_description(merch_db, id_db, name), desc)

    id_database_destroy(id_db);
    merch_database_destroy(merch_db);
}

void test_remove()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char *name = "bb";
    char *desc = "bok";
    char *name2 = "aa";
    int price = 300.2;

    merch_remove(db, id_db, name2);
    merch_insert(db, id_db, name, desc, price);
    merch_insert(db, id_db, name2, desc, price);

    CU_ASSERT_PTR_NOT_NULL(ioopm_hash_table_lookup(id_db, ptr_elem("aa")));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 2);

    merch_remove(db, id_db, name2);

    CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(id_db, ptr_elem("aa")));
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);

    id_database_destroy(id_db);
    merch_database_destroy(db);
}

void test_merch_name_sort()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char **no_names = merch_name_sort(db);
    free(no_names);

    char *name1 = "bibeln";
    char *desc1 = "bok";
    char *name2 = "a";
    char *desc2 = "bok";
    char *name3 = "c";
    char *desc3 = "bok";

    int price = 300.2;
    merch_insert(db, id_db, name1, desc1, price);
    merch_insert(db, id_db, name2, desc2, price);
    merch_insert(db, id_db, name3, desc3, price);

    char **arr = merch_name_sort(id_db);
    CU_ASSERT(!strcmp(arr[0], name2));
    CU_ASSERT(!strcmp(arr[1], name1));
    CU_ASSERT(!strcmp(arr[2], name3));

    id_database_destroy(id_db);
    merch_database_destroy(db);
    free(arr);
}

void test_replenish()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *shelf = "A22";
    int increase = 2;
    int stock = 10;

    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);
    merch_insert(db, id_db, name, desc, price);

    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    CU_ASSERT(!strcmp(ioopm_hash_table_lookup(locs_db, ptr_elem(shelf))->ptr, name));
    CU_ASSERT(ioopm_hash_table_has_key(locs_db, ptr_elem(shelf)));
    CU_ASSERT(ioopm_hash_table_has_value(locs_db, ptr_elem(name)));

    merch_get_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 0);
    merch_replenish(db, id_db, name, shelf, increase);
    merch_get_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 2);

    id_database_destroy(id_db);
    merch_database_destroy(db);
    locations_database_destroy(locs_db);
}

void test_show_stock()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();

    char *name1 = "bibeln";
    char *desc1 = "bok";

    int price = 300.2;

    char *shelf1 = "A22";
    char *shelf2 = "B22";

    merch_insert(db, id_db, name1, desc1, price);

    shelf_t **shelfs = merch_show_stock(db, id_db, "TEST ATT SÖKA PÅ NAMN SOM INTE FINNS");
    CU_ASSERT_PTR_NULL(shelfs);
    shelfs = merch_show_stock(db, id_db, name1);
    CU_ASSERT_PTR_NULL(shelfs);

    int test_int = 0;
    CU_ASSERT_PTR_NULL(shelf_get_name(NULL));
    CU_ASSERT_FALSE(shelf_get_quantity(NULL, &test_int));

    merch_shelf_insert(db, id_db, locs_db, shelf1, name1);
    merch_shelf_insert(db, id_db, locs_db, shelf2, name1);

    int increase = 2;

    merch_replenish(db, id_db, name1, shelf1, increase);
    merch_replenish(db, id_db, name1, shelf2, increase + 1);

    shelfs = merch_show_stock(db, id_db, name1);
    CU_ASSERT_PTR_NOT_NULL(shelfs);
    CU_ASSERT_TRUE(!strcmp(shelf_get_name(shelfs[0]), shelf1));
    CU_ASSERT_TRUE(!strcmp(shelf_get_name(shelfs[1]), shelf2));
    CU_ASSERT_TRUE(shelf_get_quantity(shelfs[0], &test_int));
    CU_ASSERT_EQUAL(test_int, 2);
    CU_ASSERT_TRUE(shelf_get_quantity(shelfs[1], &test_int));
    CU_ASSERT_EQUAL(test_int, 3);

    free(shelfs);
    id_database_destroy(id_db);
    merch_database_destroy(db);
    locations_database_destroy(locs_db);
}

void test_edit()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300;
    char *name_new = "LoR";
    char *desc_new = "Saga";
    int price_new = 100;
    int price_test = 0;
    int stock = 10;

    merch_insert(db, id_db, name, desc, price);
    int old_id = ioopm_hash_table_lookup(id_db, ptr_elem(name))->i;

    merch_edit(db, id_db, name_new, NULL, NULL, 0);

    merch_edit(db, id_db, name, name_new, desc_new, price_new);

    CU_ASSERT_EQUAL(ioopm_hash_table_lookup(id_db, ptr_elem(name_new))->i, old_id);
    CU_ASSERT_TRUE(!strcmp(merch_get_name(db, id_db, name_new), name_new));
    CU_ASSERT_TRUE(!strcmp(merch_get_description(db, id_db, name_new), desc_new));
    CU_ASSERT_TRUE(merch_get_price(db, id_db, name_new, &price_test));
    CU_ASSERT_EQUAL(price_test, price_new);
    merch_get_stock(db, id_db, name_new, &stock);
    CU_ASSERT_EQUAL(stock, 0);
    merch_get_available_stock(db, id_db, name_new, &stock);
    CU_ASSERT_EQUAL(stock, 0);
    CU_ASSERT_PTR_NOT_NULL(merch_get_locations(db, id_db, name_new));

    id_database_destroy(id_db);
    merch_database_destroy(db);
}

void test_create_destroy_cart()
{
    ioopm_hash_table_t *db = cart_database_create();
    cart_create(db, 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    cart_database_destroy(db);
}

void test_add_and_remove()
{
    ioopm_hash_table_t *db = cart_database_create();
    cart_create(db, 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    cart_t *cart = cart_lookup(db, 1);
    CU_ASSERT_PTR_NOT_NULL(cart);
    cart_database_destroy(db);
}

void test_reservation()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *shelf = "A22";
    int increase = 3;
    int cart_id = 1;
    int quantity = 2;
    int stock = 0;

    cart_add_to(cart_db, cart_id, name, quantity);
    cart_create(cart_db, cart_id);
    merch_insert(db, id_db, name, desc, price);
    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);

    merch_get_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 3);

    stock_reservation(db, id_db, cart_db, name, cart_id, quantity);
    cart_add_to(cart_db, cart_id, name, quantity);
    cart_add_to(cart_db, cart_id, "HEJ", quantity);

    merch_get_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 3);
    merch_get_available_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 1);
    merch_remove(db, id_db, name);

    merch_database_destroy(db);
    id_database_destroy(id_db);
    locations_database_destroy(locs_db);
    cart_database_destroy(cart_db);
}

void test_remove_from_cart()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *shelf = "A22";
    int increase = 2;
    int cart_id = 1;
    int quantity = 1;
    int stock = 0;

    cart_remove_from(db, id_db, cart_db, name, cart_id, quantity);
    cart_create(cart_db, cart_id);
    merch_insert(db, id_db, name, desc, price);
    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);
    stock_reservation(db, id_db, cart_db, name, cart_id, quantity);

    cart_remove_from(db, id_db, cart_db, name, cart_id, quantity);
    cart_remove_from(db, id_db, cart_db, "HEJ", cart_id, quantity);

    merch_get_available_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 2);
    merch_get_stock(db, id_db, name, &stock);
    CU_ASSERT_EQUAL(stock, 2);

    merch_database_destroy(db);
    id_database_destroy(id_db);
    locations_database_destroy(locs_db);
    cart_database_destroy(cart_db);
}

void test_delete_cart()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *shelf = "A22";
    int increase = 2;
    int cart_id = 1;
    int quantity = 1;
    int stock = 0;

    cart_remove(cart_db, db, id_db, cart_id);
    cart_create(cart_db, cart_id);
    merch_insert(db, id_db, name, desc, price);
    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);
    stock_reservation(db, id_db, cart_db, name, cart_id, quantity);

    cart_remove(cart_db, db, id_db, cart_id);
    merch_get_available_stock(db, id_db, name, &stock);

    CU_ASSERT_EQUAL(stock, 2);

    merch_get_stock(db, id_db, name, &stock);

    CU_ASSERT_EQUAL(stock, 2);

    merch_database_destroy(db);
    id_database_destroy(id_db);
    locations_database_destroy(locs_db);
    cart_database_destroy(cart_db);
}

void test_cost()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 5;
    char *shelf = "A22";
    int increase = 2;
    int cart_id = 1;
    int quantity = 2;

    cart_calculate(db, id_db, cart_db, cart_id);
    cart_create(cart_db, cart_id);
    merch_insert(db, id_db, name, desc, price);
    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);
    stock_reservation(db, id_db, cart_db, name, cart_id, quantity);

    CU_ASSERT_EQUAL(cart_calculate(db, id_db, cart_db, cart_id), price * 2);

    merch_database_destroy(db);
    id_database_destroy(id_db);
    locations_database_destroy(locs_db);
    cart_database_destroy(cart_db);
}

void test_checkout()
{
    ioopm_hash_table_t *db = merch_database_create();
    ioopm_hash_table_t *id_db = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();

    char *name = "bibeln";
    char *desc = "bok";
    int price = 300.2;
    char *shelf = "A22";
    int increase = 2;
    int cart_id = 1;
    int quantity = 1;
    int stock = 0;

    stock_decrease(db, id_db, locs_db, name, quantity);
    cart_checkout(db, id_db, locs_db, cart_db, cart_id);
    cart_create(cart_db, cart_id);
    merch_insert(db, id_db, name, desc, price);
    merch_shelf_insert(db, id_db, locs_db, shelf, name);
    merch_replenish(db, id_db, name, shelf, increase);
    stock_reservation(db, id_db, cart_db, name, cart_id, quantity);

    cart_checkout(db, id_db, locs_db, cart_db, cart_id);
    merch_get_available_stock(db, id_db, name, &stock);

    CU_ASSERT_EQUAL(stock, 1);

    merch_get_stock(db, id_db, name, &stock);

    CU_ASSERT_EQUAL(stock, 1);

    stock_decrease(db, id_db, locs_db, name, 100);

    merch_database_destroy(db);
    id_database_destroy(id_db);
    locations_database_destroy(locs_db);
    cart_database_destroy(cart_db);
}

int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite data_suite = CU_add_suite("Database suite", init_suite, clean_suite);
    CU_pSuite cart_suite = CU_add_suite("Cart suite", init_suite, clean_suite);
    if (data_suite == NULL || cart_suite == NULL)
    {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        (CU_add_test(data_suite, "Create Destroy", test_create_destroy) == NULL) ||
        (CU_add_test(data_suite, "Insert once", test_insert_once) == NULL) ||
        (CU_add_test(data_suite, "Insert existing", test_insert_existing) == NULL) ||
        (CU_add_test(data_suite, "Merch lookup", test_merch_lookup) == NULL) ||
        (CU_add_test(data_suite, "Remove", test_remove) == NULL) ||
        (CU_add_test(data_suite, "Sort names", test_merch_name_sort) == NULL) ||
        (CU_add_test(data_suite, "Replenish", test_replenish) == NULL) ||
        (CU_add_test(data_suite, "Show stock", test_show_stock) == NULL) ||
        (CU_add_test(data_suite, "Edit", test_edit) == NULL) ||

        (CU_add_test(cart_suite, "Create Destroy", test_create_destroy_cart) == NULL) ||
        (CU_add_test(cart_suite, "Add and Remove", test_add_and_remove) == NULL) ||
        (CU_add_test(cart_suite, "Add reservation", test_reservation) == NULL) ||
        (CU_add_test(cart_suite, "Remove from cart", test_remove_from_cart) == NULL) ||
        (CU_add_test(cart_suite, "Delete cart", test_delete_cart) == NULL) ||
        (CU_add_test(cart_suite, "Cost cart", test_cost) == NULL) ||
        (CU_add_test(cart_suite, "Checkout", test_checkout) == NULL) ||
        0)
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}