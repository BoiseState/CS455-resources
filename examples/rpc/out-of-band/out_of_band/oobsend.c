#include	"myheader.h"

int tcp_connect(char *remoteHost, int port)
{
	struct hostent *hostPtr = NULL;
    struct sockaddr_in serverName = { 0 };
	int clientSocket;
	int status;

	clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == -1)
		err_quit("socket()");

	hostPtr = gethostbyname(remoteHost);
	if (hostPtr == NULL)
        err_quit("gethostbyname()");


    serverName.sin_family = AF_INET;
    serverName.sin_port = htons(port);
    memcpy(&serverName.sin_addr, hostPtr->h_addr, hostPtr->h_length);

	status = connect(clientSocket, (struct sockaddr*) &serverName, 
														sizeof(serverName));
	if (status == -1)
		err_quit("connect");

	return clientSocket;
}

int main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 3)
		err_quit("usage: tcpsend01 <host> <port#>");

	sockfd = tcp_connect(argv[1], atoi(argv[2]));

	if (write(sockfd, "123", 3) < 0)
		err_quit("oobsend:");
	printf("wrote 3 bytes of normal data\n");
	sleep(1);

	if (send(sockfd, "4", 1, MSG_OOB)< 0)
		err_quit("oobsend:");
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	if (write(sockfd, "56", 2) < 0)
		err_quit("oobsend:");
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	if (send(sockfd, "7", 1, MSG_OOB) < 0)
		err_quit("oobsend:");
	printf("wrote 1 byte of OOB data\n");
	sleep(1);

	if (write(sockfd, "89", 2) < 0)
		err_quit("oobsend:");
	printf("wrote 2 bytes of normal data\n");
	sleep(1);

	exit(0);
}
