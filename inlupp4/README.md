# Assignment 3 - A Symbolic Calculator
by Hanna Lindmark and Alfred Englund
# Briefing 
The symbolic calculator reads input and creates different expressions depending on the input for example addition, multiplication, etc. All these expressions
are then put into one big tree of expressions which we can then evaluate. If we cannot reduce the whole given expression, it will be reduced as much as possible.

This means `5+5 + x` without setting x will be reduced to `10 + x`. If x was set to a number for example 5 then it would be reduced to `15`.

# Instructions 
To compile and run the program use the following instructions:

- `make all` Compiles the program.

- `make run` Runs the program, you must compile it before running it.

- `clean` Cleans the directory and removes compiled files.

- Tests are run using Visual Studio Code
