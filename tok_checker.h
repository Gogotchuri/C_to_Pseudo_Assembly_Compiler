#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include "bool.h"


/*returns true if char is digit*/
bool is_digit(char c);

/*Returns true if token is a integer literal*/
bool is_number(char * token);

/*Returns true if token is a char literal*/
bool is_char(char * token);

/*Returns true if token is a string literal*/
bool is_string(char * token);

/*Check if current token is literal i.e can be evaluated as value
    returns 0 if token isn't literal
    returns 1 if token is number
    returns 2 if token is char
    returns 3 if token is string */
int is_literal(char * token);

/*Returns true if passed char is allowed in identifier
    @param c identifier char
    @param is_first pass true if c is first char of identifier*/
bool is_id_char(char c, bool is_first);

/*Checks if identifier is valid*/
bool is_identifier(char * token);