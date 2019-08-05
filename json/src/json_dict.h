#ifndef JSON_DICT_H
#define JSON_DICT_H

#include "types.h"
#include "json.h"

struct json_dict
{
    int length;
    json_dict_field_t *fields;
};

struct json_dict_field
{
    char *name;    
    json_object_t value;
};

json_dict_p json_parse_dict(char **pstr);

void json_dict_print(json_dict_t dict);

json_dict_p json_dict_make();
void json_dict_destroy(json_dict_p pdict);

#endif // JSON_DICT_H