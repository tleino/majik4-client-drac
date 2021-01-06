// Win32Wait.h: interface for the Win32Wait class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WIN32WAIT_H_
#define _WIN32WAIT_H_

#include "MWait.h"
#include "CursesCon.h"
#include "UnixNet.h"

class CursesWait : public MWait  
{				 
public:
	CursesWait(UnixNet *pNet);
	virtual ~CursesWait();
	virtual	void Wait();
private:
	int nSocket;
};

#endif 
