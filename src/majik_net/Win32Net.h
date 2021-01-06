

#ifndef _WIN32NET_H_
#define _WIN32NET_H_

#include <windows.h>
#include "mnet.h"

class Win32Net  : public MNet
{
public:
	Win32Net(char *szHost,int nPort);					// Connect to a host.
	virtual ~Win32Net();
	void	Thread();									// The new thread.
public:
	virtual char *ReadLn();					// Reads a line of data, returns NULL if there is no line ready. (The tailing \r is removed)
	virtual void Write(char *szFmt,...);	// Writes data.

	void	Dispatch(char *szString);
public:
	char	*szHost;
	int		nPort;
	int		nSocket;
	char	*szBuffer;
	HANDLE	hWait;	// The wait handle.
	HANDLE	NetThread;
	HANDLE	LinesSemaphore;	// We need a semaphore to be sure that the linked list of lines recived is ok.
	struct LinkedList_s {
		char *szText;
		struct LinkedList_s *pNext;
	};
	struct LinkedList_s *pFirstLine;

};

#endif
