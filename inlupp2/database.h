#include "hash_table.h"
#include <stddef.h>
#pragma once

/**
 * @authors Hanna Lindmark and David Melin
 * @date 2023-10-31
 */

typedef struct merch merch_t;
typedef struct shelf shelf_t;

/**
 * @brief Creates a hash table containing merch id
 * Note that the database will need to be destroyed when no longer needed.
 *
 * @return ioopm_hash_table_t* A empty hash table with merch names as keys and merch ids as values
 */
ioopm_hash_table_t *id_database_create();

/**
 * @brief Creates a database storing merch information
 * Note that the database will need to be destroyed when no longer needed.
 *
 * @return ioopm_hash_table_t* An empty merch hash table with merch id as key and
 *          merch information as value
 */
ioopm_hash_table_t *merch_database_create();

/**
 * @brief Creates a database storing information about what items are stored at each location
 * Note that the database will need to be destroyed when no longer needed.
 *
 * @return ioopm_hash_table_t* An empty location hash table with shelf name as key
 *          and merch name of the merch stored at that location as value
 */
ioopm_hash_table_t *location_database_create();

/**
 * @brief Fetches information about a merch
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @return merch_t* The merch struct containing the information
 */
merch_t *merch_lookup(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name);

/**
 * @brief Fetches the name of a merch
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @return char* The merch name or NULL if the merch does not exist
 */
char *merch_get_name(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name);

/**
 * @brief Fetches the description of a merch
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param name The name of the merch
 * @return char* The description of the merch or NULL if the merch does not exist
 */
char *merch_get_description(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name);

/**
 * @brief Fetches the price of a merch
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @param price An int where the price will be stored if the fetch is successful
 * @return true If the merch exists
 * @return false If the merch does not exist
 */
bool merch_get_price(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name, int *price);

/**
 * @brief Fetches the total number of a merch
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @param stock An int where the stock will be stored if the fetch is successful
 * @return true If the merch exists
 * @return false If the merch does not exist
 */
bool merch_get_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name, int *stock);

/**
 * @brief Fetches the number of merch items which are not in a cart
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @param stock An int where the available stock will be stored if the fetch is successful
 * @return true If the merch exists
 * @return false If the merch does not exist
 */
bool merch_get_available_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name, int *available_stock);

/**
 * @brief Fetches information about the locations where a merch is stored
 *
 * @param merch_db A hash table containing merch
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch
 * @return ioopm_list_t* The list of locations
 */
ioopm_list_t *merch_get_locations(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name);

/**
 * @brief Adds a merch to a shelf
 *
 * @param merch_db The merch database to add the merch to
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param locs_db A hash table mapping each location to the merch that is stored there
 * @param shelf The name of the shelf that will store the merch
 * @param merch The name of the merch to store on the shelf
 */
void merch_shelf_insert(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db,
                        ioopm_hash_table_t *locs_db, char *shelf, char *merch);

/**
 * @brief Increases the stock of a merch
 *
 * @param merch_db The merch database in which the merch is stored
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param merch The name of the merch to replenish
 * @param shelf The name of the shelf to replenish
 * @param increase The number of merch items to add to the shelf
 */
void merch_replenish(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db,
                     char *merch, char *shelf, int increase);

/**
 * @brief Delete a hash table and free its memory
 *
 * @param merch_database the database to be destroyed
 */
void merch_database_destroy(ioopm_hash_table_t *merch_database);

/**
 * @brief Delete a hash table and free its memory
 *
 * @param id_db the database to be destroyed
 */
void id_database_destroy(ioopm_hash_table_t *id_db);

/**
 * @brief Inserting an item into the merch database. The item will have no location and has to be replenished to recive a location.
 *
 * @param merch_database The database to add the merch to
 * @param id_database A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch to add
 * @param description The description of the merch to add
 * @param merch The merch to add to the database
 */
void merch_insert(ioopm_hash_table_t *merch_database, ioopm_hash_table_t *id_database, char *name, char *description, int price);

/**
 * @brief Removes merch from a merch database
 *
 * @param merch_database The merch databse to remove the merch from
 * @param id_database A hash table containing the merch names and heir respective IDs
 * @param name The name of the merch to remove
 */
void merch_remove(ioopm_hash_table_t *merch_database, ioopm_hash_table_t *id_database, char *name);

/**
 * @brief Creates an array of all the names in a database in an alphabetic order
 * The array will need to be freed when no longer needed.
 *
 * @param merch_database The database
 * @return char** The array with all the names in alphabetic order
 */
char **merch_name_sort(ioopm_hash_table_t *merch_database);

/**
 * @brief Lets you change the name, description and price of a merch
 *
 * @param merch_db The database in which the merch is stored
 * @param id_db A hash table containing the merch names and their respective IDs
 * @param old_name The name of the merch to be edited
 * @param new_name The new name of the merch
 * @param new_desc The new description of the merch
 * @param price The new price of the merch
 */
void merch_edit(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *old_name, char *new_name, char *new_desc, int price);

/**
 * @brief Deletes a database of locations
 *
 * @param locs_db The database to delete which maps the name of a shelf to the name of a merch
 */
void locations_database_destroy(ioopm_hash_table_t *locs_db);

/**
 * @brief Fetches the names of the shelfs in alphabetical order on which a specified merch is stored
 * The result array will need to be freed when no longer needed
 *
 * @param merch_db The database in which the merch is stored
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param merch_name The name of the merch to get the stock of
 * @return shelf_t** An array of shelfs in alphabetical order
 */
shelf_t **merch_show_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *merch_name);

/**
 * @brief Fetches the name of a shelf
 *
 * @param shelf The shelf
 * @return char* The name of the shelf or NULL if shelf is NULL
 */
char *shelf_get_name(shelf_t *shelf);

/**
 * @brief Fetches the amount of merch stored on a shelf
 *
 * @param shelf The shelf to get the quantity of
 * @param quantity An int in which the quantity will be stored
 * @return true if shelf is not NULL
 * @return false if the shelf is not NULL
 */
bool shelf_get_quantity(shelf_t *shelf, int *quantity);

/**
 * @brief Adds a merch to a cart and lower the stock on the shelfs
 *
 * @param merch_db The database in which the merch is stored
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param cart_db A hash table containing carts
 * @param name Name of merch to add to cart
 * @param cart_id ID of the cart to add to
 * @param quantity The number of stock to add to a cart
 */
void stock_reservation(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *cart_db, char *name, int cart_id, size_t quantity);

/**
 * @brief Is helper function and should not be used by its own. Lowers the amount of available stock of a merch
 *
 * @param merch_db The database in which the merch is stored
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param name Name of merch to lower the available stock of
 * @param quantity The amount to available stock of
 */
void stock_restoration(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, char *name, size_t quantity);

/**
 * @brief Is helper function and should not be used by its own.
 * Decreases the amount of stock of a merch in merch db and in locations_db
 *
 * @param merch_db The database in which the merch is stored
 * @param id_db A hash table containing the merch names and heir respective IDs
 * @param locs_db The database to delete which maps the name of a shelf to the name of a merch
 * @param name Name of the merch to decrease stock of
 * @param quantity The amount of stock to lower
 */
void stock_decrease(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_db, ioopm_hash_table_t *locs_db, char *name, int quantity);