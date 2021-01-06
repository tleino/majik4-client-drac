
#ifndef _MNET_H_
#define _MNET_H_

#include <stdio.h>

class MNet
{
public:
	MNet(char *szHost,int nPort);					// Connect to a host.
	virtual ~MNet();								// Dissconnect.

public:
	virtual char *ReadLn() = NULL;					// Reads a line of data, returns NULL if there is no line ready. (The tailing \r is removed)
	virtual void Write(char *szFmt,...) = NULL;	// Writes data.
};

#endif
