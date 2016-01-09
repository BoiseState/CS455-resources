#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "compat.h"


#define TRACE(a) puts(a)
#define TRACE1(a)



#define DEFAULT_PORT    5500
#define MAX_GROUP_SIZE  256



//Note:  Since we read a timer with one second granularity, use of these
//timeouts are plus or minus a second.  Therefore it is unwise to use a
//timeout of 1

//Time between sending "Are You Alive"
#define AYA_INTERVAL      10

//Time in which to expect a response to AYA
#define AYA_TIMEOUT        2

//Time in which to expect to receive an ACK_ELECT
#define ELECT_TIMEOUT      2

//Time in which to expect to receive the results of an Election
#define IM_COORD_TIMEOUT  10



typedef struct GROUP_ADDR_TAG
{
  int pid;
  DWORD IP;
}GROUP_ADDR;



#define AYA        1
#define IamA       2
#define ELECT      3
#define ACK_ELECT  4
#define JOIN       5
#define WELCOME    6
#define LEAVE      7
#define IM_COORD   8

typedef struct MESSAGE_TAG
{
  BYTE OpCode;
  BYTE Pad[3];

  GROUP_ADDR From;
  GROUP_ADDR To;

  //Generic parameter
  DWORD lParam;
}MESSAGE;



BOOL Recv(MESSAGE *pMsg);
void Send(MESSAGE *pMsg);

BOOL IsEqual(GROUP_ADDR *pAddr1, GROUP_ADDR *pAddr2);
BOOL IsBigger(GROUP_ADDR *pAddr1, GROUP_ADDR *pAddr2);

void AddProcess(GROUP_ADDR *pAddr);
void RemoveProcess(GROUP_ADDR *pAddr);

void StartElection(void);
void CancelElection(MESSAGE *pMsg);
void CheckElectionTimer(void);
void CheckAYATimer(void);
void CheckAYAInterval(void);
void SetCoord(GROUP_ADDR *pAddr);



const GROUP_ADDR BcastAddr={0,INADDR_BROADCAST};


/* Who am I */
short GroupPort;
SOCKET s;
GROUP_ADDR Me;

/* Who is the boss */
GROUP_ADDR Coord;
BOOL bCoordAlive=FALSE;
time_t AYATimer=0;
time_t AYAInterval=0;

/* Are we currently having an election? */
BOOL bElect=FALSE;
DWORD ElectionID=0;
time_t ElectionTimer;

/* The group */
GROUP_ADDR GroupList[MAX_GROUP_SIZE];
int GroupSize=0;



