#pragma once

// the following header contains details on how the FIFO works
#include "TMultiThreadSingleQueue.h"

#define USE_QUEUE_EVENTS

class intFIFOQueue: public CTMultiThreadSingleQueue<int>
{
public:
	intFIFOQueue(void);
	virtual ~intFIFOQueue(void);
private:
	virtual void incrementSize();
	virtual void decrementSize();
private:
	bool m_bPushAllowed;
	bool m_bQueueEmpty;
#ifdef USE_QUEUE_EVENTS
private:
	HANDLE m_hPushAllowed;
	HANDLE m_hQueueEmpty;
public:
	HANDLE GetPushAllowed() {return m_hPushAllowed;};
	HANDLE GetQueueEmpty() {return m_hQueueEmpty;};
#else
public:
	bool isPushAllowed() {return m_bPushAllowed;};
	bool isQueueEmpty() {return m_bQueueEmpty;};
#endif
};
