#ifndef _MKEYBOARD_H_
#define _MKEYBOARD_H_

#include "MCon.h"

typedef enum {
	MKEYBOARD_STATE_DISABLED,
	MKEYBOARD_STATE_READLINE,
	MKEYBOARD_STATE_READCHAR,
} MKeyboardState_t;

class MKeyboard  
{
public:
	MKeyboard();
	virtual ~MKeyboard();
	void SetDefaultState(MKeyboardState_t nState) { nDefaultState = nState; };
	void SetWindow(MConWindow *pWin) { this->pWin = pWin; };
	void ReciveChar(char Char);
	void ReadAndSendLine(int nCommand,char *szPrompt,bool bEcho=true);
	void ReadAndSendChar(int nCommand) { this->nCommand = nCommand; nState = MKEYBOARD_STATE_READCHAR; };

public:
	MKeyboardState_t	nDefaultState;
	MKeyboardState_t	nState;
	MConWindow *pWin;	// Window to "echo" to.

	// Line edit mode.
	bool	bEcho;				// Should we echo to the screen?
	char	*szPrompt;			// The prompt on the screen.
	int		nCommand;			// The command to send when a line has been recived.
	char	szLine[1024*10];	// The line recived so far.
};

#endif
