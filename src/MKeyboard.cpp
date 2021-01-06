// MKeyboard.cpp: implementation of the MKeyboard class.
//
//////////////////////////////////////////////////////////////////////
#include "majik.h"
#include "MKeyboard.h"
#include "MError.h"
#include <stdlib.h>
#include <string.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MKeyboard Keyboard;

MKeyboard::MKeyboard()
{
	nDefaultState = nState = MKEYBOARD_STATE_DISABLED;
	pWin = NULL;
	szPrompt = NULL;
}

MKeyboard::~MKeyboard()
{

}

void
MKeyboard::ReadAndSendLine(int nCommand,char *szPrompt,bool bEcho)
{
	if(this->szPrompt != NULL)
	{
		free(this->szPrompt);
	}
	nState = MKEYBOARD_STATE_READLINE;
	this->nCommand = nCommand;
	this->szPrompt = strdup(szPrompt);
	this->bEcho = bEcho;
	strcpy(szLine,"");
	pWin->Move(MCoord(0,0));
	pWin->Write(szPrompt);
}

void
MKeyboard::ReciveChar(char Char)
{
	switch(nState)
	{
	case MKEYBOARD_STATE_READLINE:
		{
			if(Char == '\r')
			{
				pWin->Clear();
				MCP.Send(nCommand,szLine);
				nState = nDefaultState;
			}
			else
			{
				int nLen = strlen(szLine);
				szLine[nLen++] = Char;
				szLine[nLen++] = '\0';

				pWin->Move(MCoord(0,0));
				pWin->Write("%s%s",szPrompt,szLine);
			}
		}
		break;
	case MKEYBOARD_STATE_READCHAR:
		MCP.Send(nCommand,"%c",Char);
		nState = nDefaultState;
		break;
	}
}
