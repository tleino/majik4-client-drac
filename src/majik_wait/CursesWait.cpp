// Win32Wait.cpp: implementation of the Win32Wait class.
//
//////////////////////////////////////////////////////////////////////

#include "MError.h"
#include "CursesWait.h"
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CursesWait::CursesWait(UnixNet *pNet)
{
	this->nSocket = pNet->nSocket;
}

CursesWait::~CursesWait()
{

}

void
CursesWait::Wait()
{
	fd_set readfds;

	FD_ZERO(&readfds);
	FD_SET(0, &readfds);	// Stdio.
	FD_SET(nSocket, &readfds);

	select(nSocket + 1, &readfds, NULL, NULL, NULL);
}

