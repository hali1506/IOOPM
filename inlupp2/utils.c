#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef union
{
    int int_value;
    float float_value;
    char *string_value;
} answer_t;

typedef bool (*check_func)(char *);
typedef answer_t (*convert_func)(char *);

extern char *strdup(const char *);

void clear_input_buffer()
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

int read_string(char *buf, int buf_siz)
{
    int c;
    int counter = -1;
    do
    {
        counter++;
        c = getchar();
        buf[counter] = c;
    } while (c != '\n' && c != EOF && (buf_siz > counter));
    if (buf_siz == counter)
    {
        clear_input_buffer();
    }
    buf[counter] = '\0';
    return counter;
}

bool is_shelf(char *str)
{
    if (isalpha(str[0]) && strlen(str) > 1)
    {
        for (int i = 1; i < strlen(str); i++)
        {
            if (!isdigit(str[i]))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool is_number(char *str)
{
    if (strlen(str) == 0)
    {
        return false;
    }
    if (str[0] == '0')
    {
        return false;
    }

    for (int i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool is_float(char *str)
{
    bool has_decimal = false;
    for (int i = 0; i < strlen(str); i++)
    {

        if (isdigit(str[i]) == 0)
        {
            if ((str[i] == '.') && !has_decimal)
            {
                has_decimal = true;
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}

answer_t make_float(char *str)
{
    return (answer_t){.float_value = atof(str)};
}

bool not_empty(char *str)
{
    return strlen(str) > 0;
}

bool is_menu_item(char *str)
{
    if (strlen(str) == 1)
    {
        char answer = toupper(str[0]);
        return (answer == 'Q' ||
                answer == 'A' ||
                answer == 'L' ||
                answer == 'D' ||
                answer == 'E' ||
                answer == 'S' ||
                answer == 'P' ||
                answer == 'C' ||
                answer == 'R' ||
                answer == '+' ||
                answer == '-' ||
                answer == '=' ||
                answer == 'O');
    }
    return false;
}

bool is_continue(char *str)
{
    return true; // TODO
}

answer_t ask_question(char *question, check_func check, convert_func convert)
{
    int buf_size = 255;
    char buf[buf_size];
    bool confirm;
    printf("%s", question);
    do
    {
        read_string(buf, buf_size);
        confirm = check(buf);
        if (!confirm)
        {
            puts("Fel inmatning. Skriv igen.");
        }
    } while (!confirm);
    return convert(buf);
}

void print_menu(char *filename)
{
    FILE *f = fopen(filename, "r");
    int c = fgetc(f);

    while (c != EOF)
    {
        fputc(c, stdout);
        c = fgetc(f);
    }

    fclose(f);
}

char *ask_question_string(char *question)
{
    return ask_question(question, not_empty, (convert_func)strdup).string_value;
}

int ask_question_int(char *question)
{
    answer_t answer = ask_question(question, is_number, (convert_func)atoi);
    return answer.int_value; // svaret som ett heltal
}

double ask_question_float(char *question)
{
    return ask_question(question, is_float, make_float).float_value;
}

char *ask_question_shelf(char *question) // TODO avallokera?
{
    return ask_question(question, is_shelf, (convert_func)strdup).string_value;
}

char ask_question_menu(char *question)
{
    print_menu("menu.txt");
    char *answer = ask_question(question, is_menu_item, (convert_func)strdup).string_value;
    char letter = toupper(answer[0]);
    free(answer);
    return letter;
}

char ask_question_continue(char *question)
{
    char *str = ask_question(question, is_continue, (convert_func)strdup).string_value;
    char result = str[0];
    result = toupper(result);
    free(str);
    return result;
}

void print(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
}

void println(char *str)
{
    print(str);
    putchar('\n');
}
