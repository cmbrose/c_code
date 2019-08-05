#include "json.h"
#include "json_dict.h"
#include "json_array.h"
#include "json_primatives.h"
#include "utils.h"

#ifdef DEBUG
    #include "debug.h"
#endif

#include <stdio.h>
#include <stdlib.h>

json_object_p json_parse(char *str)
{   
    json_object_p obj = malloc(sizeof(json_object_t));
    json_parse_onto_object(&str, obj);
    return obj;
}

void json_object_destroy(json_object_p pobj)
{
    json_object_destroy_refs(*pobj);
    free(pobj);
}

void json_object_destroy_refs(json_object_t obj)
{
    switch (obj.type)
    {
        case JSON_DICT:
            json_dict_destroy(obj.value);
            break;

        case JSON_ARRAY:
            json_array_destroy(obj.value);
            break;

        case JSON_STRING:
            json_string_destroy(obj.value);
            break;
        
        case JSON_NUMBER:
            json_number_destroy(obj.value);
            break;
    }
}

void json_parse_onto_object(char **pstr, json_object_t *pobj)
{
    char *str = *pstr;

    SKIP_WHITESPACE(str);

    switch (*str)
    {
        case '{':     
            DEBUG_INFO("Parsing dict\n");
            pobj->type = JSON_DICT;
            pobj->value = json_parse_dict(&str);
            break;
        
        case '[':
            DEBUG_INFO("Parsing array\n");
            pobj->type = JSON_ARRAY;
            pobj->value = json_parse_array(&str);
            break;

        case '\"':
            DEBUG_INFO("Parsing string\n");
            pobj->type = JSON_STRING;
            pobj->value = json_parse_string(&str);
            break;

        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
            DEBUG_INFO("Parsing number\n");
            pobj->type = JSON_NUMBER;
            pobj->value = json_parse_number(&str);
            break;

        default:
            break;
    }

    *pstr = str;
}

void json_object_print(json_object_t obj)
{
    void *val = obj.value;
    switch (obj.type)
    {
        case JSON_DICT:
            json_dict_print(*((json_dict_p) val));
            break;

        case JSON_ARRAY:
            json_array_print(*((json_array_p) val));
            break;

        case JSON_STRING:
            json_string_print(*((json_string_p) val));
            break;

        case JSON_NUMBER:
            json_number_print(*((json_number_p) val));
            break;
    }
}