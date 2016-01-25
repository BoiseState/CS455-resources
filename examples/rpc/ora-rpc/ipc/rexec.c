#include <stdio.h>
#include <netdb.h>

main(argc, argv)
int argc;
char **argv;
  {
  struct servent *out;
  int sock;
  char buf[1024];

  out=getservbyname("exec","tcp");
  sock = rexec(&argv[1], out->s_port, NULL, NULL, argv[2], 0);

  while (read(sock, buf, 1024) > 0) printf("%s\n", buf);
  }
