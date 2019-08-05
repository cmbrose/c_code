#ifdef __GNUC__
#include <getopt.h>
#endif
#ifndef __GNUC__

#ifndef GETOPT_H
#define GETOPT_H

    extern int opterr;
    extern int optind;
    extern int optopt;
    extern char *optarg;
    extern int getopt(int argc, char **argv, char *opts);

#endif // GETOPT_H
#endif // __GNUC__ 