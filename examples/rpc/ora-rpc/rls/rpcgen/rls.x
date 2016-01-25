/*
 * rls.x: remote directory listing protocol
 */
const MAXNAMELEN = 255;
typedef string nametype<MAXNAMELEN>;	/* a directory entry */
typedef struct namenode *namelist;	/* a link in the listing */

/*
 * a node in the directory
 */
struct namenode {
	nametype name;		/* name of directory entry */
	namelist pNext;		/* next entry */
};

/*
 * the result of a READDIR operation
 */
union readdir_res switch (int errno) {
case 0:
	namelist list;		/* no error: return directory listing */
default:
	void;			/* error occurred: nothing else to return */
};

/*
 * the directory program definition
 */
program DIRPROG {
	version DIRVERS {
		readdir_res
		READDIR(nametype) = 1;
	} = 1;
} = 0x20000001;
