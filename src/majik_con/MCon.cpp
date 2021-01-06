// MCon.cpp: implementation of the MCon class.
//
//////////////////////////////////////////////////////////////////////

#include "MCon.h"
#include <string.h>
#include <stdarg.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void
MCon::AddWindow(MConWindow *pNew)
{
	pNew->pNext = pFirstWindow;
	pFirstWindow = pNew;
}

MConWindow *
MCon::GetWindow(char *szName)
{
	MConWindow *pFound = NULL;
	MConWindow *pNow = pFirstWindow;
	while(pNow != NULL)
	{
		if(strcmp(szName,pNow->szName) == 0)
			return pNow;
		pNow = pNow->pNext;
	}
	return NULL;
}

void
MCon::DelWindow(MConWindow *pToDel)
{
	MConWindow **ppNow = &pFirstWindow;
	while(*ppNow != NULL)
	{
		if(*ppNow == pToDel)
			break;
		ppNow = &((*ppNow)->pNext);
	}

	if(*ppNow != NULL)
		*ppNow = (*ppNow)->pNext;
}

void 
MConWindow::Box(MCoord &Pos,MCoord &Size)
{
	int x,y;
	if(Size.x < 2 || Size.y < 2)
		return;
	Move(Pos);
	PutCh(MCHAR_ULCORNER);
	for(x=0;x<(Size.x-2);x++)
		PutCh(MCHAR_HLINE);
	PutCh(MCHAR_URCORNER);
	for(y=Pos.y+1;y<Pos.y+Size.y-1;y++)
	{
		Move(MCoord(Pos.x,y));
		PutCh(MCHAR_VLINE);
		Move(MCoord(Pos.x+(Size.x-1),y));
		PutCh(MCHAR_VLINE);
	}
	Move(MCoord(Pos.x,Pos.y+Size.y-1));
	PutCh(MCHAR_LLCORNER);
	for(x=0;x<(Size.x-2);x++)
		PutCh(MCHAR_HLINE);
	PutCh(MCHAR_LRCORNER);
}

void
MConWindow::WriteWord(char *szFmt,...)
{
	va_list vl;
	static char buf[8192];

	va_start (vl, szFmt);
	vsprintf (buf, szFmt, vl);
	va_end (vl);
	

	int i;
	int nWords = 0;
	int nLen = strlen(buf);
	for(i=0;i<nLen;i++)
	{
		if(buf[i] == ' ')
		{
			nWords++;
			buf[i] = '\0';
		}
	}

	char *szNow = buf;
	int nWidth = 0;
	for(i=0;i<=nWords;i++)
	{
		if(strlen(szNow)+nWidth+1 >= GetWidth())
		{
			PutCh('\n');
			nWidth = 0;
		}


		Write("%s ",szNow);
		nWidth += strlen(szNow)+1;
		szNow += strlen(szNow)+1;
	}
}
