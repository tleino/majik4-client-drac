// Win32Wait.h: interface for the Win32Wait class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _WIN32WAIT_H_
#define _WIN32WAIT_H_

#include "MWait.h"
#include "win32con.h"
#include "win32net.h"

class Win32Wait : public MWait  
{
public:
	Win32Wait(Win32Con *pCon,Win32Net *pNet);
	virtual ~Win32Wait();
	virtual	void Wait();

public:
	HANDLE aHandles[2];
};

#endif 