int main(int argc, char *argv[])
{
  char hostname[80];
  struct hostent *hostPtr;
  struct sockaddr_in serverName;

  fd_set ReadSet;
  struct timeval TimeOut;

  MESSAGE msg;

  int on;
  int status;


  Init();

  GroupPort=DEFAULT_PORT;

  switch(argc)
  {
    case 2:
      GroupPort=atoi(argv[1]);
      //FALLTHROUGH

    case 1:
      break;

    default:
      fprintf(stderr, "Usage: %s [ServerPort]\n", argv[0]);
      exit(1);
  }/* switch(argc) */


  s=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP);
  if(s==INVALID_SOCKET)
    ErrorExit("socket()");

  on=1;
  status=setsockopt(s,SOL_SOCKET,SO_REUSEADDR,
                    (char *)&on,sizeof(on));
  if(status==SOCKET_ERROR)
    ErrorExit("setsockopt(...,SO_REUSEADDR,...)");

  status=setsockopt(s,SOL_SOCKET,SO_BROADCAST,
                    (char *)&on,sizeof(on));
  if(status==SOCKET_ERROR)
    ErrorExit("setsockopt(...,SO_BROADCAST,...)");


  /* 
   * Find out who I am.
   */
  status=_GetHostName(hostname,sizeof(hostname));
  if(status==SOCKET_ERROR)
    ErrorExit("_GetHostName()");

  hostPtr=gethostbyname(hostname);
  if(hostPtr==NULL)
    ErrorExit("gethostbyname()");

  ZERO(serverName);
  serverName.sin_family=AF_INET;
  serverName.sin_port=htons(GroupPort);
  serverName.sin_addr.s_addr=INADDR_ANY;

  /* A "feature" of Linux is that if I bind to a specific address
   * it assumes that I don't want to get broadcasts.
   *
   * memcpy(&serverName.sin_addr,hostPtr->h_addr,hostPtr->h_length);
   */

  status=bind(s,(struct sockaddr *)&serverName,sizeof(serverName));
  if(status==SOCKET_ERROR)
    ErrorExit("bind()");


  /* 
   * Init Globals
   */
  Me.pid=getpid();
  Me.IP=*((DWORD *)hostPtr->h_addr);

  printf("Hello.  My Unique ID is %X %X.\n\n",Me.pid,Me.IP);


  /* 
   * Join the group
   */
  TRACE("Broadcasting JOIN");
  msg.OpCode=JOIN;
  msg.To=BcastAddr;
  Send(&msg);


  /* 
   * Start an election (Once we know who is in the group.)
   */
  TRACE("Set Election Timer");
  AYATimer=time(NULL)+AYA_TIMEOUT;

  while(TRUE)
  {
    TimeOut.tv_sec=1;
    TimeOut.tv_usec=0;

    FD_ZERO(&ReadSet);
    FD_SET(s,&ReadSet);

    status=select(s+1,&ReadSet,NULL,NULL,&TimeOut);
    if(status)
    {
      if(Recv(&msg))
      {
        switch(msg.OpCode)
        {
          case AYA:
            TRACE("Received AYA");

            msg.OpCode=IamA;
            msg.To=msg.From;
            Send(&msg);
            break;

          case IamA:
            TRACE("Received IamA");

            if(IsEqual(&msg.From,&Coord))
              bCoordAlive=TRUE;
            else
              TRACE("IamA--Wrong Coordinator");
            break;

          case ELECT:
            TRACE("Received ELECT");

            msg.OpCode=ACK_ELECT;
            msg.To=msg.From;
            Send(&msg);

            StartElection();
            break;

          case ACK_ELECT:
            TRACE("Received ACK_ELECT");

            CancelElection(&msg);
            break;

          case JOIN:
            TRACE("Received JOIN");

            //Broadcast can include myself
            if(IsEqual(&msg.From,&Me))
            {
              TRACE("JOIN--From myself!");
              break;
            }

            AddProcess(&msg.From);

            msg.OpCode=WELCOME;
            msg.To=msg.From;
            Send(&msg);

            //It looks like we need to restart our election
            if(bElect && IsBigger(&msg.From,&Me))
            {
              bElect=FALSE;
              StartElection();
            }
            break;

          case WELCOME:
            TRACE("Received WELCOME");

            AddProcess(&msg.From);
            break;

          case IM_COORD:
            TRACE("Received IM_COORD");

            SetCoord(&msg.From);
            break;

          case LEAVE:
            TRACE("Received LEAVE");

            RemoveProcess(&msg.From);
            if(IsEqual(&msg.From,&Coord))
              StartElection();
        }//switch(OpCode
      }//if(Recv
    }//if(status

    CheckElectionTimer();
    CheckAYATimer();
    CheckAYAInterval();
  }//while(1

  return 0;
}//main



BOOL Recv(MESSAGE *pMsg)
{
  int Len;

  Len=recv(s,(char *)pMsg,sizeof(MESSAGE),0);
  if(Len<0)
  {
    if(errno==ECONNREFUSED)
    {
      TRACE1("I got Connection Refused!");
      return FALSE;
    }

    ErrorExit("recv()");
  }

  if(Len!=sizeof(MESSAGE))
  {
    fprintf(stderr,"Got packet with wrong length:  %d.\n",Len);
    return FALSE;
  }

  if( memcmp(&pMsg->To,&BcastAddr,sizeof(GROUP_ADDR)) &&
      memcmp(&pMsg->To,&Me,sizeof(GROUP_ADDR))
    )
  {
    TRACE1("Packet is not to me!");
    return FALSE;
  }

  TRACE1("I got a Packet!");
  return TRUE;
}//Recv



void Send(MESSAGE *pMsg)
{
  struct sockaddr_in OutAddress;
  int Len;


  ZERO(OutAddress);
  OutAddress.sin_family=AF_INET;
  OutAddress.sin_port=htons(GroupPort);
  OutAddress.sin_addr.s_addr=pMsg->To.IP;

  pMsg->From=Me;

  while(TRUE)
  {
    Len=sendto(s,(char *)pMsg,sizeof(MESSAGE),0,(struct sockaddr *)&OutAddress,sizeof(OutAddress));
    if(Len==sizeof(MESSAGE))
      return;

    if(errno!=ECONNREFUSED)
      ErrorExit("sendto()");

    TRACE1("I got Connection Refused!");
  }
}//Send



