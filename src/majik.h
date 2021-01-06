// Global definations file.
// Keep it down to an absolute minimum here.

// Dont do extern in other files, that makes things unmanagble.
// Think alot before adding anything to this file.
// No OS files should be included in this file only strictly ansi files.

#ifndef _MAJIK_H_
#define _MAJIK_H_

// Type definations.
#include "MTypes.h"

//* OS dependable base classes.
// The "text" library.
#include "MCon.h"
// The "net" library.
#include "MNet.h"
// The "wait" library.
#include "MWait.h"

//* Majik classes, none of these should be OS dependable.
// The Keyboard.
#include "MKeyboard.h"
// The MCP.
#include "MCP.h"
// The Map.
#include "MMap.h"
// Player.
#include "MPlayer.h"

//* Global variables. Keep them down to a minimum.
// Our console. Defined in main.cpp
extern MCon *pCon;
// Out net. Defined in main.cpp
extern MNet *pNet;
// Our wait. Defined in main.cpp
extern MWait *pWait;
// MCP, defined in MCP.cpp
extern Mcp MCP;
// Keyboard, defined in MKeyboard.cpp
extern MKeyboard Keyboard;
// The map.
extern MMap Map;
// The player.
extern MPlayer Player;

#endif
