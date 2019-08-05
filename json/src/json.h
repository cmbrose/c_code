#ifndef JSON_H
#define JSON_H

#include "types.h"
#include "utils.h"

struct json_object
{
    json_object_type_t type;
    void *value;
};

json_object_p json_parse(char *str);
void json_parse_onto_object(char **pstr, json_object_t *pobj);

void json_object_destroy(json_object_p pobj);
void json_object_destroy_refs(json_object_t obj);

void json_object_print(json_object_t obj);

#endif // JSON_H