#include "json_dict.h"
#include "debug.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

json_dict_p json_dict_make()
{
    json_dict_p dict = malloc(sizeof(json_dict_t));
    dict->length = 0;
    dict->fields = malloc(0);

    return dict;
}

void json_dict_destroy(json_dict_p pdict)
{
    int i;
    for (i = 0; i < pdict->length; ++i)
    {
        json_object_destroy_refs(pdict->fields[i].value);
    }

    free(pdict);
}

json_dict_p json_parse_dict(char **pstr)
{
    char *str = *pstr;

    json_dict_p dict = json_dict_make();

    ++str; // Skip opening {
    SKIP_WHITESPACE(str);

    if (str[0] == '}')
    {
        return dict;
    }

    while (1)
    {     
        ++str; // Skip opening "

        int i = 0;
        while (str[i] != '\"')
        {
            ++i;
        }

        char *name = malloc(i * sizeof(char));
        strncpy(name, str, i);
        name[i] = 0;

        str = str + i + 1; // Skip closing "
        SKIP_WHITESPACE(str);
        ++str; // Skip :
        SKIP_WHITESPACE(str);

        dict->fields = realloc(dict->fields, (dict->length + 1) * sizeof(json_dict_field_t));
        dict->fields[dict->length].name = name;
        json_parse_onto_object(&str, &dict->fields[dict->length].value);
        dict->length += 1;

        SKIP_WHITESPACE(str);

        if (str[0] == '}')
        {
            break;
        }

        ++str; // Skip ,
        SKIP_WHITESPACE(str);
    }

    ++str; // Skip closing }
    *pstr = str;
    return dict;
}

void json_dict_print(json_dict_t dict)
{
    printf("{ ");

    int i;
    for (i = 0; i < dict.length; ++i)
    {
        if (i > 0)
        {
            printf(", ");
        }

        json_dict_field_t field = dict.fields[i];
        printf("\"%s\": ", field.name);
        json_object_print(field.value);                
    }

    printf(" }");
}