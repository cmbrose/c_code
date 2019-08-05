#ifndef __GNUC__

#include "win_getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	    optind = 1;
char	*optarg = NULL;

int getopt(int argc, char **argv, char *opts)
{
    if ( optarg ) free(optarg);
    optarg = NULL;

    while (optind < argc && argv[optind][0] != '-')
    {
        ++optind;
    }

    if ( optind >= argc )
    {
        return -1;
    }
    
    int len = strlen(argv[optind]);

    if ( len == 1 ) return '?';

    char c = argv[optind][1];

    char* i;
    for ( i = opts; *i && *i != c; ++i );

    if ( *i == 0 ) return '?';

    if ( *(i + 1) == ':' )
    {
        if ( len > 2 )
        {
            optarg = malloc(len - 1);
            strcpy(optarg, argv[optind] + 2); // Skip '-' and c
        }
        else
        {
            optind++;
            if ( optind >= argc ) return '?';

            optarg = malloc(strlen(argv[optind]) + 1);
            strcpy(optarg, argv[optind]);
        }
    }

    ++optind;

    return c;
}

#endif // __GNUC__