#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define DIRSIZE 8192

main()
{
        char     dir[DIRSIZE];  /* used for incomming dir name, and
					outgoing data */
        int      sd, sd_current; /* socket descriptors */
        struct   sockaddr_un sin; /* socket address stuff */

        /* get a UNIX domain AF_UNIX TCP socket */
        strcpy(sin.sun_path, "./socket");
        if ((sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                exit(1);
        }
        /* bind the socket to the socket file name ./socket */
        if (bind(sd, &sin, sizeof(sin)) == -1) {
                perror("bind");
                exit(1);
        }
        /* make available the fact that we are listening */
        if (listen(sd, 5) == -1) {
                perror("listen");
                exit(1);
        }
        /* wait for a client to talk to us */
        if ((sd_current = accept(sd, 0, 0)) == -1) {
                perror("accept");
                exit(1);
        }
        /* get a message from the client */
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
        /* close up both sockets */
        close(sd_current); close(sd);
        /* don't forget to clean-up */
        unlink("./socket");
}
