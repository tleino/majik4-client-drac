#include "CursesCon.h"
#include "MError.h"

CursesCon::CursesCon()
{
	int i;

	if(initscr() == NULL)
	{
		MError("Failed to initialize curses\n");
	}
   
	unsigned char ct[] =
	{
	        COLOR_BLACK, COLOR_BLUE, COLOR_GREEN, COLOR_CYAN, COLOR_RED,
	        COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE, COLOR_BLACK, COLOR_BLUE,
	        COLOR_GREEN, COLOR_CYAN, COLOR_RED, COLOR_MAGENTA, COLOR_YELLOW,
	        COLOR_WHITE
	};

    start_color ();

    for (i = 0; i < sizeof(ct)/sizeof(ct[0]); i++)
        init_pair (i, ct[i], COLOR_BLACK);

	// Dont echo chars typed by the user.
	noecho();
	// Dont do line buffer.
	cbreak();
	// No delay mode.
	timeout(0);
}

CursesCon::~CursesCon()
{
	// Closedown curses.
	endwin();
}

MCoord
CursesCon::GetSize()
{
	return MCoord(COLS,LINES);
}

MConWindow *
CursesCon::NewWindow(MCoord &Pos,MCoord &Size,char *szTitle)
{
	CursesConWindow *pNew = new CursesConWindow(this,szTitle,Pos,Size);
	AddWindow(pNew);
	return pNew;
}

void
CursesCon::Clear()
{
}

void
CursesCon::Update()
{
	MConWindow *pNow = pFirstWindow;
	while(pNow != NULL)
	{
		pNow->Update();
		pNow = pNow->pNext;
	}
	doupdate();
}

// CursesConWindow

CursesConWindow::CursesConWindow(MCon *pCon,char *szName,MCoord &Pos,MCoord &Size) : MConWindow(pCon,szName)
{
	pWin = newwin(Size.y,Size.x,Pos.y,Pos.x);
	if(pWin == NULL)
	{
		MError("Failed to create window %d,%d(%d,%d)\n",Pos.x,Pos.y,Size.x,Size.y);
	}
	this->Size = Size;
	keypad(pWin,TRUE);
	fUpdated = true;
}

CursesConWindow::~CursesConWindow()
{
	if(pWin != NULL)
	{
		delwin(pWin);
	}
}

void
CursesConWindow::Move(MCoord &Pos)
{
	wmove(pWin,Pos.y,Pos.x);
}

void
CursesConWindow::PutCh(char Char,MColor_t Color)
{
	fUpdated = true;
	waddch(pWin,Char);
}

void
CursesConWindow::DelCh(int nToDel)
{
	fUpdated = true;
	int i;
	for(i=0;i<nToDel;i++)
	{
		wdelch(pWin);
	}
}

void
CursesConWindow::DelLine(int nToDel)
{
	fUpdated = true;
	int i;
	for(i=0;i<nToDel;i++)
	{
		wdeleteln(pWin);
	}
}

void
CursesConWindow::Scroll(MCoord &Dir)
{
	fUpdated = true;
	wscrl(pWin,-Dir.y);
	//TODO: Fix x scrolling.
}

void
CursesConWindow::Clear()
{
	fUpdated = true;
	werase(pWin);
}

void
CursesConWindow::Update()
{
	if(fUpdated == false)
		return;
	fUpdated = false;
	wnoutrefresh(pWin);
}

void
CursesConWindow::SetColor(MColor_t Color)
{
	if(Color == MCOLOR_NOCHANGE || Color == LastColor)
		return;

	enum color_types
	{
	   C_BLACK, C_BLUE, C_GREEN, C_CYAN, C_RED, C_MAGENTA,
	   C_BROWN, C_LIGHT_GRAY, C_DARK_GRAY, C_LIGHT_BLUE,
	   C_LIGHT_GREEN, C_LIGHT_CYAN, C_LIGHT_RED, C_LIGHT_MAGENTA,
	   C_YELLOW, C_WHITE, MAX_COLOR
	};

 	int c;
	switch(Color)
	{
		case MCOLOR_BLACK:
			c = C_BLACK;
			break;
		case MCOLOR_RED:
			c = C_RED;
			break;
		case MCOLOR_GREEN:
			c = C_GREEN;
			break;
		case MCOLOR_BROWN:
			c = C_BROWN;
			break;
		case MCOLOR_BLUE:
			c = C_BLUE;
			break;
		case MCOLOR_MAGENTA:
			c = C_MAGENTA;
			break;
		case MCOLOR_CYAN:
			c = C_CYAN;
			break;
		case MCOLOR_WHITE:
			c = C_WHITE;
			break;
		case MCOLOR_LIGHT_GRAY:
			c = C_LIGHT_GRAY;
			break;
		case MCOLOR_DARK_GRAY:
			c = C_DARK_GRAY;
			break;
		case MCOLOR_LIGHT_RED:
			c = C_LIGHT_RED;
			break;
		case MCOLOR_LIGHT_GREEN:
			c = C_LIGHT_GREEN;
			break;
		case MCOLOR_YELLOW:
			c = C_YELLOW;
			break;
		case MCOLOR_LIGHT_BLUE:
			c = C_LIGHT_BLUE;
			break;
		case MCOLOR_LIGHT_MAGENTA:
			c = C_LIGHT_MAGENTA;
			break;
		case MCOLOR_LIGHT_CYAN:
			c = C_LIGHT_CYAN;
			break;
		default:
			MDebug("Unknown color '%c'\n",Color);
			break;
	}
	if (c >= 8)
		wattrset (pWin, A_BOLD | (COLOR_PAIR (c)));
	else
		wattrset (pWin, COLOR_PAIR (c));
	LastColor = Color;

}

void
CursesConWindow::Write(char *szFmt,...)
{
	fUpdated = true;
	int  i, inverse;
	va_list vl;
	static char buf[8192];

	SetColor(MCOLOR_WHITE);

	va_start (vl, szFmt);
	vsprintf (buf, szFmt, vl);
	va_end (vl);

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
}

char
CursesCon::GetCh()
{
	char ch = getch();
	if(ch == ERR)
		return -1;
	switch(ch)
	{
	case '\n':
		ch = '\r';
		break;
	}
	return ch;
}

