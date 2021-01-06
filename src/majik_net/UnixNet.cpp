// Win32Net.cpp: implementation of the Win32Net class.
//
//////////////////////////////////////////////////////////////////////

#include "UnixNet.h"
#include <string.h>
#include <stdlib.h>
#include "MError.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <stdarg.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

UnixNet::UnixNet(char *szHost,int nPort) : MNet(szHost,nPort)
{
	struct hostent *pHostEnt;
	unsigned long nAddr;
	struct sockaddr_in ServerAddr;

	szBuffer = strdup("");

	// Namelookup

	// We dont want to do any unnessery dns requests.
	if(inet_addr(szHost) == INADDR_NONE)
	{
		if((pHostEnt = gethostbyname(szHost)) != NULL)
		{
			free(szHost);
			szHost = (char *)malloc(3*4);
			sprintf(szHost, "%d.%d.%d.%d",
					(unsigned char)pHostEnt->h_addr_list[0][0],
					(unsigned char)pHostEnt->h_addr_list[0][1],
					(unsigned char)pHostEnt->h_addr_list[0][2],
					(unsigned char)pHostEnt->h_addr_list[0][3]);
		}
	}

	memset (&ServerAddr, 0, sizeof (ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons (nPort);

	if(inet_aton(szHost,&ServerAddr.sin_addr) <= 0)
	{
		MError("Host '%s' not found, can not connect\n",szHost);
	}

	// Connect.

	if ((nSocket = socket (AF_INET, SOCK_STREAM, 0)) < 0)
	{
		MError("Could not create a new socket\n");
	}

    if (connect (nSocket, (struct sockaddr *) &ServerAddr, sizeof (ServerAddr)) < 0)
	{
		MError("Connection to '%s' failed\n",szHost);
	}

}

UnixNet::~UnixNet()
{

	if(szBuffer != NULL)
	{
		free(szBuffer);
	}
}

char *
UnixNet::ReadLn()
{
	unsigned long nLen=0;

	do {
		ioctl(nSocket,FIONREAD,&nLen);
		if(nLen != 0)
		{
			char szData[1024*10];
			long	nRecived;
			if((nRecived = recv(nSocket,(char *)&szData,sizeof(szData)-1,0)) == -1)
			{
				MError("Could not recive data from host, connection closed?\n");
			}
			szData[nRecived] = 0;

			// Resize the buffer
			szBuffer = (char *)realloc(szBuffer,strlen(szData)+strlen(szBuffer)+1);
			// Copy the new data.
			strcat(szBuffer,szData);
		}
	} while(nLen != 0);

	// Split the data into lines.
	char *szReturn;
	while((szReturn = strchr(szBuffer,'\r')) != NULL)
	{
		char *szRet = NULL;
		// Terminate the string.
		*(szReturn++) = '\0';
		// Dispatch a new string.
		szRet = strdup(szBuffer);
		// Im reciving \n's A small cludge to remove them.
		if(*szReturn == '\n')
		{
			szReturn ++;
		}
		// Move the old data. 
		// (I wont bother to resize, that will be done when the next network packet is recived)
		memmove(szBuffer,szReturn,strlen(szReturn)+1);

		return szRet;
	}
	return NULL;
}

void
UnixNet::Write(char *szFmt,...)
{
	va_list vl;
	char szData[1024*10];
	va_start (vl, szFmt);
	vsprintf (szData, szFmt, vl);
	va_end (vl);

	if(send(nSocket,szData,strlen(szData),0) == -1)
	{
		MError("Could not send data over the network\n");
	}

}
