#pragma once

#include "common.h"

class atomicThread
{
public:
	atomicThread(char * threadName = NULL);
	virtual ~atomicThread(void);
protected:
	bool m_bContinueProcessing;
    HANDLE   m_hThreadHandle;
    unsigned  m_uiThreadID;
	string m_threadName;

	void die() {m_bContinueProcessing = false;};

public:
	bool createThread(unsigned (__stdcall * _StartAddress) (void *));
	DWORD resumeThread(void);
	bool stopThread(void);

	virtual void threadLoop() = 0;
};

// pInstance must point to an object of subclass of atomicThread

template <class T>
unsigned __stdcall startThreadLoop(void * pInstance)
{
    T* pTheInstance = (T*)pInstance;
    pTheInstance->threadLoop();

    // You can call _endthreadex() explicitly to terminate a thread;
    // however, _endthreadex() is called automatically when the thread
    // returns from the routine passed as a parameter to _beginthreadex().

    // By calling _endthreadex() we cause our thread object to go
    // to the signaled state, which releases anyone waiting on us.

    _endthreadex( 0 );    // the thread exit code

    // _endthreadex() does not close the thread handle so somebody
    // must still call CloseHandle().

    // We never get to here because of the earlier call to _endthreadex().

    return 0;  // the thread exit code
}

