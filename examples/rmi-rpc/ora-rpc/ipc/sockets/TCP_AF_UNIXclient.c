#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#define DIRSIZE 8192

main(argc, argv)
int argc; char **argv;
{
	char	dir[DIRSIZE];
	int	sd;
	struct sockaddr_un sin;

	/* set up a unix domain socket */
	strcpy(sin.sun_path, "./socket");
	if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	/* connect to the UNIX socket ./socket */
	if (connect(sd, &sin, sizeof(sin)) == -1) {
		perror("connect");
		exit(1);
	}

	/* send a message to the server */
	if (send(sd, argv[1], strlen(argv[1]), 0) == -1) {
		perror("send");
		exit(1);
	}

	/* wait for a message to come back from the server */
	if (recv(sd, dir, DIRSIZE, 0) == -1) {
		perror("recv");
		exit(1);
	}

	/* spew-out the results */
	printf("%s\n", dir);

	/* close the socket connection */
	close(sd);
}
