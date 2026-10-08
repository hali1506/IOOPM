#include <CUnit/Basic.h>
#include <stdlib.h>
#include "linked_list.h"
#include <string.h>
#include "iterator.h"


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
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lst);
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst,0).value.ptr, ioopm_iterator_current(iter).value.ptr);
    ioopm_linked_list_destroy(lst);
    ioopm_iterator_destroy(iter);
}

void test_has_next(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element = 1;
    ioopm_linked_list_append(lst, int_elem(element));
    ioopm_linked_list_append(lst, int_elem(element));
    ioopm_list_iterator_t  *iter = ioopm_list_iterator(lst);
    CU_ASSERT(ioopm_iterator_has_next(iter).value.b);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter).value.i, 1);
    CU_ASSERT(!ioopm_iterator_has_next(iter).value.b);
    ioopm_iterator_next(iter);
    CU_ASSERT(!ioopm_iterator_has_next(iter).success);
    ioopm_linked_list_destroy(lst);
    ioopm_iterator_destroy(iter);
}

void test_current(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    ioopm_list_iterator_t *iter_empty = ioopm_list_iterator(lst);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter_empty).value.i, NULL);
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lst);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.i, 1);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter).value.i, 2);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.i, 2);
    ioopm_linked_list_destroy(lst);
    ioopm_iterator_destroy(iter);
    ioopm_iterator_destroy(iter_empty);
}

void test_reset(){
    ioopm_list_t *lst = ioopm_linked_list_create(int_eq);
    int element1 = 1;
    int element2 = 2;
    int element3 = 3;
    ioopm_linked_list_append(lst, int_elem(element1));
    ioopm_linked_list_append(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_list_iterator_t *iter = ioopm_list_iterator(lst);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter).value.i, 2);
    CU_ASSERT_EQUAL(ioopm_iterator_next(iter).value.i, 3);
    ioopm_iterator_reset(iter);
    CU_ASSERT_EQUAL(ioopm_iterator_current(iter).value.i, 1);
    ioopm_iterator_destroy(iter);
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
    ioopm_linked_list_prepend(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_insert(lst, 2, int_elem(element4));
    CU_ASSERT_EQUAL(ioopm_linked_list_get(lst, 2).value.i, element4);
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
    ioopm_linked_list_prepend(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_insert(lst, 2, int_elem(element4));
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
    ioopm_linked_list_prepend(lst, int_elem(element2));
    ioopm_linked_list_append(lst, int_elem(element3));
    ioopm_linked_list_insert(lst, 2, int_elem(element4));
    CU_ASSERT(!ioopm_linked_list_all(lst, check_even, NULL));
    ioopm_linked_list_remove(lst, 1);
    CU_ASSERT(ioopm_linked_list_all(lst, check_even, NULL));
    ioopm_linked_list_destroy(lst);
}

void test_any(){
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
    CU_ASSERT(!ioopm_linked_list_any(lst, check_even, NULL));
    ioopm_linked_list_remove(lst, 1);
    CU_ASSERT(ioopm_linked_list_any(lst, check_even, NULL));
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
    (CU_add_test(my_test_suite, "Has next", test_has_next) == NULL) ||  
    (CU_add_test(my_test_suite, "Reset", test_reset) == NULL) ||
    (CU_add_test(my_test_suite, "Current", test_current) == NULL) ||
    0
  )
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