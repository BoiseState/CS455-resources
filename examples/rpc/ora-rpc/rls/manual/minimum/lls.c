/*
 * ls.c: local directory listing main - before RPC
 */
#include <stdio.h>
#include <strings.h>
#include "rls.h"

main (argc, argv)
int argc; char *argv[];
{
char    dir[DIR_SIZE];

        /* call the local procedure */
        strcpy(dir, argv[1]);	/* char dir[DIR_SIZE] is coming and going... */
        read_dir(dir);

        /* spew-out the results and bail out of here! */
        printf("%s\n", dir);

        exit(0);
}
