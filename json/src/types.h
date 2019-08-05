#ifndef TYPES_H
#define TYPES_H

#define TYPEDEF(name) typedef struct name name##_t, *name##_p

typedef enum json_object_type 
{
    JSON_DICT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER
} json_object_type_t;

TYPEDEF(json_object);
TYPEDEF(json_dict);
TYPEDEF(json_dict_field);
TYPEDEF(json_array);
TYPEDEF(json_string);
TYPEDEF(json_number);

#endif // TYPES_H