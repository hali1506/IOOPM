#include "linked_list.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "iterator.h"
#include <string.h>
/// A link in the linked structure
typedef struct link link_t;
struct link
{
    elem_t element;
    link_t *next;
};

struct list
{
    ioopm_eq_function eq_fun;
    link_t *first;
    link_t *last;
    int size;
};

struct iterator
{
    link_t *current;
    ioopm_list_t *list;
};

ioopm_list_t *ioopm_linked_list_create(ioopm_eq_function eq_fun)
{
    ioopm_list_t *list = calloc(1, sizeof(ioopm_list_t));
    link_t *new_ptr = calloc(1, sizeof(link_t));
    link_t new_link = {.next = NULL, .element = ptr_elem(NULL)};
    *new_ptr = new_link;
    list->first = new_ptr;
    list->eq_fun = eq_fun;
    return list;
}

void link_destroy(link_t *link)
{
    if (link->next != NULL)
    {
        link_destroy(link->next);
    }
    free(link);
}

void ioopm_linked_list_clear(ioopm_list_t *list)
{
    link_destroy(list->first);
    list->size = 0;
}

void ioopm_linked_list_destroy(ioopm_list_t *list)
{
    ioopm_linked_list_clear(list);
    free(list);
}

void ioopm_linked_list_append(ioopm_list_t *list, elem_t value)
{
    link_t *new_ptr = calloc(1, sizeof(link_t));
    link_t new_link = {.next = NULL, .element = value};
    *new_ptr = new_link;
    if (list->size == 0)
    {
        list->last = new_ptr;
        list->first->next = new_ptr;
    }
    else
    {
        list->last->next = new_ptr;
        list->last = new_ptr;
    }
    list->size++;
}

void ioopm_linked_list_prepend(ioopm_list_t *list, elem_t value)
{
    if (list->size == 0)
    {
        link_t *new_ptr = calloc(1, sizeof(link_t));
        link_t new_link = {.next = NULL, .element = value};
        *new_ptr = new_link;
        list->last = new_ptr;
        list->first->next = new_ptr;
    }
    else
    {
        link_t *new_ptr = calloc(1, sizeof(link_t));
        link_t new_link = {.next = list->first->next, .element = value};
        *new_ptr = new_link;
        list->first->next = new_ptr;
    }
    list->size++;
}

int index_adjust(int index, int size)
{
    if (index > size)
    {
        return size;
    }
    else if (index < 0)
    {
        return 0;
    }
    return index;
}

void ioopm_linked_list_insert(ioopm_list_t *list, int index, elem_t value)
{
    index_adjust(index, list->size);
    if (index == list->size)
    {
        ioopm_linked_list_append(list, value);
    }
    else if (index == 0)
    {
        ioopm_linked_list_prepend(list, value);
    }
    link_t *current_link = list->first;
    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }
    link_t *new_ptr = calloc(1, sizeof(link_t));
    link_t new_link = {.element = value, .next = current_link->next};
    *new_ptr = new_link;
    current_link->next = new_ptr;
    list->size++;
}

option_t ioopm_linked_list_remove(ioopm_list_t *list, int index)
{
    if (index > list->size - 1 || index < 0)
    {
        return Failure();
    }

    link_t *current_link = list->first;

    for (int i = 0; i < index; i++)
    {
        current_link = current_link->next;
    }
    link_t *tmp;
    if (index == list->size - 1)
    {
        list->last = current_link;
        tmp = NULL;
    }
    else
    {
        tmp = current_link->next->next;
    }
    elem_t removed = current_link->next->element;
    free(current_link->next);
    current_link->next = tmp;
    list->size--;
    return Success(removed);
}

option_t ioopm_linked_list_get(ioopm_list_t *list, int index)
{
    if (index > list->size - 1 || index < 0)
    {
        return Failure();
    }
    if (index == list->size - 1)
    {
        return Success(list->last->element);
    }

    link_t *current_link = list->first;

    for (int i = 0; i < index + 1; i++)
    {
        current_link = current_link->next;
    }
    return Success(current_link->element);
}

bool compare_str(int index, elem_t element, void *compare)
{
    return !strcmp(element.ptr, (char *)compare);
}

bool compare_int(int index, elem_t element, void *compare)
{
    int compare_int = ((elem_t *)compare)->i;
    return element.i == compare_int;
}

bool ioopm_linked_list_contains(ioopm_list_t *list, elem_t element)
{
    bool result = false;
    link_t *current_link = list->first->next;
    for (int i = 0; i < list->size; i++)
    {
        if (list->eq_fun(current_link->element, element))
        {
            result = true;
        }
        current_link = current_link->next;
    }
    return result;
}

int ioopm_linked_list_size(ioopm_list_t *list)
{
    return list->size;
}

bool ioopm_linked_list_is_empty(ioopm_list_t *list)
{
    return list->size == 0;
}

bool ioopm_linked_list_all(ioopm_list_t *list, ioopm_elem_predicate prop, void *extra)
{
    bool result = true;
    link_t *current_link = list->first->next;
    for (int i = 0; i < list->size; i++)
    {
        if (!prop(current_link->element, extra))
        {
            result = false;
        }
        current_link = current_link->next;
    }
    return result;
}

bool ioopm_linked_list_any(ioopm_list_t *list, ioopm_elem_predicate prop, void *extra)
{
    bool result = false;
    link_t *current_link = list->first->next;
    for (int i = 0; i < list->size; i++)
    {
        if (prop(current_link->element, extra))
        {
            result = true;
        }
        current_link = current_link->next;
    }
    return result;
}

void ioopm_linked_list_apply_to_all(ioopm_list_t *list, ioopm_apply_elem_function fun, void *extra)
{
    link_t *current_link = list->first->next;
    for (int i = 0; i < list->size; i++)
    {
        fun(&current_link->element, extra);
        current_link = current_link->next;
    }
}

ioopm_list_iterator_t *ioopm_list_iterator(ioopm_list_t *list)
{
    if (list != NULL)
    {
        ioopm_list_iterator_t *new_ptr = calloc(1, sizeof(ioopm_list_iterator_t));
        ioopm_list_iterator_t new_iter = {.current = list->first->next, .list = list};
        *new_ptr = new_iter;
        return new_ptr;
    }
    return NULL;
}

option_t ioopm_iterator_has_next(ioopm_list_iterator_t *iter)
{
    if (iter->current != NULL)
    {
        return Success(int_elem(1));
    }
    return Failure();
}

option_t ioopm_iterator_next(ioopm_list_iterator_t *iter)
{
    if (iter->current != NULL)
    {
        iter->current = iter->current->next;
        if (iter->current != NULL)
        {
            return Success(iter->current->element);
        }
    }
    return Failure();
}

void ioopm_iterator_reset(ioopm_list_iterator_t *iter)
{
    iter->current = iter->list->first->next;
}

option_t ioopm_iterator_current(ioopm_list_iterator_t *iter)
{
    if (iter->current != NULL && iter->current != iter->list->first)
    {
        return Success(iter->current->element);
    }
    return Failure();
}

elem_t left_accumulate(link_t *link, ioopm_accumilate_function fun, elem_t initial_value)
{
    return link->next == NULL ? fun(link->element, initial_value)
                              : fun(link->element, left_accumulate(link->next, fun, initial_value));
}

elem_t ioopm_accumulate(ioopm_list_t *list, ioopm_accumilate_function fun, elem_t initial_value)
{
    if (list->first->next != NULL)
    {
        return left_accumulate(list->first, fun, initial_value);
    }
    return initial_value;
}

void ioopm_iterator_destroy(ioopm_list_iterator_t *iter)
{
    free(iter);
}