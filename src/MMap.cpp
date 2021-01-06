// MMap.cpp: implementation of the MMap class.
//
//////////////////////////////////////////////////////////////////////

#include "MMap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "MError.h"
#include <math.h>
#include "majik.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MMap Map;

MMap::MMap()
{
	int i;
	bLock = false;
	szMapChars = NULL;
	aLightMap = NULL;
	FILE	*pFile;

	for(i=0;i<255;i++)
	{
		aTiles[i].nVisability = 0;
		strcpy(aTiles[i].szDesc,"Unknown?");
		strcpy(aTiles[i].szString,"^cp?");
	}

	char szLine[1024*10];
	pFile = fopen("terrain.txt","r");
	if(pFile != NULL)
	{
		while(fgets(szLine,sizeof(szLine),pFile) != NULL)
		{
			if(szLine[0] == ';')
				continue;
			
			char	szDesc[20];
			char	szString[10];
			char	nChar;
			int		nVisability;

			if(sscanf(szLine,"%s %c %d %s",szDesc,&nChar,&nVisability,szString) != 4)
				continue;

			aTiles[nChar].nVisability = nVisability;
			strcpy(aTiles[nChar].szDesc,szDesc);
			strcpy(aTiles[nChar].szString,szString);
		}
		fclose(pFile);
	}

}

MMap::~MMap()
{
	if(szMapChars != NULL)
	{
		free(szMapChars);
	}
	if(aLightMap != NULL)
	{
		free(aLightMap);
	}
}

void
MMap::UpdateMap(char *szData)
{
	char *szTmp;
	int x,y;
	if(szData == NULL)
		return;
	if((szTmp = strchr(szData,';')) == NULL)
	{
		return;
	}
	Size.x = (szTmp-szData)/sizeof(char);
	Size.y = strlen(szData)/(Size.x+1);

	if(szMapChars != NULL)
	{
		free(szMapChars);
	}
	szMapChars = (char *)malloc(Size.x*Size.y*sizeof(char));

	if(aLightMap != NULL)
	{
		free(aLightMap);
	}
	aLightMap = (int *)malloc(Size.x*Size.y*sizeof(int));

	for(y=0;y<Size.y;y++)
	{
		for(x=0;x<Size.x;x++)
		{
			szMapChars[x+(y*Size.x)] = szData[x+(y*(Size.x+1))];
		}
	}
}

int
MMap::Los(MCoord &A,MCoord &B,bool bOnMap)
{
	int nVis=0;
	int	nLen;
	float	dx,dy;
	float	x,y;

	dx = (float)(B.x-A.x);
	dy = (float)(B.y-A.y);
	
	if(abs((int)dx) > abs((int)dy))
		nLen = abs((int)dx);
	else
		nLen = abs((int)dy);

	dx /= (float)nLen;
	dy /= (float)nLen;

	x = ((float)A.x)+0.5f;
	y = ((float)A.y)+0.5f;
	if(bOnMap == false)
	{
		x -= MapPos.x;
		y -= MapPos.y;
	}
	// Skip the first and the last tile. We want to see the walls, 
	// and we want to be able to look around from inside a stone.
	x += dx;
	y += dy;
	nLen -=2;

	for(;nLen >= 0;nLen--)
	{
		if(x>=0 && x < Size.x && y>=0 && y < Size.y)
		{

			char	cMap = szMapChars[(int)x+(Size.x*(int)y)];

			nVis += aTiles[cMap].nVisability;

			x += dx;
			y += dy;
		}
	}

	return nVis;
}

void
MMap::Display(MConWindow *pWin,MCoord Center)
{
	int x,y;
	if(bLock == true || pWin == NULL || szMapChars == NULL)
		return;

	if(pWin == pLastWindow && Center == LastPos)
		return;

	LastPos = Center;
	pLastWindow = pWin;

	MCoord WinSize = pWin->GetSize();

	Center.x -= MapPos.x + WinSize.x/2;
	Center.y -= MapPos.y + WinSize.y/2;

	pWin->Move(MCoord(0,0));
	for(y=Center.y;y<Center.y+WinSize.y;y++)
	{
		for(x=Center.x;x<Center.x+WinSize.x;x++)
		{
			if(	x>=0 && x < Size.x && y>=0 && y < Size.y && 
				Los(MCoord(x+MapPos.x,y+MapPos.y),Player.GetPos()) < 100 )
			{
				pWin->Write(aTiles[szMapChars[x+(y*Size.x)]].szString);
			}
			else
			{
				pWin->PutCh(' ');
			}
		}
	}

	MObject_t	*pNow = pFirstObject;
	while(pNow != NULL)
	{
		MCoord Pos = pNow->Pos;
		Pos.x -= Center.x + MapPos.x;
		Pos.y -= Center.y + MapPos.y;

		if(	Pos.x >= 0 && Pos.x < WinSize.x &&
			Pos.y >= 0 && Pos.y < WinSize.y && 
			Los(MCoord(Pos.x+Center.x+MapPos.x,Pos.y+Center.y+MapPos.y),Player.GetPos()) < 100 &&
			pNow->nInvis == 0)
		{
			pWin->Move(Pos);
			pWin->PutCh(pNow->nChar,pNow->nColor);
		}
		pNow = pNow->pNext;
	}
}

void
MMap::AddObject(int nObjId)
{
	pLastWindow = NULL; // Force update.
	MObject_t	*pNew = new MObject_t;
	pNew->nObjId = nObjId;
	pNew->pNext = pFirstObject;
	pFirstObject = pNew;
}

MObject_t *
MMap::GetObject(int nObjId)
{
	pLastWindow = NULL; // Force update.
	MObject_t *pRet = pFirstObject;
	do {
		if(pRet->nObjId == nObjId)
		{
			return pRet;
		}
		pRet = pRet->pNext;
	} while(pRet != NULL);
	return NULL;
}

void
MMap::DelObject(int nObjId)
{
	pLastWindow = NULL; // Force update.
	MObject_t	**ppObj = &pFirstObject;
	while(*ppObj != NULL)
	{
		if((*ppObj)->nObjId == nObjId)
		{
			MObject_t *pNow = *ppObj;
			*ppObj = pNow->pNext;
			free(pNow);
			continue;
		}
		ppObj = &((*ppObj)->pNext);
	}
}

void
MMap::MapSlice(int nSide,char *szData)
{
	int y;
   
	switch (nSide)
	{
	case MAPPICE_TOP:
		memmove(szMapChars+Size.x,szMapChars,sizeof(char)*Size.x*(Size.y-1));
		memcpy(szMapChars,szData,sizeof(char)*Size.x);
		break;
	case MAPPICE_BOTTOM:
		memmove(szMapChars,szMapChars+Size.x,sizeof(char)*Size.x*(Size.y-1));
		memcpy(szMapChars+(Size.x*(Size.y-1)),szData,sizeof(char)*Size.x);
		break;
	case MAPPICE_RIGTH:
		memmove(szMapChars,szMapChars+1,sizeof(char)*((Size.x*Size.y)-1));
		for(y=0;y<Size.y;y++)
			szMapChars[(y*Size.x)+(Size.x-1)] = *(szData++);
		break;
	case MAPPICE_LEFT:
		memmove(szMapChars+1,szMapChars,sizeof(char)*((Size.x*Size.y)-1));
		for(y=0;y<Size.y;y++)
			szMapChars[(y*Size.x)] = *(szData++);
		break;
	default:
		break;
	}
   
}
