#ifndef JSON_ARRAY_H
#define JSON_ARRAY_H

#include "types.h"
#include "json.h"

struct json_array
{
    int length;
    json_object_t *values;
};

json_array_p json_parse_array(char **pstr);

void json_array_print(json_array_t arr);

json_array_p json_array_make();
void json_array_destroy(json_array_p parr);

#endif // JSON_ARRAY_H