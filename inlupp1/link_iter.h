#pragma once
#include <stdbool.h>

typedef struct option option_t;
typedef union elem elem_t;
typedef bool(*ioopm_eq_function)(elem_t a, elem_t b);

#define Success(v) (option_t) {.success = true, .value = v };
#define Failure() (option_t) {.success = false};
#define int_elem(x) (elem_t) { .i=(x) }
#define ptr_elem(x) (elem_t) { .ptr=(x) }

union elem{
  int i;
  bool b;
  float f;
  void *ptr;
};


struct option{
  bool success;
  elem_t value;
};
