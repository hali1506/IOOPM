#include <CUnit/Basic.h>
#include <stdlib.h>
#include "hash_table.h"
#include <string.h>
#include "linked_list.h"




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

// These are example test functions. You should replace them with
// functions of your own.

void test_create_destroy()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  CU_ASSERT_PTR_NOT_NULL(ht);
  ioopm_hash_table_destroy(ht);
}

void test_lookup_empty()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL , NULL);

  for (int i = -1; i < No_Buckets + 1; ++i)
  {
    CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(i)));
  }

  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(-1)));
  ioopm_hash_table_destroy(ht);
}

void test_insert_once()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL , NULL);
  int k = -1;
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(k)));
  char *v = "Hej";
  ioopm_hash_table_insert(ht, int_elem(k), ptr_elem(v));
  char *c = (*ioopm_hash_table_lookup(ht, int_elem(k))).ptr;
  CU_ASSERT_EQUAL(c, v);
  ioopm_hash_table_destroy(ht);
}

void test_insert_key_in_use()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k = 1;
  char *v1 = "Hej";
  ioopm_hash_table_insert(ht, int_elem(k), ptr_elem(v1));
  CU_ASSERT_STRING_EQUAL((*ioopm_hash_table_lookup(ht, int_elem(k))).ptr, v1);
  char *v2 = "Hej!";
  ioopm_hash_table_insert(ht, int_elem(k), ptr_elem(v2));
  CU_ASSERT_STRING_EQUAL((*ioopm_hash_table_lookup(ht, int_elem(k))).ptr, v2);
  ioopm_hash_table_destroy(ht);
}

void test_remove_existing_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k = 3;
  char *v = "Hej";
  ioopm_hash_table_insert(ht, int_elem(k), ptr_elem(v));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 1);
  elem_t val = ioopm_hash_table_remove(ht, int_elem(k));

  CU_ASSERT_STRING_EQUAL(val.ptr, v);
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(k)));
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_remove_non_last_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k1 = No_Buckets + 3;
  char *v = "Hej";
  int k2 = 2 * No_Buckets + 3;

  ioopm_hash_table_insert(ht, int_elem(k1), ptr_elem(v));
  ioopm_hash_table_insert(ht, int_elem(k2), ptr_elem(v));
  elem_t val1 = ioopm_hash_table_remove(ht, int_elem(k1));
  elem_t val2 = ioopm_hash_table_remove(ht, int_elem(k2));

  CU_ASSERT(!strcmp(v, val1.ptr));
  CU_ASSERT(!strcmp(v, val2.ptr));

  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT(ioopm_hash_table_is_empty(ht));

  ioopm_hash_table_destroy(ht);
}

void test_remove_non_existing_entry()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k = -3;
  ioopm_hash_table_remove(ht, int_elem(k));
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(k)));
  ioopm_hash_table_destroy(ht);
}

void test_clear_hash_table()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k = 1;
  char *v1 = "Hej";
  ioopm_hash_table_insert(ht, int_elem(k), ptr_elem(v1));
  ioopm_hash_table_clear(ht);
  CU_ASSERT_EQUAL(ioopm_hash_table_size(ht), 0);
  CU_ASSERT_PTR_NULL(ioopm_hash_table_lookup(ht, int_elem(k)));
  ioopm_hash_table_destroy(ht);
}

void test_get_keys()
{
  int keys[] = {1, 5, -6, 100, 4};
  char *value = "hej";
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, int_elem((int)keys[i]), ptr_elem(value));
  }

  bool found[5];
  ioopm_list_t *found_keys = ioopm_hash_table_keys(ht);

  for (int found_key = 0; found_key < 5; found_key++)
  {
    for (int old_key = 0; old_key < 5; old_key++)
    {
      if (ioopm_linked_list_get(found_keys,found_key).value.i == keys[old_key])
      {
        found[found_key] = true;
      }
    }
  }

  for (int i = 0; i < 5; i++)
  {
    CU_ASSERT(found[i]);
  }

  ioopm_linked_list_destroy(found_keys);
  ioopm_hash_table_destroy(ht);
}

void test_get_values()
{
  int keys[] = {1, 5, -23, -6, 4};
  char *values[] = {"hej", "hejdå", "ok", "nej", "tack"};
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);

  for (int i = 0; i < 5; i++)
  {
    ioopm_hash_table_insert(ht, int_elem((int)keys[i]), ptr_elem(values[i]));
  }

  bool found[5];
  ioopm_list_t *found_keys = ioopm_hash_table_keys(ht);
  ioopm_list_t *found_values = ioopm_hash_table_values(ht);

  for (int i = 0; i < 5; i++)
  {
    for (int j = 0; j < 5; j++)
    {
      if (ioopm_linked_list_get(found_keys,i).value.i == keys[j])
      {
        if (ioopm_linked_list_get(found_values,i).value.ptr == values[j])
        {
          found[j] = true;
        }
        else
        {
          CU_FAIL("Found a value that was never inserted!");
        }
      }
    }
  }

  for (int i = 0; i < 5; i++)
  {
    CU_ASSERT(found[i]);
  }

  ioopm_linked_list_destroy(found_values);
  ioopm_linked_list_destroy(found_keys);
  ioopm_hash_table_destroy(ht);
}

