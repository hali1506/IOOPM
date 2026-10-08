#include <ctype.h>
#include "database.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include "cart.h"

bool confirmation(char *prompt)
{
    char ans = ask_question_continue(prompt);
    if (ans == 'Y')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void input_merch(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht)
{
    char *name = ask_question_string("Name of merch: ");
    char *description = ask_question_string("Description of merch: ");
    int price = ask_question_int("Price of merch: ");

    if (ioopm_hash_table_has_key(id_ht, ptr_elem(name)))
    {
        printf("%s already exists!\n", name);
    }
    else
    {
        merch_insert(merch_db, id_ht, name, description, price);
        printf("Merch created!\n");
    }
    free(description);
    free(name);
}

void list_merch(ioopm_hash_table_t *id_ht)
{
    size_t size = ioopm_hash_table_size(id_ht);
    char **merch_array = merch_name_sort(id_ht);
    for (int printed_items = 0; printed_items < size; printed_items++)
    {
        if (printed_items % 20 == 0 && printed_items > 0)
        {
            char answer = ask_question_continue("Press [N] if you would like to return to the main menu:");
            if (answer == 'N')
            {
                free(merch_array);
                return;
            }
        }

        printf("%i: %s\n", printed_items + 1, merch_array[printed_items]);
    }
    free(merch_array);
    puts("All items listed\n");
}

void remove_merch(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht)
{
    char *merch_to_remove = ask_question_string("Enter the name of the merch you would like to remove: ");

    if (ioopm_hash_table_has_key(id_ht, ptr_elem(merch_to_remove)))
    {
        int stock = 0;
        int available_stock = 0;
        bool get_stock = merch_get_stock(merch_db, id_ht, merch_to_remove, &stock);
        bool get_available_stock = merch_get_available_stock(merch_db, id_ht, merch_to_remove, &available_stock);
        if (get_stock && get_available_stock && stock != available_stock)
        {
            printf("Unable to remove merch, it is currently in a cart!\n");
            return;
        }

        char *question = "Are you sure you would like to remove this merch?[Y] ";
        if (confirmation(question))
        {
            merch_remove(merch_db, id_ht, merch_to_remove);
            printf("%s remmoved!\n", merch_to_remove);
        }
    }
    else
    {
        printf("Merch does not exist!\n");
    }
    free(merch_to_remove);
}

void edit(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht)
{
    char *old_name = ask_question_string("Which merch would you like to edit? ");
    char *new_name = ask_question_string("What is the new name? ");
    char *new_desc = ask_question_string("What is the new description?");
    int new_price = ask_question_int("What is the new price?");

    if (ioopm_hash_table_has_key(id_ht, ptr_elem(old_name)))
    {
        if (ioopm_hash_table_has_key(id_ht, ptr_elem(new_name)))
        {
            printf("Merch already exists!\n");
        }
        else
        {
            char *question = "Are you sure that you would like to edit this merch?[Y] ";

            if (confirmation(question))
            {
                merch_edit(merch_db, id_ht, old_name, new_name, new_desc, new_price);
            }
        }
    }
    else
    {
        printf("Merch does not exist!");
    }
    free(old_name);
    free(new_name);
    free(new_desc);
}

void print_stock(shelf_t **shelfs, size_t size)
{
    for (int i = 0; i < size; i++)
    {
        int quant = 0;
        bool shelf_exist = shelf_get_quantity(shelfs[i], &quant);
        assert(shelf_exist);
        printf("Shelf: %s   Stock: %d\n", shelf_get_name(shelfs[i]), quant);
    }
}

void show_stock(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht)
{
    char *merch_name = ask_question_string("Which merch would you like to show the stock of? ");
    if (ioopm_hash_table_has_key(id_ht, ptr_elem(merch_name)))
    {
        shelf_t **shelfs = merch_show_stock(merch_db, id_ht, merch_name);
        if (!shelfs)
        {
            printf("Merch is not in stock!\n");
        }
        print_stock(shelfs, ioopm_linked_list_size(merch_get_locations(merch_db, id_ht, merch_name)));
        free(shelfs);
    }
    else
    {
        puts("The merch does not exist!\n");
    }
    free(merch_name);
}

void replenish(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht, ioopm_hash_table_t *locs_db)
{
    char *merch = ask_question_string("Which merch would you like to replenish? ");
    char *shelf = ask_question_shelf("Which location would you like to replenish? ");
    int increase = ask_question_int("How many merch items would you like to add to the stock? ");

    if (ioopm_hash_table_has_key(id_ht, ptr_elem(merch)))
    {

        if (!ioopm_hash_table_has_key(locs_db, ptr_elem(shelf)))
        {                                                               
            merch_shelf_insert(merch_db, id_ht, locs_db, shelf, merch); 
            merch_replenish(merch_db, id_ht, merch, shelf, increase);
        }
        else if (strcmp(ioopm_hash_table_lookup(locs_db, ptr_elem(shelf))->ptr, merch) != 0)
        {
            puts("Another merch is stored in this location!\n");
        }
        else if (ioopm_hash_table_has_key(locs_db, ptr_elem(shelf)))
        {
            merch_replenish(merch_db, id_ht, merch, shelf, increase);
            printf("Merch added!\n");
        }
    }

    else
    {
        printf("Merch does not exist!\n");
    }
    free(shelf);
    free(merch);
}

void remove_cart(ioopm_hash_table_t *cart_db, ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht)
{
    int cart_id = ask_question_int("Enter the id of the cart you would like to remove: ");
    if (ioopm_hash_table_has_key(cart_db, int_elem(cart_id)))
    {
        char *question = "Are you sure you would like to remove this cart[Y]? ";
        if (confirmation(question))
        {
            cart_remove(cart_db, merch_db, id_ht, cart_id);
        }
        puts("Cart removed!\n");
    }
    else
    {
        puts("The cart does not exist! \n");
    }
}

void add_to_cart(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht, ioopm_hash_table_t *cart_db)
{
    int cart_id = ask_question_int("Enter the id of the cart you would like to add to: ");
    char *name = ask_question_string("Which merch would you like to add? ");
    int quantity = ask_question_int("How many items would you like to add? ");
    if (ioopm_hash_table_has_key(cart_db, int_elem(cart_id)))
    {
        if (ioopm_hash_table_has_key(id_ht, ptr_elem(name)))
        {
            int available_stock = 0;
            bool get_available_stock = merch_get_available_stock(merch_db, id_ht, name, &available_stock);
            if (!get_available_stock || available_stock < quantity)
            {
                printf("Not enough available stock!\n");
            }
            else
            {
                stock_reservation(merch_db, id_ht, cart_db, name, cart_id, quantity);
                puts("Merch added!\n");
            }
        }
        else
        {
            printf("Merch does not exist!\n");
        }
    }
    else
    {
        puts("The cart does not exist! \n");
    }
    free(name);
}

void remove_from_cart(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht, ioopm_hash_table_t *cart_db)
{
    int cart_id = ask_question_int("Enter the id of the cart you would like to remove from: ");
    char *name = ask_question_string("Which merch would you like to remove? ");
    int quantity = ask_question_int("How many items would you like to remove? ");
    if (ioopm_hash_table_has_key(cart_db, int_elem(cart_id)))
    {

        if (cart_remove_from(merch_db, id_ht, cart_db, name, cart_id, quantity))
        {
            puts("Merch removed!\n");
        }
        else
            printf("Cannot remove more items than the amount in the cart!\n");
    }
    else
    {
        puts("The cart does not exist!\n");
    }
    free(name);
}

void calculate_cost(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht, ioopm_hash_table_t *cart_db)
{
    int cart_id = ask_question_int("Which cart would you like to calculate the cost of? ");
    if (ioopm_hash_table_has_key(cart_db, int_elem(cart_id)))
    {
        int price = cart_calculate(merch_db, id_ht, cart_db, cart_id);
        printf("The total price is %d\n", price);
    }
    else
    {
        printf("The cart does not exist!\n");
    }
}

void checkout(ioopm_hash_table_t *merch_db, ioopm_hash_table_t *id_ht, ioopm_hash_table_t *locs_db, ioopm_hash_table_t *cart_db)
{
    int cart_id = ask_question_int("Which cart would you like to checkout? ");
    if (ioopm_hash_table_has_key(cart_db, int_elem(cart_id)))
    {
        cart_checkout(merch_db, id_ht, locs_db, cart_db, cart_id);
        printf("You have now checked out\n");
    }
    else
        printf("The cart does not exist!\n");
}

void eventloop()
{
    ioopm_hash_table_t *merch_db = merch_database_create();
    ioopm_hash_table_t *id_ht = id_database_create();
    ioopm_hash_table_t *locs_db = location_database_create();
    ioopm_hash_table_t *cart_db = cart_database_create();
    size_t cart_id = 1;

    char answer = ' ';

    while (answer != 'Q')
    {
        answer = ask_question_menu("Select one of the options above: ");
        if (answer == 'A')
        {
            input_merch(merch_db, id_ht);
        }
        else if (answer == 'L')
        {
            list_merch(id_ht);
        }
        else if (answer == 'D')
        {
            remove_merch(merch_db, id_ht);
        }
        else if (answer == 'E')
        {
            edit(merch_db, id_ht);
        }
        else if (answer == 'S')
        {
            show_stock(merch_db, id_ht);
        }
        else if (answer == 'P')
        {
            replenish(merch_db, id_ht, locs_db);
        }
        else if (answer == 'C')
        {
            cart_create(cart_db, cart_id);
            printf("The cart id is: %li\n", cart_id);
            cart_id++;
        }
        else if (answer == 'R')
        {
            remove_cart(cart_db, merch_db, id_ht);
        }
        else if (answer == '+')
        {
            add_to_cart(merch_db, id_ht, cart_db);
        }
        else if (answer == '-')
        {
            remove_from_cart(merch_db, id_ht, cart_db);
        }
        else if (answer == '=')
        {
            calculate_cost(merch_db, id_ht, cart_db);
        }
        else if (answer == 'O')
        {
            checkout(merch_db, id_ht, locs_db, cart_db);
        }
    }

    locations_database_destroy(locs_db);
    id_database_destroy(id_ht);
    merch_database_destroy(merch_db);
    cart_database_destroy(cart_db);
}

int main(int argc, char const *argv[])
{
    eventloop();
    return 0;
}
