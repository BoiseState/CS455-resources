#include <stdio.h>
#include "compat.h"


void Init(void)
{
  WSADATA WsaData;
  int err;

  err=WSAStartup(0x0101,&WsaData);
  if(err)
    ErrorExit("WSAStartup()");
  
}//Init


void DeInit(void)
{
  WSACleanup();
}//DeInit




#ifdef MINI_FORK
/*
 * I have to ifdef this stuff out because GCC is to stupid to realize that
 * I don't use this in the client code.
 */

BOOL MiniFork(FORK_FUNC foo, SOCKET s)
{
  HANDLE hThread;
  DWORD ThreadID;


  hThread=CreateThread(NULL,0,(void *)foo,(void *)s,0,&ThreadID);
  if(!hThread)
    return FALSE;

  CloseHandle(hThread);
  return TRUE;
}//MiniFork

#endif /* MINI_FORK */



void ErrorExit(const char *pErrorMsg)
{
  char Output[1024];
  DWORD LastError;


  if(errno)
  {
    //Show a RTL Error
    perror(pErrorMsg);
  }
  else
  {
    //Show a Windows Error

    LastError=GetLastError();
    if(!FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,NULL,LastError,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                      Output,sizeof(Output),NULL))
    {
      //Unfortunately, FormatMessage is stupid and doesn't handle socket error codes!
      wsprintf(Output,"%d",LastError);
    }

    fprintf(stderr,"%s:  %s\n\n",pErrorMsg,Output);
  }
  
  exit(1);
}//ErrorExit
