/* note - RPC compliant procedure calls take one input and
   return one output. Everything is passed by pointer.  Return
   values should point to static data, as it might have to 
   survive some while. */
#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>     /* use <xpg2include/sys/dirent.h> (SunOS4.1) or
        <sys/dirent.h> for X/Open Portability Guide, issue 2 conformance */
#include "rls.h"

read_dir(dir)
char    *dir;   /* char dir[DIR_SIZE] */
{
        DIR * dirp;
        struct direct *d;
		  printf("beginning ");

        /* open directory */
        dirp = opendir(dir);
        if (dirp == NULL)
                return(NULL);

        /* stuff filenames into dir buffer */
        dir[0] = NULL;
        while (d = readdir(dirp))
                sprintf(dir, "%s%s\n", dir, d->d_name);

        /* return the result */
		  printf("returning ");
        closedir(dirp);
        return((int)dir);  /* this is the only new line from Example 4-3 */
}
