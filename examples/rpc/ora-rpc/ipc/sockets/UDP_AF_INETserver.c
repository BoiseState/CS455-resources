#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#define UDP_SERVICE "my_udp_read_dir_server" /* note - must add this as su to
 /etc/services */ 
#define DIRSIZE         8192

main()
{
	char     dir[DIRSIZE];  /* used for incomming dir name, and
						outgoing data */
	int	s, cc, addrlen;
	struct sockaddr_in myaddr;        /* my internet location */
	struct sockaddr_in claddr;        /* the client's internet location */
	struct servent *sp;                /* the port map for my service */

	/* clear and initialize the socket descriptors */
	bzero((char *) &myaddr, sizeof(struct sockaddr_in));
	bzero((char *) &claddr, sizeof(struct sockaddr_in));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = INADDR_ANY;
	sp = getservbyname(UDP_SERVICE, "udp");
	if (sp == NULL) {
		printf("Can not find %s in /etc/services\n", UDP_SERVICE);
		exit(1);
	}
	myaddr.sin_port = sp->s_port;

	/* grab an internet socket descriptor */
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);

	} 
 
	/* bind the socket to our port description as specified above */
	if (bind(s, &myaddr, sizeof(struct sockaddr_in )) == -1) {
		perror("bind");
		exit(1);
	}

	/* wait for a message to arrive */
	errno = 0;  /* no errors at this point */
	addrlen = sizeof(struct sockaddr_in );

	/* get the message which is being sent */
	cc = recvfrom(s, dir, sizeof(dir), 0, &claddr, &addrlen);
	if (cc == -1) {
		perror("recv");
		exit(1);
	}

	/* get the directory contents */
	read_dir(dir);

	/* acknowledge the message, reply w/ the file names */
	if (sendto(s, dir, strlen(dir), 0, &claddr, addrlen) == -1) {
		perror("sendto");
		exit(1);
	}

	/* connectionless - no accept() extra socket descriptor to close */
	close(s);
}


