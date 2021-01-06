// Win32Con.cpp: implementation of the Win32Con class.
//
//////////////////////////////////////////////////////////////////////
#include "MError.h"
#include "Win32Con.h"
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
Win32Con::Win32Con()
{
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	pLastWin = NULL; 
	SetConsoleMode(hStdIn,ENABLE_MOUSE_INPUT);
}

Win32Con::~Win32Con()
{

}

char
Win32Con::GetCh()
{
	DWORD nRead;
	while(1)
	{
		INPUT_RECORD	InRec;
		if(!GetNumberOfConsoleInputEvents(hStdIn,&nRead))
		{
			MError("GetNumberOfConsoleInputEvents failed\n");
		}

		if(nRead > 0)
		{
			if(!ReadConsoleInput(hStdIn,&InRec,1,&nRead))
			{
				MError("ReadConsoleInput failed\n");

			}
			switch(InRec.EventType)
			{
			case KEY_EVENT:
				if(InRec.Event.KeyEvent.bKeyDown)
				{
					switch(InRec.Event.KeyEvent.wVirtualKeyCode)
					{
					case VK_F1:
						return MKEY_F1;
					case VK_F2:
						return MKEY_F2;
					case VK_F3:
						return MKEY_F3;
					case VK_F4:
						return MKEY_F4;
					case VK_F5:
						return MKEY_F5;
					case VK_F6:
						return MKEY_F6;
					case VK_F7:
						return MKEY_F7;
					case VK_F8:
						return MKEY_F8;
					case VK_F9:
						return MKEY_F9;
					case VK_F10:
						return MKEY_F10;
					case VK_F11:
						return MKEY_F11;
					case VK_F12:
						return MKEY_F12;
					case VK_UP:
						return MKEY_UP;
					case VK_DOWN:
						return MKEY_DOWN;
					case VK_LEFT:
						return MKEY_LEFT;
					case VK_RIGHT:
						return MKEY_RIGHT;
					case VK_HOME:
						return MKEY_HOME;
					case VK_END:
						return MKEY_END;
					case VK_INSERT:
						return MKEY_INSERT;
					case VK_DELETE:
						return MKEY_DELETE;
					case VK_PRIOR:
						return MKEY_PAGE_UP;
					case VK_NEXT:
						return MKEY_PAGE_DOWN;

					default:
						return InRec.Event.KeyEvent.uChar.AsciiChar;
					}
				}
				break;
			}
		}
		else
		{
			break;
		}
	}
	return -1;
}

MConWindow *
Win32Con::NewWindow(MCoord &Pos,MCoord &Size,char *szTitle)
{
	Win32ConWindow *pNew = new Win32ConWindow(this,szTitle,Pos,Size);
	AddWindow(pNew);
	return pNew;
}

MCoord
Win32Con::GetSize()
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	if(!GetConsoleScreenBufferInfo(hStdOut,&Info))
	{
		MError("Failed to get the screen size, should not happend\n");
	}
	return MCoord(Info.dwSize.X,Info.dwSize.Y);
}

void
Win32Con::Update()
{
	Win32ConWindow *pNow;
	pNow = (Win32ConWindow *)pFirstWindow;
	while(pNow != NULL)
	{
		pNow->Update();
		pNow = (Win32ConWindow *)(pNow->pNext);
	}

}

void
Win32Con::Clear()
{
}

/////////////////////
// Win32ConWindow. //
/////////////////////
Win32ConWindow::Win32ConWindow(MCon *pCon,char *szName,MCoord &Pos,MCoord &Size) : MConWindow(pCon,szName)
{
	this->Pos.X = Pos.x;
	this->Pos.Y = Pos.y;
	this->Size.X = Size.x;
	this->Size.Y = Size.y;
	pChars = (CHAR_INFO *)malloc(sizeof(CHAR_INFO)*(Size.y*Size.x));
	LastColor = MCOLOR_NOCHANGE;
	Attrib = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
	Clear();
}

Win32ConWindow::~Win32ConWindow()
{
	free(pChars);
}

int
Win32ConWindow::CheckScroll()
{
	int nLen = (int)(pCharNow-pChars);
	nLen -= Size.X*Size.Y;
	if(nLen >= 0)
	{
		memmove(pChars,pChars+Size.X,Size.X*Size.Y*sizeof(CHAR_INFO));
		memset(pChars+(Size.X*(Size.Y-1)),0,Size.X*sizeof(CHAR_INFO));
		nLen -= Size.X;
		pCharNow-=Size.X;
	}
	return -nLen;
}