BOOL IsEqual(GROUP_ADDR *pAddr1, GROUP_ADDR *pAddr2)
{
  return 0==memcmp(pAddr1,pAddr2,sizeof(GROUP_ADDR));
}//IsEqual


BOOL IsBigger(GROUP_ADDR *pAddr1, GROUP_ADDR *pAddr2)
{
  return 0>memcmp(pAddr1,pAddr2,sizeof(GROUP_ADDR));
}//IsBigger



int FindProcess(GROUP_ADDR *pAddr)
{
  int i;

  for(i=0;i<GroupSize;i++)
    if(IsEqual(&GroupList[i],pAddr))
      return i;
  return -1;
}//FindProcess


void AddProcess(GROUP_ADDR *pAddr)
{
  int i;

  i=FindProcess(pAddr);
  if(i<0)
    GroupList[GroupSize++]=*pAddr;
}//AddProcess


void RemoveProcess(GROUP_ADDR *pAddr)
{
  int i;

  i=FindProcess(pAddr);
  if(i>=0)
    GroupList[i]=GroupList[--GroupSize];
}//RemoveProcess



void StartElection(void)
{
  MESSAGE msg;
  int i;
  BOOL bAnySent=FALSE;


  if(bElect)
    return;

  TRACE("Starting Election");
  bElect=TRUE;
  bCoordAlive=FALSE;

  msg.OpCode=ELECT;
  msg.lParam=++ElectionID;

  for(i=0;i<GroupSize;i++)
  {
    if(IsBigger(&GroupList[i],&Me))
    {
      msg.To=GroupList[i];
      Send(&msg);
      bAnySent=TRUE;
    }
  }

  ElectionTimer=time(NULL);
  if(bAnySent)
    ElectionTimer+=ELECT_TIMEOUT;
  else
    CheckElectionTimer();
}//StartElection


void CancelElection(MESSAGE *pMsg)
{
  TRACE("Cancel Election");

  if(pMsg->lParam==ElectionID)
  {
    bElect=FALSE;

    //If I don't here from the Coordinator before too long,
    //I'm going to have to start another election!
    AYATimer=time(NULL)+IM_COORD_TIMEOUT;
  }
  else
    TRACE("Stale Election");
}//CancelElection


void CheckElectionTimer(void)
{
  MESSAGE msg;
  int i;


  if(!bElect)
    return;

  if(time(NULL)<ElectionTimer)
    return;

  /*TRACE("I won the Election");*/
  printf("I won the election %d:%d \n",Me.pid,Me.IP);

  SetCoord(&Me);
  bElect=FALSE;

  //Remove all members above me
  while(1)
  {
    for(i=0;i<GroupSize;i++)
    {
      if(IsBigger(&GroupList[i],&Me))
      {
        RemoveProcess(&GroupList[i]);
        continue;
      }
    }
    break;
  }

  //Tell everyone that I'm the boss
  msg.OpCode=IM_COORD;
  for(i=0;i<GroupSize;i++)
  {
    msg.To=GroupList[i];
    Send(&msg);
  }
}//CheckElectionTimer


void CheckAYATimer(void)
{
  if(bCoordAlive)
    return;

  if(time(NULL)<AYATimer)
    return;

  StartElection();
}//CheckAYATimer


void CheckAYAInterval(void)
{
  MESSAGE msg;


  if(!bCoordAlive)
    return;

  if(time(NULL)<AYAInterval)
    return;

  if( (!IsEqual(&Me,&Coord)) && (!bElect) )
  {
    TRACE("Sending AYA");

    msg.OpCode=AYA;
    msg.To=Coord;
    Send(&msg);
    bCoordAlive=FALSE;
  }

  AYATimer=AYAInterval=time(NULL);
  AYATimer+=AYA_TIMEOUT;
  AYAInterval+=AYA_INTERVAL;
}//CheckAYAInterval


void SetCoord(GROUP_ADDR *pAddr)
{
  Coord=*pAddr;

  //Wait for AYA_INTERVAL before sending AYA
  bCoordAlive=TRUE;
  AYAInterval=time(NULL)+AYA_INTERVAL;


  //Let's play king of the hill!
  if(IsBigger(&Me,&Coord))
    StartElection();
}//SetCoord


