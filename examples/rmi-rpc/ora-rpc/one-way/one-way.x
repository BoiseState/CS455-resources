/*
 * one-way.x: no-reply remote multiple directory listing protocol
 */
const           MAXNAMELEN = 255;
typedef string  nametype < MAXNAMELEN >;        /* a directory entry */
typedef struct namenode *namelist;      		/* a link in the listing */

/*
 * a node in the directory
 */
struct namenode {
  nametype        name;         /* name of directory entry */
  namelist        pNext;        /* next entry */
};

/*
 * the result of a READDIR operation
 */
union readdir_res switch (int errno) {
  case 0:
  namelist list;                /* no error: return directory listing */
default:
  void;                         /* error occurred: nothing else to return */
};

/*
 * The directory program definition, two procedures, one expects nothing, the
 * other retrieves any results.
 */
program         DIRPROG {
  version         DIRVERS {
    void
    DIRNAME(nametype) = 1;
    readdir_res
    READDIR(nametype) = 2;
  } =             1;
} =             0x20000001;
