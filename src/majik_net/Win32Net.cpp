// Win32Net.cpp: implementation of the Win32Net class.
//
//////////////////////////////////////////////////////////////////////

#include "Win32Net.h"
#include <process.h>
#include <string.h>
#include <stdlib.h>
#include <winsock.h>
#include "MError.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static bool fWsaStarted = false;

static unsigned long __stdcall
NonClassFunctionDummy(void *pNet)
{
	((Win32Net *)pNet)->Thread();
	return 0;
}

Win32Net::Win32Net(char *szHost,int nPort) : MNet(szHost,nPort)
{
	pFirstLine = NULL;
	szBuffer = strdup("");
	this->nPort = nPort;
	this->szHost = strdup(szHost);

	if((hWait = CreateEvent(NULL,FALSE,FALSE,NULL)) == NULL)
	{
		MError("CreateEvent failed, low on resources?\n");
	}

	if((LinesSemaphore = CreateSemaphore(NULL,1,10,NULL)) == NULL)
	{
		MError("CreateSemaphore failed, low on resources?\n");
	}

	DWORD nId;
	if((NetThread = CreateThread(NULL,0,&NonClassFunctionDummy,this,0,&nId)) == NULL)
	{
		MError("CreateThread failed, low on resources?\n");
	}
	// Wait for the network to initialize.
	WaitForSingleObject(LinesSemaphore,INFINITE);

}

Win32Net::~Win32Net()
{
	if(NetThread != NULL)
	{
		TerminateThread(NetThread,0);
	}
	
	free(szHost);
	
	if(LinesSemaphore != NULL)
	{
		CloseHandle(LinesSemaphore);
	}

	if(hWait)
	{
		CloseHandle(hWait);
	}

	struct LinkedList_s	*pNow = pFirstLine;
	while(pNow != NULL)
	{
		pNow = pNow->pNext;
		free(pFirstLine->szText);
		free(pFirstLine);
		pFirstLine = pNow;
	}

	if(szBuffer != NULL)
	{
		free(szBuffer);
	}

	closesocket(nSocket);
}

char *
Win32Net::ReadLn()
{
	char *szRet;
	WaitForSingleObject(LinesSemaphore,INFINITE);

	if(pFirstLine == NULL)
	{
		szRet = NULL;
	}
	else
	{
		struct LinkedList_s *pNow = pFirstLine;
		szRet = pNow->szText;
		pFirstLine = pNow->pNext;
		free(pNow);

	}

	ReleaseSemaphore(LinesSemaphore,1,NULL);

	return szRet;
}

void
Win32Net::Write(char *szFmt,...)
{
	va_list vl;
	char szData[1024*10];
	va_start (vl, szFmt);
	vsprintf (szData, szFmt, vl);
	va_end (vl);
	if(send(nSocket,szData,strlen(szData),0) == SOCKET_ERROR)
	{
		MError("Could not send data over the network\n");
	}

}

void
Win32Net::Dispatch(char *szLine)
{
	WaitForSingleObject(LinesSemaphore,INFINITE);

	struct LinkedList_s	**ppNow = &pFirstLine;
	while(*ppNow != NULL)
	{
		ppNow = &((*ppNow)->pNext);
	}
	*ppNow = (struct LinkedList_s *)malloc(sizeof(struct LinkedList_s));
	(*ppNow)->pNext = NULL;
	(*ppNow)->szText = szLine;

	SetEvent(hWait);

	ReleaseSemaphore(LinesSemaphore,1,NULL);
}

void
Win32Net::Thread()
{
	struct hostent *pHostEnt;
	unsigned long nAddr;
	struct sockaddr_in ServerAddr;


	// Startup

	if(fWsaStarted == false)
	{
		fWsaStarted = true;
		WSADATA wsaData;
		if(WSAStartup(MAKEWORD( 1, 1 ),&wsaData) != 0)
		{
				MError("winsock.dll could not be loaded");
				return;
		}
	}

	// Namelookup

	// We dont want to do any unnessery dns requests.
	if(inet_addr(szHost) == INADDR_NONE)
	{
		if((pHostEnt = gethostbyname(szHost)) != NULL)
		{
			free(szHost);
			szHost = (char *)malloc(3*4);
			sprintf(szHost, "%d.%d.%d.%d",
					(byte)pHostEnt->h_addr_list[0][0],
					(byte)pHostEnt->h_addr_list[0][1],
					(byte)pHostEnt->h_addr_list[0][2],
					(byte)pHostEnt->h_addr_list[0][3]);
		}
	}

	if((nAddr = inet_addr(szHost)) == INADDR_NONE)
	{
		MError("Host '%s' not found, can not connect\n",szHost);
	}

	// Connect.

	if ((nSocket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		MError("Could not create a new socket\n");
	}

	memset (&ServerAddr, 0, sizeof (ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons (nPort);
	ServerAddr.sin_addr.S_un.S_addr = nAddr;

    if (connect (nSocket, (struct sockaddr *) &ServerAddr, sizeof (ServerAddr)) < 0)
	{
		MError("Connection to '%s' failed\n",szHost);
	}

	// Release the other thread.
	ReleaseSemaphore(LinesSemaphore,1,NULL);

	// Read.

	while(1)
	{
		char szData[1024*10];
		long	nRecived;
		if((nRecived = recv(nSocket,(char *)&szData,sizeof(szData)-1,0)) == SOCKET_ERROR)
		{
			MError("Could not recive data from host, connection closed?\n");
		}
		szData[nRecived] = 0;

		// Resize the buffer
		szBuffer = (char *)realloc(szBuffer,strlen(szData)+strlen(szBuffer)+1);
		// Copy the new data.
		strcat(szBuffer,szData);
		
		// Split the data into lines.
		char *szReturn;
		while((szReturn = strchr(szBuffer,'\r')) != NULL)
		{
			// Terminate the string.
			*(szReturn++) = '\0';
			// Dispatch a new string.
			Dispatch(strdup(szBuffer));
			// Im reciving \n's A small cludge to remove them.
			if(*szReturn == '\n')
			{
				szReturn ++;
			}
			// Move the old data. 
			// (I wont bother to resize, that will be done when the next network packet is recived)
			memmove(szBuffer,szReturn,strlen(szReturn)+1);
		}
	}
}