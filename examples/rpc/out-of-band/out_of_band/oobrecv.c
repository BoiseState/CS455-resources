#include	"myheader.h"

#define BACK_LOG 1024

int		listenfd, connfd;

void	sig_urg(int);

int main(int argc, char **argv)
{
	int		n;
	char	buff[100];
	int serverSocket;
	int port;
    struct hostent *hostPtr = NULL;
    char *machine;
    struct sockaddr_in serverName;
	int status;


	if (argc != 3)
		err_quit("usage: tcprecv01 <host> <port#>");

    machine = argv[1];
    port = atoi(argv[2]);
    serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == -1) {
        err_quit("socket()");
    }

    hostPtr = gethostbyname(machine);
    if (hostPtr == NULL) {
        err_quit("gethostbyname()");
    }

    memset(&serverName, 0, sizeof(serverName));
    memcpy(&serverName.sin_addr, hostPtr->h_addr, hostPtr->h_length);
    serverName.sin_family = AF_INET;
    serverName.sin_port = htons(port);
    status = bind(serverSocket, (struct sockaddr *) &serverName, 
													sizeof(serverName));
    if (status == -1) {
        err_quit("bind()");
    }

    status = listen(serverSocket, BACK_LOG);
    if (status == -1) {
        err_quit("listen()");
	}


	if ((connfd = accept(serverSocket, NULL, NULL)) < 0)
		err_quit("accept");

	signal(SIGURG, sig_urg);
	fcntl(connfd, F_SETOWN, getpid());

	for ( ; ; ) {
		if ( (n = read(connfd, buff, sizeof(buff)-1)) == 0) {
			printf("received EOF\n");
			exit(0);
		}
		buff[n] = 0;	/* null terminate */
		printf("read %d bytes: %s\n", n, buff);
	}
	exit(0);
}

void sig_urg(int signo)
{
	int		n;
	char	buff[100];

	printf("SIGURG received\n");
	if ((n = recv(connfd, buff, sizeof(buff)-1, MSG_OOB)) < 0)
		err_ret("sigurg handler:");
	buff[n] = 0;		/* null terminate */
	printf("read %d OOB byte: %s\n", n, buff);
}
