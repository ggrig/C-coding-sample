#pragma once

#include "atomicThread.h"
#include "intFIFOQueue.h"

//- Producer: sleeps 0…100 milliseconds randomly;
//            when wakes up generates a random number between 1 and 100 and then puts this number to the queue

class producer: public atomicThread 
{
public:
	producer(char * threadName, intFIFOQueue& queue);
	virtual ~producer(void);
#ifdef _DEBUG
	unsigned int m_nNum;
#endif
private:
	intFIFOQueue& m_queue;
public:
	void threadLoop(void);
};
