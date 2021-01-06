#ifndef _MWAIT_H_
#define _MWAIT_H_
#include <stdio.h>

class MWait  
{
public:
	MWait();
	virtual ~MWait();
	virtual void Wait()=NULL;	// Wait for either data over the network or input from the keyboard.
};

#endif
