#pragma once

#include "atomicThread.h"

#define TIMER_TICK_PERIOD			1000

// timer class for accurate time measuring

class timer: public atomicThread
{
public:
	timer(char * threadName);
	virtual ~timer(void);
protected:
	HANDLE m_hTimer;
public:
	virtual void threadLoop(void);
	HANDLE getTimer() {return m_hTimer;};
};
