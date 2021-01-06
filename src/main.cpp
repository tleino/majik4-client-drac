#include "MTypes.h"
#include "MCon.h"
#include "MCP.h"
#include "majik.h"
#include "MNet.h"
#include "MWait.h"
#include "MError.h"
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
#include "win32con.h"
#include "win32net.h"
#include "win32wait.h"
#elif defined(CURSES)
#include "CursesCon.h"
#include "UnixNet.h"
#include "CursesWait.h"
#else
#error "Unknown Os type"
#endif

MCon *pCon = NULL;
MNet *pNet = NULL;
MWait *pWait = NULL;

void
OpenWindows()
{
	MCoord Size = pCon->GetSize();

	Keyboard.SetWindow(pCon->NewWindow(MCoord(0,Size.y-1),MCoord(Size.x,1),"prompt"));

	pCon->NewWindow(MCoord(0,11),MCoord(Size.x,Size.y-(10+1+3+1)),"message");

	pCon->NewWindow(MCoord(0,0),MCoord(15,10),"map");

	pCon->NewWindow(MCoord(0,Size.y-4),MCoord(Size.x,3),"info");

	pCon->NewWindow(MCoord(16,0),MCoord(Size.x-16,10),"description");

	Keyboard.pWin->Write("Foo?\n");

}
int
main(int argc,char *argv[])
{
     if(argc != 2)
     {
	printf("Usage: %s <host>\n",argv[0]);
	exit(1);
     }


#if defined(WIN32)
	pCon = new Win32Con();
#elif defined(CURSES)
	pCon = new CursesCon();
#else
#error "Unknown Console type"
#endif

	printf("%dx%d\n",pCon->GetWidth(),pCon->GetHeight());

	OpenWindows();

	MConWindow *pWin = pCon->GetWindow("message");

#if defined(WIN32)
	pNet = new Win32Net(argv[1],7680);
	pWait = new Win32Wait((Win32Con *)pCon,(Win32Net *)pNet);
#elif defined(CURSES)
	pNet = new UnixNet(argv[1],7680);
	pWait = new CursesWait((UnixNet *)pNet);
#else
#error "Unknown Net type"
#endif

	while(1)
	{
		pWait->Wait();
		
		char ch;
		while((ch = pCon->GetCh()) != -1)
		{
			Keyboard.ReciveChar(ch);
		}

		char *szNow;
		while((szNow = pNet->ReadLn()) != NULL)
		{
			char *szData;
			int nCommand;
			int nDataLen;
			if (sscanf(szNow, "%d:%d:%*s", &nCommand, &nDataLen) != 2)
			{                                                    /* Invalid command */
				free(szNow);
				continue;
			}

			szData = strchr(strchr(szNow,':')+1,':');
			
			if(szData != NULL)
				szData++;

			if(*szData == 0)
			{
				szData = NULL;
			}

			MCP.Command(nCommand,nDataLen,szData);

			free(szNow);
		}

		Player.Display(pCon->GetWindow("info"));

		Map.Display(pCon->GetWindow("map"),Player.GetPos());

		pCon->Update();
		
	}
	delete pCon;
	delete pNet;
	delete pWait;

	return 0;
}
