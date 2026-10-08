#include <stdbool.h>

#pragma once

/**
 * @brief A function that prints a prompt and returns a string from user input
 * 
 * @param question The prompt that the function prints
 * @return char* The string from the user
 */
char *ask_question_string(char *question);

/**
 * @brief A function that prints a prompt and returns a integer from user input
 * 
 * @param question The prompt that the function prints
 * @return int 
 */
int ask_question_int(char *question);

/**
 * @brief A function that prints a prompt and returns a float from user input
 * 
 * @param question The prompt that the function prints
 * @return double 
 */
double ask_question_float(char *question);

/**
 * @brief A function that prints a prompt and returns a shelf from user input
 * 
 * @param question The prompt that the function prints
 * @return char* The shelf is represented by one letter and an integer
 */
char *ask_question_shelf(char *question);

/**
 * @brief A function that prints a prompt and returns a specific character from user input
 * The character has should have an action linked to the corresponding menu
 * 
 * @param question The prompt that the function prints 
 * @return char A char that has an action in the menu
 */
char ask_question_menu(char *question);

/**
 * @brief A function that prints a prompt and returns a charachter from user input
 * 
 * @param question The prompt that the function prints 
 * @return char The first character from a string from the user
 */
char ask_question_continue(char *question);