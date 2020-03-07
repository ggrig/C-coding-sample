#include "atomicThread.h"

atomicThread::atomicThread(char * threadName): 
	m_bContinueProcessing(true),
	m_hThreadHandle(NULL),
	m_uiThreadID(NULL),
	m_threadName(threadName)
{
}

atomicThread::~atomicThread(void)
{
	stopThread();
#ifdef _DEBUG
	cout << m_threadName << " stopped" << endl;
#endif
}

bool atomicThread::createThread(unsigned (__stdcall * _StartAddress) (void *))
{
	bool ret = true;

	// _beginthreadex seems to be safer than CreateThread
	// A discussion on "CreateThread vs _beginthreadex" may be found at
	// http://social.msdn.microsoft.com/forums/en-US/vclanguage/thread/c727ae29-5a7a-42b6-ad0b-f6b21c1180b2

    // When developing a multithreaded WIN32-based application with
    // Visual C++, you need to use the CRT thread functions to create
    // any threads that call CRT functions. Hence to create and terminate
    // threads, use _beginthreadex() and _endthreadex() instead of
    // the Win32 APIs CreateThread() and EndThread().

    // Unlike the thread handle returned by _beginthread(), the thread handle
    // returned by _beginthreadex() can be used with the synchronization APIs.

    m_hThreadHandle = (HANDLE)_beginthreadex( NULL,
                                      0,
                                      _StartAddress,
                                      this,
                                      CREATE_SUSPENDED, // so we can later call ResumeThread()
                                      &m_uiThreadID );

    if ( NULL == m_hThreadHandle )
	{
		ret = false;
	}

	if (ret)
	{
		DWORD dwExitCode;
		GetExitCodeThread( m_hThreadHandle, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
		ret = (STILL_ACTIVE == dwExitCode);
	}

	if (!ret)
	{
		cout << "Failed to create a " << m_threadName << " thread" << endl;
	}

	return ret;
}

bool atomicThread::stopThread(void)
{
	if (NULL != m_hThreadHandle)
	{
		die();
		WaitForSingleObject(m_hThreadHandle, INFINITE);
		m_hThreadHandle = NULL;
		m_uiThreadID = NULL;
	}

	return true;
}

DWORD atomicThread::resumeThread(void)
{
	if (NULL != m_hThreadHandle)
	{
		return ::ResumeThread(m_hThreadHandle);
	}

	return 0xFFFFFFFF ; // ResumeThread error code
}
