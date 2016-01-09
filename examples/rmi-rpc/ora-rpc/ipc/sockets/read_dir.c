#include <stdio.h>
#include <sys/types.h>
#include <sys/dir.h>

#define DIR_SIZE 8192

read_dir(dir)
char    *dir;   /* anticipate char dir[DIR_SIZE] */
{
        DIR * dirp;
        struct direct *d;

        /* open directory */
        dirp = opendir(dir);
        if (dirp == NULL)
                return(NULL);

        /* stuff filenames into dir buffer */
        dir[0] = NULL;
        while (d = readdir(dirp))
                sprintf(dir, "%s%s\n", dir, d->d_name);

        /* return the result */
        closedir(dirp);
}
