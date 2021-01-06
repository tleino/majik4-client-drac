// Win32Wait.cpp: implementation of the Win32Wait class.
//
//////////////////////////////////////////////////////////////////////

#include "merror.h"
#include "Win32Wait.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Win32Wait::Win32Wait(Win32Con *pCon,Win32Net *pNet)
{
	aHandles[0] = pNet->hWait;
	aHandles[1] = pCon->hStdIn;
}

Win32Wait::~Win32Wait()
{

}

void
Win32Wait::Wait()
{
	if(WaitForMultipleObjects(2,aHandles,FALSE,INFINITE) == WAIT_FAILED)
	{
		MError("Wait failed\n");
	}
}

