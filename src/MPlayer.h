// MPlayer.h: interface for the MPlayer class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MPLAYER_H_
#define _MPLAYER_H_

#include "MCon.h"

class MPlayer  
{
public:
	MPlayer();
	virtual ~MPlayer();
	void	Display(MConWindow *pWin);

public:
	void	SetPos(MCoord Pos) { 	bUpdated = true;	this->Pos = Pos; };
	MCoord GetPos() { return Pos; };
	void	SetStats(char *szData);
	void	SetHp(int nHp) { bUpdated = true;	this->nHp = nHp; };
	void	SetEp(int nEp) { bUpdated = true;	this->nEp = nEp; };
	void	SetSp(int nSp) { bUpdated = true;	this->nSp = nSp; };

private:
	bool	bUpdated;
	typedef struct { int nMax, nCurrent; } Attribute_t;
	Attribute_t	Str,Dex,Stu,Con,Agi,Int,Wis,Pow,Cha;

	char	szName[12];

	unsigned long	nMoney;
	int		nHp,nEp,nSp;
	
	MCoord	Pos;
	int		nSpeed;
	bool	bFreeze;
	bool	bOnline;

};

#endif