bool len_pred(elem_t key, elem_t value, void *extra)
{
  return strlen(value.ptr) > 3;
}

/*bool str_equal(elem_t key, elem_t value, void *extra)
{
  return !strcmp((char *)extra, (char *)value);
}*/

void test_hash_table_all()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  char *v1 = "Hej.";
  char *v2 = "Hej!";
  char *v3 = "Hej!!!";
  int k1 = -No_Buckets - 1;
  int k2 = -1;
  int k3 = -2 * No_Buckets - 1;
  ioopm_hash_table_insert(ht, int_elem(k1), ptr_elem(v1));
  ioopm_hash_table_insert(ht, int_elem(k2), ptr_elem(v2));
  ioopm_hash_table_insert(ht, int_elem(k3), ptr_elem(v3));

  CU_ASSERT(ioopm_hash_table_all(ht, len_pred, ""));
  ioopm_hash_table_destroy(ht);
}

void test_hash_table_any()
{ 
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  char *v1 = "Hej";
  char *v2 = "Hej!";
  char *v3 = "Hej!!!";
  int k1 = 0;
  int k2 = No_Buckets;
  int k3 = 2 * No_Buckets;
  ioopm_hash_table_insert(ht, int_elem(k1), ptr_elem(v1));
  ioopm_hash_table_insert(ht, int_elem(k2), ptr_elem(v2));
  ioopm_hash_table_insert(ht, int_elem(k3), ptr_elem(v3));

  CU_ASSERT(ioopm_hash_table_any(ht, len_pred, ""));
  ioopm_hash_table_destroy(ht);
}
bool str_equal(int key, char *value, void *extra)
{
  return !strcmp((char *)extra, value);
}

void set_all(elem_t key, elem_t *value, void *extra)
{
  (*value).ptr = (char *)extra;
}


bool str_comp(elem_t key, elem_t value, void *extra)
{
  return value.ptr  == (char *)extra;
}

void test_apply_to_all()
{
  char *str = "123";
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, NULL, NULL);
  int k1 = -18;
  char *v1 = "Hej1";
  int k2 = -1;
  char *v2 = "Hej2";
  int k3 = -35;
  char *v3 = "Hej3";
  ioopm_hash_table_insert(ht, int_elem(k1), ptr_elem(v1));
  ioopm_hash_table_insert(ht, int_elem(k2), ptr_elem(v2));
  ioopm_hash_table_insert(ht, int_elem(k3), ptr_elem(v3));

  ioopm_hash_table_apply_to_all(ht, set_all, str);

  CU_ASSERT(ioopm_hash_table_all(ht, str_comp, str));
  ioopm_hash_table_destroy(ht);
}


bool str_eq(elem_t a, elem_t b){
  return !strcmp(a.ptr,b.ptr);
}

void test_has_key_value()
{
  ioopm_hash_table_t *ht = ioopm_hash_table_create(NULL, str_eq, NULL);
  char *value = "hej";
  char *copy = strdup(value);
  char *other = "ok";
  int key = 0;
  ioopm_hash_table_insert(ht, int_elem(key), ptr_elem(value));
  CU_ASSERT(ioopm_hash_table_has_key(ht, int_elem(key)));
  CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem(value)));
  CU_ASSERT(ioopm_hash_table_has_value(ht, ptr_elem(copy)));
  CU_ASSERT(!ioopm_hash_table_has_value(ht, ptr_elem(other)));
  free(copy);
  ioopm_hash_table_destroy(ht);
}

int main()
{
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("My awesome test suite", init_suite, clean_suite);
  if (my_test_suite == NULL)
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
      (CU_add_test(my_test_suite, "Create Destroy", test_create_destroy) == NULL) ||
      (CU_add_test(my_test_suite, "Empty table is empty", test_lookup_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Insert Once", test_insert_once) == NULL) ||
      (CU_add_test(my_test_suite, "Key in use", test_insert_key_in_use) == NULL) ||
      (CU_add_test(my_test_suite, "Remove existing entry", test_remove_existing_entry) == NULL) ||
      (CU_add_test(my_test_suite, "Remove non last entry", test_remove_non_last_entry) == NULL) ||
      (CU_add_test(my_test_suite, "Remove non existing", test_remove_non_existing_entry) == NULL) ||
      (CU_add_test(my_test_suite, "Clear hash table", test_clear_hash_table) == NULL) ||
      (CU_add_test(my_test_suite, "Get keys", test_get_keys) == NULL) ||
      (CU_add_test(my_test_suite, "Get values", test_get_values) == NULL) ||
      (CU_add_test(my_test_suite, "All", test_hash_table_all) == NULL) ||
      (CU_add_test(my_test_suite, "Any", test_hash_table_any) == NULL) ||
      (CU_add_test(my_test_suite, "Apply to all", test_apply_to_all) == NULL) ||
      (CU_add_test(my_test_suite, "Has key value", test_has_key_value) == NULL) ||
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