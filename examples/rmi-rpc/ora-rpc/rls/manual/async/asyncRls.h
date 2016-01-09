#define DIR_SIZE 8192
#define DIRPROG ((u_long) 0x20000002)	/* server program (suite) number */
#define DIRVERS ((u_long) 1)	/* program version number */
#define READDIR ((u_long) 1)	/* procedure number for look-up */


#define DIRDPROG ((u_long) 0x20000003)	/* local daemon to collect returns */
#define DIRDVERS ((u_long) 1)
#define LOCALD   ((u_long) 1)

/*#define CHLDDIRPROC ((u_long) 0x20000004)	/* Child daemon */
