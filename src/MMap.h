// MMap.h: interface for the MMap class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MMAP_H_
#define _MMAP_H_
#include "MTypes.h"
#include "MCon.h"

typedef struct MObject_s {
	struct MObject_s *pNext;
	int		nObjId;
	int		nChar;
	MColor_t	nColor;
	int		nLight;
	int		nPhysical;
	int		nInvis;
	char	szName[12];

	MCoord	Pos;
} MObject_t;

typedef struct {
	char	szString[10];
	char	szDesc[20];
	int		nVisability;
} MTile_t;

class MMap  
{
public:
	MMap();
	virtual ~MMap();
	void UpdateMap(char *szData);
	void MapSlice(int nSide,char *szData);
	void Display(MConWindow *pWin,MCoord Center);
	void SetMapPos(MCoord Pos) { MapPos = Pos; };
	void Lock() { bLock = true; };
	void Release() { bLock = false; };
	void AddObject(int nObjId);
	MObject_t *GetObject(int nObjId);
	void DelObject(int nObjId);
	int Los(MCoord &A,MCoord &B,bool bOnMap = false);

public:
	MObject_t	*pFirstObject;
	bool	bLock;
	MCoord	Size;
	MCoord	MapPos;
	char	*szMapChars;
	int		*aLightMap;
	MCoord		LastPos;
	MConWindow	*pLastWindow;
	MTile_t	aTiles[256];
};

#define MAPPICE_TOP              0
#define MAPPICE_RIGTH            1 
#define MAPPICE_BOTTOM           2
#define MAPPICE_LEFT             3

#endif
