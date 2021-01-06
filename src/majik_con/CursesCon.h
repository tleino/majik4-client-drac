// CursesCon.h: interface for the Win32Con class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(_CURSESCON_H_) && defined(CURSES) 
#define _CURSESCON_H_

#include "MCon.h"
#include <curses.h>

class CursesConWindow : public MConWindow
{
public:
	CursesConWindow(MCon *pCon,char *szName,MCoord &Pos,MCoord &Size);
	virtual ~CursesConWindow();
	virtual void	PutCh(char Char,MColor_t Color=MCOLOR_NOCHANGE);
	virtual void	DelCh(int nToDel=1);
	virtual void	DelLine(int nToDel=1);
	virtual void	Scroll(MCoord &Dir);
	virtual	void	Clear();
	virtual void	Write(char *szFmt,...);
	virtual void	Update();
	virtual void	Move(MCoord &Pos);
	virtual MCoord	GetSize() { return Size; };
			void	SetColor(MColor_t Color);
public:
	WINDOW	*pWin;
	MColor_t	LastColor;
	MCoord Size;
	bool fUpdated;
};

class CursesCon : public MCon
{
public:
	// Constructors/destructors..
	CursesCon();
	virtual ~CursesCon();
	virtual MConWindow *NewWindow(MCoord &Pos,MCoord &Size,char *szTitle);
	virtual MCoord GetSize();
	virtual void Clear();
	virtual void Update();
	virtual char GetCh();

public:
	MCoord GetCursorPos();
};

#endif
