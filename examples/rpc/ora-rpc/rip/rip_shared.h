/*
 * rip_shared.c -  remote image processing common header file (different
 * from rip.h generetaed by RPCGEN).
 */
#define MAXSERVERS      8
#define MAXARGS         6
#define MIN(a,b)        (a<b)?a:b
#define MAX(a,b)        (a>b)?a:b
#define TRAVERSE(p,a)   for(p=a; (p->pNext!=NULL); p=p->pNext)
#define FAILED(line_num, args, line) \
  { fprintf(stderr, "%s failed on line %d: \"%s\"\n", args[0], line_num, line); }

typedef char   *string;

/* Procedure definition stubs */
	/* Parsing in commands: */
ReadRIP();			/* Read a rip command line from a file */
Parse();			/* Parse a command line */
RemoveComments();		/* Helps out in above */

	/* Scheduling server jobs: */
SubBFS();			/* Launches and partitions server jobs */
ForkNextServer();		/* Used in SubBFS() to launch children */
FreeServer();			/* Used in SubBFS() to mark children as free */
RequestAll();			/* Send one request to every server */

	/* Client-side rip operations */
AddServer();			/* Try to add a server to the active list */
ReadImage();			/* Read a rasterfile, cc: to servers */
ReadKernel();			/* Reads in a filter kernel, cc: to servers */
Convolve();			/* Tells servers what image and kernel to
				 * convolve */

	/* Server-side rip operations */
SAddServer();			/* Free local memory, reset */
SReadImage();			/* Copy this raster image */
SReadKernel();			/* "       " kernel */
SConvolve();			/* USe the named image and kernel for
				 * convolution */

	/* Client window overhead */
OpenWindow();			/* Starts things off - all action thru the window */
MouseCaught();			/* Mouse notifier routine to fire-off rip operations */
Help();				/* Lists know command usage */
