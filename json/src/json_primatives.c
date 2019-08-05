#include "json_primatives.h"
#include "debug.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

json_string_p json_string_make()
{
    return malloc(sizeof(json_string_t));
}

json_number_p json_number_make()
{
    return malloc(sizeof(json_number_t));
}

void json_string_destroy(json_string_p pstr)
{
    free(pstr);
}

void json_number_destroy(json_number_p pnum)
{
    free(pnum);
}


json_string_p json_parse_string(char **pstr)
{
    char *str = *pstr;

    ++str; // Skip opening "

    int len = 0;
    while (str[len] != '\"')
    {
        if (str[len] == '\\')
        {
            ++len;
        }

        ++len;
    }

    json_string_p jstr = json_string_make();
    jstr->value = malloc((len + 1) * sizeof(char));
    strncpy(jstr->value, str, len);
    jstr->value[len] = 0;

    str += len + 1;
    *pstr = str;
    return jstr;
}

json_number_p json_parse_number(char **pstr)
{
    char *str = *pstr;

    int len = 0;
    while ((str[len] >= '0' && str[len] <= '9') || str[len] == '.')
    {
        ++len;
    }

    json_number_p num = json_number_make();
    sscanf(str, "%lf", &num->value);

    str += len;
    SKIP_WHITESPACE(str);

    *pstr = str;
    return num;
}

void json_string_print(json_string_t str)
{
    printf("\"%s\"", str.value);
}

void json_number_print(json_number_t num)
{
    printf("%lf", num.value);
}