#include "hash_table.h"
#include <stddef.h>
#pragma once

/**
 * @authors Hanna Lindmark and David Melin
 * @date 2023-10-31
 */

typedef struct reservation reservation_t;
typedef struct cart cart_t;

/**
 * @brief A function that creats a database for shoppingcarts. 
 * Note that the database will need to be destroyed when no longer needed.
 *
 * @return ioopm_hash_table_t* a hash table where key is an ID and value is a cart
 */
ioopm_hash_table_t *cart_database_create();

/**
 * @brief Adds a new cart to a cart db
 *
 * @param cart_db the db to add a new cart to
 * @param cart_id a specific number to that cart. A new ID should always be bigger than an old carts ID.
 */
void cart_create(ioopm_hash_table_t *cart_db, size_t cart_id);

/**
 * @brief Fetches information about a cart
 *
 * @param cart_db A hash table containing carts
 * @param cart_id ID of the cart to be fetsched
 * @return cart_t* The cart struct containing the information
 */
cart_t *cart_lookup(ioopm_hash_table_t *cart_db, size_t cart_id);

/**
 * @brief Removes a cart from the cart db, putting all the stock of a merch in the cart back to the shelfs
 *
 * @param cart_db A hash table containing carts
 * @param merch_db A db with merch to put the stock back into
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param cart_id ID of the cart to be removed
 */
void cart_remove(ioopm_hash_table_t *cart_db, ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, int cart_id);

/**
 * @brief Destroys a cart_db and frees all memory
 *
 * @param cart_db the db with carts to be destroyed
 */
void cart_database_destroy(ioopm_hash_table_t *cart_db);

/**
 * @brief Add an item to a cart as long as it has the quantity is lower then the available stock
 *
 * @param cart_db A hash table containing carts
 * @param cart_id ID of the cart to add to
 * @param name Name of merch to add to cart
 * @param quantity The number of merch to add to a cart
 */
void cart_add_to(ioopm_hash_table_t *cart_db, int cart_id, char *name, size_t quantity);

/**
 * @brief Removes a merch from a cart and puts it stock back to the shelfs
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param cart_db A hash table containing carts
 * @param name Name of the the merch to be removed from cart
 * @param cart_id ID of the cart to remove merch from
 * @param quantity The number of stock to remove from cart
 * @return true if successful
 * @return false if not successful
 */
bool cart_remove_from(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, char *name, int cart_id, size_t quantity);

/**
 * @brief Calculates the cost of a cart
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param cart_db A hash table containing carts
 * @param cart_id ID of the cart to have its cost calculated
 * @return int The cost that have been calculated from a cart
 */
int cart_calculate(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, int cart_id);

/**
 * @brief Removes the cart and decreases the stock from merch db and the shelfs
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param locs_db The database which maps the name of a shelf to the name of a merch
 * @param cart_db A hash table containing carts
 * @param cart_id ID of the cart to check out
 */
void cart_checkout(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *locs_db, ioopm_hash_table_t *cart_db, int cart_id);