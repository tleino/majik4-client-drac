

#ifndef _UNIXNET_H_
#define _UNIXNET_H_

#include "MNet.h"

class UnixNet  : public MNet
{
public:
	UnixNet(char *szHost,int nPort);					// Connect to a host.
	virtual ~UnixNet();
public:
	virtual char *ReadLn();					// Reads a line of data, returns NULL if there is no line ready. (The tailing \r is removed)
	virtual void Write(char *szFmt,...);	// Writes data.

public:
	int		nSocket;
	char	*szBuffer;
};

#endif
