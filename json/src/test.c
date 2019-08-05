#include "json.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    printf("starting\n");
    json_object_p obj = json_parse("{ \"number\" : 1, \"string\" : \"a string\", \"array\" : [ 1, 2, 3 ], \"dict\" : { \"inner_num\" : 1 } }");
    printf("parsed obj\n");
    json_object_print(*obj);
    printf("\n");

    return 0;
}