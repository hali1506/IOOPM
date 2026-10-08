#include <CUnit/Basic.h>
#include "hash_table.h"
#include "cart.h"

void test_create_destroy(){
    ioopm_hash_table_t *db = cart_database_create();
    cart_create(db, 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    cart_database_destroy(db);
}

void test_add_and_remove(){
    ioopm_hash_table_t *db = cart_database_create();
    cart_create(db, 1);
    CU_ASSERT_EQUAL(ioopm_hash_table_size(db), 1);
    ioopm_list_t *reservations = ioopm_hash_table_lookup(db, int_elem(1))->ptr;
    cart_destroy(int_elem(1), &ptr_elem(reservations), NULL);
    cart_database_destroy(db);
}

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


int main() {
	if (CU_initialize_registry() != CUE_SUCCESS)
		return CU_get_error();
	CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
	if (my_test_suite == NULL) {
		CU_cleanup_registry();
		return CU_get_error();
	}
    if((CU_add_test(my_test_suite, "Create Destroy", test_create_destroy) == NULL) || 
        (CU_add_test(my_test_suite, "Create Destroy", test_add_and_remove) == NULL) || 
    0) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
  	CU_basic_run_tests();
  	CU_cleanup_registry();
 	return CU_get_error();
}