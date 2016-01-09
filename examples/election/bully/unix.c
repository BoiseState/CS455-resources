#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compat.h"


/*
 *
 *  Local replace of gethostname() to improve
 *  portability.
 */

int _GetHostName(char *buffer, int length)
{
  struct utsname sysname;
  int status;

  ZERO(sysname);
  status=uname(&sysname);
  if(status!=-1)
    strncpy(buffer,sysname.nodename,length);

  return status;
}/* _GetHostName */




#ifdef MINI_FORK
/*
 * I have to ifdef this stuff out because GCC is to stupid to realize that
 * I don't use this in the client code.
 */

BOOL MiniFork(FORK_FUNC foo, SOCKET s)
{
  int childPid;
  int RetVal;


  childPid=fork();
        
  switch(childPid)
  {
    case -1:  
      /* fork failed */
      return FALSE;

    case 0:
      /* in child */
      CloseChild();
      RetVal=foo(s);
      exit(RetVal);

    default:
      /* in parent */
      CloseParent();
      return TRUE;
  }

}/* MiniFork */

#endif /* MINI_FORK */




void ErrorExit(const char *pErrorMsg)
{
  perror(pErrorMsg);
  exit(1);
}/* ErrorExit */
