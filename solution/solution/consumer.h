#pragma once

#include "atomicThread.h"
#include "intFIFOQueue.h"
#include "txtFile.h"

//- Consumer: sleeps 0…100 milliseconds randomly;
//            when wakes up takes a number from the queue and saves it to the output ‘data.txt’ file

class consumer: public atomicThread
{
public:
	consumer(char * threadName, intFIFOQueue& queue, txtFile& theFile);
	virtual ~consumer(void);
#ifdef _DEBUG
	unsigned int m_nNum;
#endif
private:
	intFIFOQueue& m_queue;
	txtFile& m_file;
public:
	void threadLoop(void);
};
