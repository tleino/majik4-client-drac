// Win32Con.h: interface for the Win32Con class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(_WIN32CON_H_) && defined(WIN32) 
#define _WIN32CON_H_

#include "MCon.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <conio.h>

class Win32ConWindow : public MConWindow
{
public:
	Win32ConWindow(MCon *pCon,char *szName,MCoord &Pos,MCoord &Size);
	virtual ~Win32ConWindow();
	virtual void	PutCh(char Char,MColor_t Color=MCOLOR_NOCHANGE);
	virtual void	DelCh(int nToDel=1);
	virtual void	DelLine(int nToDel=1);
	virtual void	Scroll(MCoord &Dir);
	virtual	void	Clear();
	virtual void	Write(char *szFmt,...);
	virtual void	Update();
	virtual void	Move(MCoord &Pos);
	virtual MCoord	GetSize() { return MCoord(Size.X,Size.Y); };
	int	CheckScroll();
	void SetColor(MColor_t Color);

public:
	COORD Size,Pos;
	CHAR_INFO	*pChars,*pCharNow;
	WORD Attrib;
	MColor_t LastColor;
	bool	bModified;
};

class Win32Con : public MCon
{
public:
	// Constructors/destructors..
	Win32Con();
	virtual ~Win32Con();
	virtual MConWindow *NewWindow(MCoord &Pos,MCoord &Size,char *szTitle);
	virtual MCoord GetSize();
	virtual void Clear();
	virtual void Update();
	virtual char GetCh();

public:
	void PutStr(char *pChr,MColor_t Color);
	void SetFgColor(MColor_t Color);
	void SetCurrentWindow(Win32ConWindow *pWin);
	void DelCh(int nToDel);
	void DelLine(int nToDel);
	void Scroll(MCoord &Dir);
	MCoord GetCursorPos();

public:
	HANDLE hStdOut;
	HANDLE hStdIn;
	Win32ConWindow *pLastWin;
};

#endif
