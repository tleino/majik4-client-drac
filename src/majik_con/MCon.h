// MCon (Majic Console)
//
// This is the class that describes the console we are connected to.
//

#ifndef _MCON_H_
#define _MCON_H_
#include <stdio.h>
#include "MTypes.h"

class MCon;

class MConWindow
{
public:
	// Construct and destruct.
	MConWindow(MCon *pCon,char *szName) { this->pCon = pCon; pNext = NULL; this->szName = szName; };
	virtual ~MConWindow() { if(pNext != NULL) delete pNext; };

public:
	// Char functions.
	virtual void Move(MCoord &Pos) = NULL;									// Move the cursor to this position.
	virtual void PutCh(char szChar,MColor_t Color = MCOLOR_NOCHANGE) = NULL;// Put a character on the screen.
	virtual void Write(char *szFmt,...) = NULL;								// Write a string to the screen.
	virtual void WriteWord(char *szFmt,...);								// Write a string to the screen, but fit the words to the window size.
	virtual void DelCh(int nToDelete=1) = NULL;								// Delete the characters under the cursor, and move the rest of the line to the left.
	virtual void DelLine(int nToDelete=1) = NULL;							// Delete the current lines, move the lines under up.
	virtual void Scroll(MCoord &Dir) = NULL;								// Scroll the window so that 0,0 will end up at {Dir}.
	virtual void Clear() = NULL;											// Clear the window.
	virtual void Update() = NULL;											// Flush the window.
	virtual void Box(MCoord &Pos,MCoord &Size);								// Draw a box in the window.
	virtual MCoord GetSize() = NULL;										// Returns the size of the window.
	virtual int GetWidth() { return GetSize().x; };							// Returns the width of the window.
	virtual int GetHeight() { return GetSize().y; };						// Returns the height of the window/


public:
	// Variables.
	MConWindow *pNext;	// Linked list of the windows.
	MCon *pCon;			// Pointer to the console.
	char *szName;		// Name of this window.
};


class MCon  
{
public:
	// Construct and destruct.
	MCon() { pFirstWindow = NULL; };
	virtual ~MCon() { if(pFirstWindow != NULL) delete pFirstWindow; };
public:
	// Information functions.
	virtual MCoord GetSize() = NULL;
	virtual int GetWidth() { return GetSize().x; };
	virtual int GetHeight() { return GetSize().y; };
	virtual  char GetCh() = NULL; // Return a char from the keyboard, -1 if none is avaible.
								  // Look at the enum below for special chars.

	// Window functions.
	virtual MConWindow *NewWindow(MCoord &Pos,MCoord &Size,char *szName) = NULL;
	virtual MConWindow *GetWindow(char *szName);

	// Modify functions.
	virtual void Clear() = NULL;	// Clear the console, and all windows contained in it.
	virtual void Update() = NULL;	// Update the console and all windows on it.

protected:
	// Window managment functions.
	virtual void AddWindow(MConWindow *pToAdd);
	virtual void DelWindow(MConWindow *pToDel);

protected:
	// Variables.
	MConWindow *pFirstWindow;
};

enum SpecialChars {
	MCHAR_ULCORNER = -128,	/* upper left corner */
	MCHAR_LLCORNER,			/* lower left corner */
	MCHAR_URCORNER,			/* upper right corner */
	MCHAR_LRCORNER,			/* lower right corner */
	MCHAR_LTEE,				/* tee pointing right */
	MCHAR_RTEE,				/* tee pointing left */
	MCHAR_BTEE,				/* tee pointing up */
	MCHAR_TTEE,				/* tee pointing down */
	MCHAR_HLINE,			/* horizontal line */
	MCHAR_VLINE,			/* vertical line */
	MCHAR_PLUS,				/* large plus or crossover */
};


// I take for granted that people have arrow keys
// and F1 to F10 on their keyboard. These must be be supported by all ports.
// The rest is optional.
enum SpecialKeys {
	MKEY_UP = -128,
	MKEY_DOWN,
	MKEY_LEFT,
	MKEY_RIGHT,
	MKEY_INSERT,
	MKEY_DELETE,
	MKEY_HOME,
	MKEY_END,
	MKEY_PAGE_UP,
	MKEY_PAGE_DOWN,
	MKEY_F0,
	MKEY_F1,
	MKEY_F2,
	MKEY_F3,
	MKEY_F4,
	MKEY_F5,
	MKEY_F6,
	MKEY_F7,
	MKEY_F8,
	MKEY_F9,
	MKEY_F10,
	MKEY_F11,
	MKEY_F12,
	MKEY_F13,
	MKEY_F14,
	MKEY_F15,
	MKEY_F16,
	MKEY_F17,
	MKEY_F18,
	MKEY_F19,
	MKEY_F20,
	MKEY_A0,
	MKEY_A1,
	MKEY_A2,
	MKEY_A3,
	MKEY_A4,
	MKEY_A5,
	MKEY_A6,
	MKEY_A7,
	MKEY_A8,
	MKEY_A9,
	MKEY_A10,
	MKEY_A11,
	MKEY_A12,
	MKEY_A13,
	MKEY_A14,
	MKEY_A15,
	MKEY_A16,
	MKEY_A17,
	MKEY_A18,
	MKEY_A19,
	MKEY_A20,
};

#endif
