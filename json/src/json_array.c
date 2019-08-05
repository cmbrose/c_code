#include "json_array.h"
#include "debug.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

json_array_p json_array_make()
{
    json_array_p arr = malloc(sizeof(json_array_t));
    arr->length = 0;
    arr->values = malloc(0);

    return arr;
}

void json_array_destroy(json_array_p parray)
{
    int i;
    for (i = 0; i < parray->length; ++i)
    {
        json_object_destroy_refs(parray->values[i]);
    }

    free(parray);
}

json_array_p json_parse_array(char **pstr)
{
    char *str = *pstr;
    
    json_array_p arr = json_array_make();

    ++str; // Skip opening [
    SKIP_WHITESPACE(str);

    if (str[0] == ']')
    {
        return arr;
    }

    while (1)
    {
        arr->values = realloc(arr->values, (arr->length + 1) * sizeof(json_object_t));
        json_parse_onto_object(&str, &arr->values[arr->length]);
        arr->length += 1;

        SKIP_WHITESPACE(str);

        if (str[0] == ']')
        {
            break;
        }

        ++str; // Skip ,
        SKIP_WHITESPACE(str);
    }

    ++str; // Skip closing ]
    *pstr = str;
    return arr;
}

void json_array_print(json_array_t arr)
{
    printf("[ ");

    int i;
    for (i = 0; i < arr.length; ++i)
    {
        if (i > 0)
        {
            printf(", ");
        }
        
        json_object_print(arr.values[i]);                
    }

    printf(" ]");
}