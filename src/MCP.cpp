// MCP.cpp: implementation of the MCP class.
//
//////////////////////////////////////////////////////////////////////

#include "MError.h"
#include "majik.h"
#include "MCP.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Mcp MCP;

Mcp::Mcp()
{

}

Mcp::~Mcp()
{

}

void
Mcp::Send(int nCmd,char *szFmt,...)
{
	va_list vl;
	char szData[1024*10];
	va_start (vl, szFmt);
	vsprintf (szData, szFmt, vl);
	va_end (vl);
	pNet->Write("%d:%d:%s\n",nCmd,strlen(szData)+1,szData);
}

void
Mcp::Command(int nCommand,int nDatalen,char *szData)
{
	switch(nCommand)
	{
	case CMD_START:
		Keyboard.SetDefaultState(MKEYBOARD_STATE_READCHAR);
		Keyboard.ReadAndSendChar(CMD_CHAR);
		Send(CMD_START,"0.05");
		break;
	case CMD_ASK_NAME:
		Keyboard.SetDefaultState(MKEYBOARD_STATE_DISABLED);
		Keyboard.ReadAndSendLine(CMD_LOGIN_NAME,"Your character's name: ");
		break;
	case CMD_ASK_PASS:
		Keyboard.SetDefaultState(MKEYBOARD_STATE_DISABLED);
		Keyboard.ReadAndSendLine(CMD_LOGIN_PASS,"Password: ");
		break;
	case CMD_QUIT:
		MError(szData);
		break;

	case CMD_SEE:
	case CMD_HEAR:
	case CMD_SMELL:
	case CMD_FEEL:
	case CMD_TASTE:
	case CMD_SENSE:
	case CMD_OOC:
	case CMD_INFO:
	case CMD_MSG:
		break;
	case CMD_MAP:
		Map.UpdateMap(szData);
		break;
	case CMD_MAP_SLICE:
		{
			char szBuff[1024];
			int nSide;
			if(sscanf(szData, "%d %s", &nSide, szBuff) == 2)
			{
				Map.MapSlice(nSide,szBuff);
			}
		}
		break;
	case CMD_LOCK_MAP:
		Map.Lock();
		break;
	case CMD_RELEASE_MAP:
		Map.Release();
		break;
	case CMD_MAP_COORD:
		{
			MCoord Pos;
			if (sscanf (szData, "%d#%d", &Pos.x, &Pos.y) == 2)
			{
				Map.SetMapPos(Pos);
			}
		}
		break;
	case CMD_XYPOS:
		{
			MCoord Pos;
			if (sscanf (szData, "%d#%d", &Pos.x, &Pos.y) == 2)
			{
				Player.SetPos(Pos);
			}
		}
		break;
	case CMD_PLR_STAT:
		Player.SetStats(szData);
		break;
	case CMD_PLR_HP:
		Player.SetHp(atoi(szData));
		break;
	case CMD_PLR_SP:
		Player.SetSp(atoi(szData));
		break;
	case CMD_PLR_EP:
		Player.SetEp(atoi(szData));
		break;

	case CMD_ADD_OBJECT:
		Map.AddObject(atoi(szData));
		break;
	case CMD_DEL_OBJECT:
		Map.DelObject(atoi(szData));
		break;
	case CMD_MOVE_OBJECT:
		{
			int nObjId;
			MCoord Pos;
			if(sscanf(szData,"%d %d %d",&nObjId,&Pos.x,&Pos.y) != 3)
			{
				break;
			}
			MObject_t *pObj = Map.GetObject(nObjId);
			if(pObj == NULL)
			{
				break;
			}
			pObj->Pos = Pos;
		}
		break;
	case CMD_INFO_OBJECT:
		{
			int		nObjId;
			int		nChar;
			MColor_t	nColor;
			int		nLight;
			int		nPhysical;
			int		nInvis;
			char	szName[12];
            if (sscanf (szData, "%d %d %d %d %d %d %s", &nObjId,&nChar,&nColor,&nLight,&nPhysical,&nInvis,szName) != 7)
			{
				break;
			}
			MObject_t *pObj = Map.GetObject(nObjId);
			if(pObj == NULL)
			{
				break;
			}
			pObj->nChar = nChar;
			pObj->nColor = nColor;
			pObj->nLight = nLight;
			pObj->nPhysical = nPhysical;
			pObj->nInvis = nInvis;
			strcpy(pObj->szName,szName);
		}
		break;
	case CMD_DESCRIPTION:
		{
			MConWindow *pDesc = pCon->GetWindow("description");
			if(pDesc == NULL)
			{
				break;
			}
			pDesc->Clear();
			pDesc->WriteWord(szData);

		}
		break;
	default:
		MDebug("< %d:%s\n",nCommand,szData);

		break;
	}

}
