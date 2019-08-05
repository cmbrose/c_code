#ifndef JSON_PRIMATIVES_H
#define JSON_PRIMATIVES_H

#include "types.h"

struct json_string
{
    char *value;
};

struct json_number
{
    double value;
};

json_string_p json_parse_string(char **pstr);
json_number_p json_parse_number(char **pstr);

void json_string_print(json_string_t str);
void json_number_print(json_number_t num);

json_string_p json_string_make();
json_number_p json_number_make();

void json_string_destroy(json_string_p pstr);
void json_number_destroy(json_number_p pnum);

#endif // JSON_PRIMATIVES_H