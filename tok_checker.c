
#include "tok_checker.h"

bool is_digit(char c)
{
    if ('0' <= c && c <= '9')
        return true;
    else
        return false;
}

bool is_number(char *token)
{
    bool is = true;
    int l = strlen(token);
    for (int i = 0; i < l; i++)
        is = is && is_digit(token[i]);
    return is;
}

bool is_char(char *token)
{
    int l = strlen(token);
    if (l == 3)
        if (token[0] == '\'' && token[l - 1] == '\'')
            return true;

    return false;
}

bool is_string(char *token)
{
    int l = strlen(token);
    if (l >= 2)
        if (token[0] == '\"' && token[l - 1] == '\"')
            return true;
    return false;
}

/*Check if current token is literal i.e can be evaluated as value
    returns 0 if token isn't literal
    returns 1 if token is number
    returns 2 if token is char
    returns 3 if token is string 
*/
int is_literal(char *token)
{
    if (is_number(token))
        return 1;
    if (is_char(token))
        return 2;
    if (is_string(token))
        return 3;
    return 0;
}

/*Returns true if passed char is allowed in identifier
    @param c identifier char
    @param is_first pass true if c is first char of identifier
*/
bool is_id_char(char c, bool is_first)
{
    bool is_correct = (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_');

    if (is_first)
        return is_correct;

    is_correct = is_correct || ('0' <= c && c <= '9');
    return is_correct;
}

/*Checks if identifier is valid*/
bool is_identifier(char *token)
{
    int l = strlen(token);
    int is_correct = true;

    if (l == 0)
        return false;

    is_correct = is_correct && is_id_char(token[0], true);

    if (!is_correct)
        return false;

    if (l == 1)
        return is_correct;

    for (int i = 1; i < l; i++)
        is_correct = is_correct && is_id_char(token[i], false);

    return is_correct;
}
