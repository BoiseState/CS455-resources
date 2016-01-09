#include <stdio.h>
#include <netdb.h>

main(argc, argv)
int argc;
char **argv;
  {
  struct servent *out;
  int rem;
  char c;

  out=getservbyname("exec","tcp");
  rem = rexec(&argv[1], out->s_port, NULL, NULL, argv[2], 0);
  while (read(rem, &c, 1)) putchar(c);
  }
