// MPlayer.cpp: implementation of the MPlayer class.
//
//////////////////////////////////////////////////////////////////////

#include "MPlayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MPlayer Player;

MPlayer::MPlayer()
{
	bUpdated = true;
}

MPlayer::~MPlayer()
{

}

void
MPlayer::Display(MConWindow *pWin)
{
	if(bUpdated == false)
		return;
	bUpdated = false;

	pWin->Write("%-12s Str:%2d Dex:%2d Stu:%2d Con:%2d Agi:%2d Int:%2d Wis:%2d Pow:%2d Cha:%2d\nHp: %3d Sp: %3d Ep: %3d Money: %6d X: %3d Y: %3d\n",
		szName,
		Str.nCurrent,Dex.nCurrent,Stu.nCurrent,Con.nCurrent,Agi.nCurrent,Int.nCurrent,Wis.nCurrent,Pow.nCurrent,Cha.nCurrent,
		nHp, nSp, nEp, nMoney, Pos.x,Pos.y);

	pWin->DelLine();
	//TODO: Output status..
}

void
MPlayer::SetStats(char *szData)
{
	if(szData == NULL)
	{
		return;
	}

	bUpdated = true;

	sscanf (szData, "%d %d %d %d %d %d %d %d %d",
		&Str.nCurrent,&Dex.nCurrent,&Stu.nCurrent,&Con.nCurrent,
		&Agi.nCurrent,&Int.nCurrent,&Wis.nCurrent,&Pow.nCurrent,
		&Cha.nCurrent);
}