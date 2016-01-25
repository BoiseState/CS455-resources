#if RPC_SVC		/* added some authentication-testing macros */
%#include "auth.h" /* could pass extern int uid into services */
#endif
#include "../dim.x"
