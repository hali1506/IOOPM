# Sprint 2 - webstore TUI
Authors: Hanna Lindmark and David Melin
The program consists of a simple webstore backend which handles a database of merch items
as well as shopping carts. To interact with the backend a text user interface is used.

# Instructions 
To build and run the program use one of the following rules from the Makefile:

- `test`          -- compiles, builds and runs a testfile

- `memtest`       -- compiles, builds and runt the testfile with valgrinds full leak check

- `memrun`        -- compiles, builds and runs the user interface

- `coverage`      -- compiles, builds and runs the testfile with lcov

- `clean`         -- Removes any built files

# Coverage 
The line coverage of both the database.c and cart.c files is 100%. The branch coverage of
the cart.c file is 88.2% and the branch coverage of the database.c file is 90.6%. This
is mainly due to asserts which are not executed.

# Use of code from Sprint 1
- The program uses libraries with hash tables, linked lists, iterators and utils.
  The hash table, linked list and iterator liberaries where from Hanna Lindmark.
  The utils library was from David Melin

-----------------------------------------------------------
# Backend implementation
The backend consists of two c-files: cart.c and database.c which both build on 
the previously mentioned hash table, linked list and iterator files. We chose to do this
to split the code into smaller parts and make the code easier to navigate.

## Data Structures 
The cart-and merch database implementation build on several different database structures:
- `merch_databse`- A hash table which maps a merch id in the form of an int to the address of a
   merch
- `id_database` - Maps the name of a merch to a unique id
- `locations_database` - Maps the name of a shelf to the name of the merch stored there
- `cart_database` - Maps the id of a cart to the address of a cart

To store information about specific shelfs three new structs are implemented:
- `struct merch` consists of the strings name and description, the ints price, stock and available
  stock and a list of location structs.
- `struct cart` consists of an int id and a list of reservations of merch items
- `struct location` consists of a name in the form of a string and a quantity int
- `struct reservation` represents the number of a specific merch stored in a cart. It consists
  of a string with the name of the merch and an int representing the quantity.

# Frontend implementation
The frontend uses many of the functions from the utils.c file to make sure the
user input has the correct format. To print the users different options the menu.txt file is used.