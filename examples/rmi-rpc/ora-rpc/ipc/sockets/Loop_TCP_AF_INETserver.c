#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORT 		0x1234
#define DIRSIZE 	8192

main()
{
        char     dir[DIRSIZE];  /* used for incomming dir name, and
					outgoing data */
	int 	 sd, sd_current, cc, fromlen, tolen;
	int 	 addrlen;
	struct   sockaddr_in sin;
	struct   sockaddr_in pin;

	/* get an internet domain socket */
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	/* complete the socket structure */
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	/* bind the socket to the port number */
	if (bind(sd, &sin, sizeof(sin)) == -1) {
		perror("bind");
		exit(1);
	}

	/* show that we are willing to listen */
	if (listen(sd, 5) == -1) {
		perror("listen");
		exit(1);
	}
again:
	/* wait for a client to talk to us */
	if ((sd_current = accept(sd, &pin, &addrlen)) == -1) {
		perror("accept");
		exit(1);
	}

	/* get a message from the client - note must now clear dir */
	bzero(dir, sizeof(dir));
	if (recv(sd_current, dir, sizeof(dir), 0) == -1) {
		perror("recv");
		exit(1);
	}

        /* get the directory contents */
        read_dir(dir);

	/* acknowledge the message, reply w/ the file names */
	if (send(sd_current, dir, strlen(dir), 0) == -1) {
		perror("send");
		exit(1);
	}

	/* close up the one socket and continue */
        close(sd_current);
	goto again;
}


