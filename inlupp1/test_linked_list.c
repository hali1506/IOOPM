#include <CUnit/Basic.h>
#include <stdlib.h>
#include "linked_list.h"
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

bool int_eq(elem_t a, elem_t b){
  return a.i == b.i;
}
bool str_eq(elem_t a, elem_t b){
  return strcmp(a.ptr, b.ptr);
}
bool ptr_eq(elem_t a, elem_t b){
  return a.ptr == b.ptr;
}
bool bool_eq(elem_t a, elem_t b){
  return (a.b+b.b) % 2 == 0;
}

void test_create_destroy(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    CU_ASSERT_PTR_NOT_NULL(lst);
    ioopm_linked_list_destroy(lst);
}

void test_append_once(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element = 1;
    ioopm_linked_list_append(lst, int_elem(element));
    CU_ASSERT(ioopm_linked_list_contains(lst, int_elem(element)));
    ioopm_linked_list_remove(lst, 0);
    CU_ASSERT(!ioopm_linked_list_contains(lst, int_elem(element)));
    ioopm_linked_list_destroy(lst);
}

void test_append_prepend(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    ioopm_linked_list_append(lst, int_elem(element1));
    CU_ASSERT(ioopm_linked_list_contains(lst, int_elem(element1)));
    ioopm_linked_list_prepend(lst, int_elem(element2));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).value.i, element2);
    ioopm_linked_list_append(lst, int_elem(element3));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).value.i, element3);
    ioopm_linked_list_destroy(lst);
}

void test_remove_empty(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    ioopm_linked_list_remove(lst, 0);
    CU_ASSERT(!ioopm_linked_list_size(lst));
    ioopm_linked_list_destroy(lst);
}

void test_insert(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    int element4 = 4;
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).value.i, element2);
    ioopm_linked_list_destroy(lst);
}

void test_insert_str(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    char *element1 = "hej";
    char *element2 = "ok";
    char *element3 = "hejdå";
    int element4 = 4;
    ioopm_linked_list_append(lst, ptr_elem(element1));
    ioopm_linked_list_append(lst, ptr_elem(element2));
    ioopm_linked_list_append(lst, ptr_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).value.ptr, element2);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 3).value.i, element4);
    ioopm_linked_list_destroy(lst);
}



void test_size_clear_empty(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    int element4 = 4;
    ioopm_linked_list_append(lst, int_elem(element1));
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 1);
    ioopm_linked_list_remove(lst, 0);
    CU_ASSERT(!ioopm_linked_list_size(lst));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    ioopm_linked_list_clear(lst);
    CU_ASSERT_EQUAL(ioopm_linked_list_size(lst), 0);
    CU_ASSERT(ioopm_linked_list_is_empty(lst));
    ioopm_linked_list_destroy(lst);
}

bool check_even(int index, elem_t element, void *extra){
    return (element.i + 1) % 2;
}


void test_all(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    CU_ASSERT(ioopm_linked_list_all(lst, check_even, NULL));
    int element1 = 1;
    int element2 = 2;
    int element3 = 6;
    int element4 = 4;
    ioopm_linked_list_append(lst,int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    CU_ASSERT(!ioopm_linked_list_all(lst, check_even, NULL));
    ioopm_linked_list_remove(lst, 0);
    CU_ASSERT(ioopm_linked_list_all(lst, check_even, NULL));
    ioopm_linked_list_destroy(lst);
}

void test_any(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    CU_ASSERT(!ioopm_linked_list_any(lst, check_even, NULL));
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    int element4 = 5;
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    CU_ASSERT(ioopm_linked_list_any(lst, check_even, NULL));
    ioopm_linked_list_remove(lst, 1);
    CU_ASSERT(!ioopm_linked_list_any(lst, check_even, NULL));
    ioopm_linked_list_destroy(lst);
}

void multiply(int index, elem_t *element, void *extra){
    (*element).i *= *(int *)extra;
}

void test_apply_to_all(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    CU_ASSERT(!ioopm_linked_list_any(lst, check_even, NULL));
    int element1 = 1;
    int element2 = 2;
    int element3 = 6;
    int element4 = 4;
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_prepend(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_insert(lst, 2, int_elem(element4));
    int ex = 3;
    int *extra = &ex;
    ioopm_linked_list_apply_to_all(lst, multiply, extra);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 0).value.i, 6);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 1).value.i, 3);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).value.i, 12);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 3).value.i, 18);
    ioopm_linked_list_destroy(lst);
}

elem_t add(elem_t a, elem_t b){
  return int_elem(a.i+b.i);
}

void test_accumulate(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    CU_ASSERT(!ioopm_linked_list_any(lst, check_even, NULL));
    int element1 = 1;
    int element2 = 2;
    int element3 = 6;
    int element4 = 4;
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_append(lst, int_elem(element4));
    CU_ASSERT_EQUAL(ioopm_accumulate(lst,add, int_elem(0)).i, 13);
    ioopm_linked_list_destroy(lst);
}

int main() {
  // First we try to set up CUnit, and exit if we fail
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  // We then create an empty test suite and specify the name and
  // the init and cleanup functions
  CU_pSuite my_test_suite = CU_add_suite("Linked list suite", init_suite, clean_suite);
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
      (CU_add_test(my_test_suite, "Append once", test_append_once) == NULL) ||
      (CU_add_test(my_test_suite, "Append Prepend", test_append_prepend) == NULL) ||
      (CU_add_test(my_test_suite, "Remove empty", test_remove_empty) == NULL) ||
      (CU_add_test(my_test_suite, "Insert", test_insert) == NULL) ||
      (CU_add_test(my_test_suite, "Insert strings", test_insert_str) == NULL) ||
      (CU_add_test(my_test_suite, "All", test_all) == NULL) ||
      (CU_add_test(my_test_suite, "Any", test_any) == NULL) ||
      (CU_add_test(my_test_suite, "Apply to all", test_apply_to_all) == NULL) ||
      (CU_add_test(my_test_suite, "Accumulate", test_accumulate) == NULL) ||
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