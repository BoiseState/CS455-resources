#ifndef WIN32

  #include <sys/types.h>
  #include <sys/socket.h>
  #include <sys/time.h>
  #include <unistd.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <sys/utsname.h>
  #include <errno.h>

  #define Init()
  #define DeInit()

  int _GetHostName(char *name, int namelen);

  typedef int SOCKET;
  #define INVALID_SOCKET -1
  #define SOCKET_ERROR   -1
  #define closesocket close


  typedef unsigned char  BYTE;
  typedef unsigned short WORD;
  typedef unsigned int  DWORD;

  typedef int BOOL;
  #define TRUE  1
  #define FALSE 0

#else /* WIN32 */

  #include <windows.h>
  #include <process.h>

  void Init(void);
  void DeInit(void);

  #define _GetHostName gethostname

  #define ECONNREFUSED WSAECONNREFUSED

#endif /* WIN32 */



#define ZERO(s) memset(&s,0,sizeof(s))


#ifdef MINI_FORK
/*
 * MiniFork creates a new process, or a new thread in the current program.
 *          I call it MiniFork, because it does not call exec.
 */

typedef int (FORK_FUNC)(SOCKET s);
BOOL MiniFork(FORK_FUNC foo, SOCKET s);

/*
 * CloseParent and CloseChild are called if a new process is created
 * Between the two, they should close all duplicated handles
 */
void CloseParent(void);
void CloseChild(void);

#endif /* MINI_FORK */


void ErrorExit(const char *pErrorMsg);

