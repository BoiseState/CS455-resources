/* @(#)printmsg.c	2.1 88/08/11 4.0 RPCSRC */
/*
 * printmsg.c: print a message on the console
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int printmessage(char *);

int main(int argc, char **argv)
{
	char *message;

	if (argc < 2) {
		fprintf(stderr, "usage: %s <message>\n", argv[0]);
		exit(1);
	}
	message = argv[1];

	if (!printmessage(message)) {
		fprintf(stderr, "%s: sorry, couldn't print your message\n",
			argv[0]);
		exit(1);
	} 
	printf("Message delivered!\n");
	exit(0);
}

/*
 * Print a message to the console.
 * Return a boolean indicating whether the message was actually printed.
 */
int printmessage(char *msg)
{
	FILE *f;

	f = fopen("/dev/tty", "w");
	if (f == NULL) {
		return (0);
	}
	fprintf(f, "%s\n", msg);
	fclose(f);
	return(1);
}
