/*
 * rdb.x: remote database access protocol
 */
/* preprocessor directives */
%#define DATABASE "personnel.dat"	/* '%' passes it through */

/* constant definitions */
const MAX_STR = 256;

/* structure definitions, no enumerations needed */
struct record {
  string firstName<MAX_STR>;	/* <> defines the maximum */
  string middleInitial<MAX_STR>;		 /* possible length */
  string lastName<MAX_STR>;
  int phone;
  string location<MAX_STR>;
};

/* program definition, no union or typdef definitions needed */
program RDBPROG { /* could manage multiple servers */
	version RDBVERS {
		record FIRSTNAME_KEY(string) = 1;
		record LASTNAME_KEY(string) = 2;
		record PHONE_KEY(int) = 3;
		record LOCATION_KEY(string) = 4;
		int ADD_RECORD(record) = 5;
	} = 1;
} = 0x20000001;  /* program number ranges established by ONC */
