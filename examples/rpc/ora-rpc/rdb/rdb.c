#include <stdio.h>
#include <ctype.h>
#include <rpc/rpc.h>
#include "rdb.h"

#define PRINTRECORD(pR) {\
  printf("first\tmiddle\tlast\tphone\tlocation\n"); \
  printf("%s\t%s\t%s\t%d\t%s\n", \
    pR->firstName, pR->middleInitial, \
    pR->lastName, pR->phone, pR->location); \
  }

main(argc, argv)
  int             argc;
  char           *argv[];
{
  CLIENT         *cl;    /* a client handle */
  char           *value;
  int             key;

  if ((argc != 4) || (!isdigit(argv[2][0]))) {
    fprintf(stderr, "Usage: %s server key value\n", argv[0]);
    exit(1);
  }
  if (!(cl = clnt_create(argv[1], RDBPROG, RDBVERS, "tcp"))) {
    /*
     * CLIENT handle couldn't be created, server not there.
     */
    clnt_pcreateerror(argv[1]);
    exit(1);
  }
  value = argv[3];
  switch (key = atol(argv[2])) {
  case FIRSTNAME_KEY:
    PRINTRECORD(firstname_key_1(&value, cl));
    break;
  case LASTNAME_KEY:
    PRINTRECORD(lastname_key_1(&value, cl));
    break;
  case PHONE_KEY:{
    int             p;
    if (!(sscanf(argv[3], "%d", &p)) != 1) {
      fprintf(stderr,"\"PHONE_KEY\" requires integer value\n");
      exit(1);
    }
    PRINTRECORD(phone_key_1(&p, cl));
    break;
  }
  case LOCATION_KEY:
    PRINTRECORD(location_key_1(&value, cl));
    break;
  case ADD_RECORD:{
    record  *pR = (record *) malloc(sizeof(record));
    pR->firstName = (char *) malloc(MAX_STR);
    pR->middleInitial = (char *) malloc(MAX_STR);
    pR->lastName = (char *) malloc(MAX_STR);
    pR->location = (char *) malloc(MAX_STR);
    if (sscanf(argv[3], "%s%s%s%d%s", pR->firstName,
        pR->middleInitial, pR->lastName, &(pR->phone),
        pR->location) != 5) {
      fprintf(stderr, "\"ADD_RECORD\" requires a complete quoted record\n");
      exit(1);
    }
    if (!(*add_record_1(pR, cl))) {
      fprintf(stderr, "couldn't add record\n");
      exit(1);
    }
    break;
  }
  default:
    fprintf(stderr, "%s: unknown key\n", argv[0]);
    exit(1);
  }
}
