// Merror.cpp: implementation of the Merror class.
//
//////////////////////////////////////////////////////////////////////

#include "majik.h"
#include "MError.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void
MError(char *szFmt,...)
{
	va_list vl;
	static char buf[8192];

	va_start (vl, szFmt);
	vsprintf (buf, szFmt, vl);
	va_end (vl);
	printf(buf);
	exit(1);
}

void
MDebug(char *szFmt,...)
{
	va_list vl;
	static char buf[8192];

	va_start (vl, szFmt);
	vsprintf (buf, szFmt, vl);
	va_end (vl);

	MConWindow *pMsg;
	if(pCon != NULL)
	{
		if((pMsg = pCon->GetWindow("message")) == NULL)
		{
			printf(buf);
		}
		else
		{
			pMsg->Write("CLIENT DEBUG: %s",buf);
			pMsg->Update();
		}
	}

}
