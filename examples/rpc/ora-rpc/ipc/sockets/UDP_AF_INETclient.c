#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <signal.h>
#include <netinet/in.h>
#include <netdb.h>

#define UDP_SERVICE 	"my_udp_read_dir_server"
#define HOST 		"rodson"
#define DIRSIZE		8192

void t_nop() 
{
	fprintf(stderr, "waiting for a reply...");
}


main(argc, argv)
int argc; char **argv;
{
	char    dir[DIRSIZE];
	int	sd, retry;
	extern int	errno;                /* error type */
	struct hostent *hp;                /* our port descriptor */
	struct servent *sp;                /* server's port descriptor */
	struct sockaddr_in myaddr;        /* our internet descriptor */
	struct sockaddr_in svaddr;        /* server's internet descriptor */

	/* clear and initialize the socket descriptors */
	memset((char *) & myaddr, 0, sizeof(struct sockaddr_in ));
	memset((char *) & svaddr, 0, sizeof(struct sockaddr_in ));
	svaddr.sin_family = AF_INET;
	hp = gethostbyname(HOST);
	if (hp == NULL) {
		fprintf(stderr, "%s not found in /etc/hosts\n", HOST);
		exit(1);
	}
	svaddr.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
	sp = getservbyname(UDP_SERVICE, "udp");
	if (sp == NULL) {
		fprintf(stderr, "%s not found in /etc/services\n", UDP_SERVICE);
		exit(1);
	}
	svaddr.sin_port = sp->s_port;

	/* create the socket */
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd == -1) {
		perror("socket");
		exit(1);
	}

	/* add some home truths about ourselves */
	myaddr.sin_family = AF_INET;
	myaddr.sin_port = 0;
	myaddr.sin_addr.s_addr = INADDR_ANY;

	/* bind the socket to our port */
	if (bind(sd, &myaddr, sizeof(struct sockaddr_in )) == -1) {
		perror("bind");
		exit(1);
	}

	/* attempt to send a message to the server */
	signal(SIGALRM, t_nop);
	retry = 5;

again:       
	if (sendto(sd, argv[1], strlen(argv[1]), 0, &svaddr, 
		sizeof(struct sockaddr_in )) == -1) {
		perror("sendto");
		exit(1);
	}
	alarm(5);
	if (recv(sd, dir, DIRSIZE, 0) == -1) {
		if (errno == EINTR) {
			if (--retry) {
				goto again;
			} else {
				perror("recv");
				exit(1);
			}
		} else {
			perror("recv");
			exit(1);
		}
	}
	alarm(0);

	/* success - spew-out results and close-up shop */
	printf("%s\n", dir);
	close(sd);
}
