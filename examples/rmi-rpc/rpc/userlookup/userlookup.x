/* Protocol definition file for an RPC based user lookup application */

/* There are 2 remote procedures -
	byuid - returns a user name given the uid number
	byname - returns a uid given a user name

   Both routines now include a structure that includes an error code.

*/


/* define error codes - notice the '%' that tells rpcgen to simply
  include this statement in all generated source code files */

%#define NOTFOUND 0
%#define FOUND 1

/* Need to define a string type here with max length
	(remember about needing a wrapper for the
	xdr string filter?)
*/

typedef string username<10>;

/* need structures for return values so we can include an error code */
struct uname_retval {
    int found;		/* Will be set to FOUND or NOTFOUND */
    username name;      /* If found - will hold the real name */
};

struct uid_retval {
    int found;		/* Will be set to FOUND or NOTFOUND */
    int uid;            /* If found - will hold the real uid */
};


program ULKUP_PROG {
   version ULKUP_VERSION {
     uid_retval byname(username) = 1;
     uname_retval bynum(int) = 2;
   } = 1;
} = 555555556;





