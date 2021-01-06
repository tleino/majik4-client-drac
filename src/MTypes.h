// MTypes (Majik types) contains general types used in Majik.

#ifndef _MTYPES_H_
#define _MTYPES_H_

// Return type.
typedef enum {
	RETURN_YES=0,
	RETURN_OK=0,
	RETURN_NO=1,
	RETURN_ERROR=1,
} Return_t;

// Coordinate type.
// TODO: Consider if this should be floats instead.
class MCoord
{
public:
	MCoord() {};
	MCoord(const int x,const int y) { this->x = x; this->y = y; };
	MCoord(const MCoord &From) { x = From.x; y = From.y; };
	operator ==(MCoord &A) { return (A.x == x) && (A.y == y); };
public:
	int x,y;
};


// Color type.
typedef enum {
	MCOLOR_NOCHANGE = 0,
	MCOLOR_BLACK = 'l',
	MCOLOR_RED = 'r',
	MCOLOR_GREEN = 'g',
	MCOLOR_BROWN = 'y',
	MCOLOR_BLUE = 'b',
	MCOLOR_MAGENTA = 'p',
	MCOLOR_CYAN = 'c',
	MCOLOR_LIGHT_GRAY = 'w',
	MCOLOR_DARK_GRAY = 'L',
	MCOLOR_LIGHT_RED = 'R',
	MCOLOR_LIGHT_GREEN = 'G',
	MCOLOR_YELLOW = 'Y',
	MCOLOR_LIGHT_BLUE = 'B',
	MCOLOR_LIGHT_MAGENTA = 'P',
	MCOLOR_LIGHT_CYAN = 'C',
	MCOLOR_WHITE = 'W',
} MColor_t;

#endif
