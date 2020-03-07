#pragma once

#include "atomicThread.h"
#include "intFIFOQueue.h"

// whenever it cathces a time event
// outputs the queue size to the console

class watchDog: public atomicThread
{
public:
	watchDog(char * threadName, HANDLE hTimerEvent, intFIFOQueue& queue);
	virtual ~watchDog(void);
protected:
	HANDLE m_hTimerEvent;
	intFIFOQueue& m_queue;
public:
	void threadLoop(void);
};