void
Win32ConWindow::SetColor(MColor_t Color)
{
	WORD nColor;
	if(Color == MCOLOR_NOCHANGE || Color == LastColor)
		return;
	switch(Color)
	{
	case MCOLOR_BLACK:
		nColor = 0;
		break;
	case MCOLOR_RED:
		nColor = FOREGROUND_RED;
		break;
	case MCOLOR_GREEN:
		nColor = FOREGROUND_GREEN;
		break;
	case MCOLOR_BROWN:
		nColor = FOREGROUND_RED|FOREGROUND_GREEN; // This is dark yellow, the closest I got.
		break;
	case MCOLOR_BLUE:
		nColor = FOREGROUND_BLUE;
		break;
	case MCOLOR_MAGENTA:
		nColor = FOREGROUND_BLUE|FOREGROUND_RED;
		break;
	case MCOLOR_CYAN:
		nColor = FOREGROUND_BLUE|FOREGROUND_GREEN;
		break;
	case MCOLOR_WHITE:
	case MCOLOR_LIGHT_GRAY:
		nColor = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_DARK_GRAY:
		nColor = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED;
		break;
	case MCOLOR_LIGHT_RED:
		nColor = FOREGROUND_RED|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_LIGHT_GREEN:
		nColor = FOREGROUND_GREEN|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_YELLOW:
		nColor = nColor = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_LIGHT_BLUE:
		nColor = FOREGROUND_BLUE|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_LIGHT_MAGENTA:
		nColor = FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY;
		break;
	case MCOLOR_LIGHT_CYAN:
		nColor = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
		break;
	default:
		MDebug("Unknown color '%c'\n",Color);
		return;
	}
	LastColor = Color;
	Attrib = nColor;
}

void
Win32ConWindow::PutCh(char Char,MColor_t Color)
{
	bModified = true;
	CheckScroll();
	SetColor(Color);
	switch(Char)
	{
	case '\n':
		{
			int nWhere = (int)(pCharNow-pChars);
			nWhere %= Size.X;
			pCharNow += Size.X-nWhere;
		}
		return;
	case MCHAR_ULCORNER:
		Char = 'Ú';
		break;
	case MCHAR_LLCORNER:
		Char = 'À';
		break;
	case MCHAR_URCORNER:
		Char = '¿';
		break;
	case MCHAR_LRCORNER:
		Char = 'Ù';
		break;
	case MCHAR_LTEE:
		Char = 'Ã';
		break;
	case MCHAR_RTEE:
		Char = '´';
		break;
	case MCHAR_TTEE:
		Char = 'Â';
		break;
	case MCHAR_BTEE:
		Char = 'Á';
		break;
	case MCHAR_VLINE:
		Char = '³';
		break;
	case MCHAR_HLINE:
		Char = 'Ä';
		break;
	case MCHAR_PLUS:
		Char = 'Å';
		break;
	}
	pCharNow->Attributes = Attrib;
	pCharNow->Char.AsciiChar = Char;
	pCharNow++;
}

void
Win32ConWindow::DelCh(int nToDel)
{
	bModified = true;
	int i;
	CheckScroll();
	int nWhere = (int)(pCharNow-pChars);
	nWhere %= Size.X;
	for(i=0;i<nToDel;i++)
	{
		memmove(pCharNow,pCharNow+1,sizeof(CHAR_INFO)*((Size.X-nWhere)-1));
		(pCharNow+(Size.X-(nWhere+1)))->Attributes = Attrib;
		(pCharNow+(Size.X-(nWhere+1)))->Char.AsciiChar = ' ';
	}
}

void
Win32ConWindow::DelLine(int nToDel)
{
	bModified = true;
	int i;
	CheckScroll();
	int nWhere = (int)(pCharNow-pChars);
	nWhere /= Size.X;

	for(i=0;i<nToDel;i++)
	{
		memmove(pChars+(nWhere*Size.X),pChars+((nWhere+1)*Size.X),sizeof(CHAR_INFO)*(((Size.Y-1)-nWhere)*Size.X));
		memset(pChars+((Size.Y-1)*Size.X),0,sizeof(CHAR_INFO)*Size.X);
	}

}

void
Win32ConWindow::Scroll(MCoord &Dir)
{

}

void
Win32ConWindow::Write(char *szFmt,...)
{
	WORD	nAttrib;
	int  i, inverse;
	va_list vl;
	static char buf[8192];

	va_start (vl, szFmt);
	vsprintf (buf, szFmt, vl);
	va_end (vl);


	nAttrib = Attrib;

	for (i=0;i<strlen(buf);i++)
	{
		if (i < strlen(buf) - 3)
		{
			if (buf[i] == '^' && (buf[i+1] == 'c' || buf[i+1] == 'b'))
			{
				if (i > 0 && buf[i-1] == '/')
                {
					i += 3;
					continue;
                }

				inverse = FALSE;
                                  
				if (buf[i+1] == 'b')
					inverse = TRUE;
                                  
				SetColor((MColor_t)buf[i+2]);
				i += 3;
			}
		}
		PutCh(buf[i]);
	}
	Attrib = nAttrib;
}

void
Win32ConWindow::Move(MCoord &Pos)
{
	pCharNow = pChars + (Pos.x+(Size.X*(Pos.y)));
}

void
Win32ConWindow::Update()
{
	if(bModified == false)
		return;
	bModified = false;
	SMALL_RECT Window;
	COORD Zero = {0,0};
	Window.Left = Pos.X;
	Window.Top = Pos.Y;
	Window.Right = Pos.X+Size.X;
	Window.Bottom = Pos.Y+Size.Y;
	if(!WriteConsoleOutput(((Win32Con *)pCon)->hStdOut,pChars,Size,Zero,&Window))
	{
		MError("Failed to copy data to screen\n");
	}
}

void
Win32ConWindow::Clear()
{
	bModified = true;
	int i;
	for(i=0;i<Size.X*Size.Y;i++)
	{
		pChars[i].Attributes = Attrib;
		pChars[i].Char.AsciiChar = ' ';
	}

	Move(MCoord(0,0));
}
