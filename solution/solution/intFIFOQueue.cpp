#include "intFIFOQueue.h"

intFIFOQueue::intFIFOQueue(void):
	CTMultiThreadSingleQueue()
	,m_bPushAllowed(true)
	,m_bQueueEmpty(true)
{
#ifdef USE_QUEUE_EVENTS
	m_hPushAllowed = CreateEvent(	NULL,    // no security attributes
									TRUE,    // manual-reset event
									TRUE,    // initial state is signaled
									"pushAllowed" );    // lpName

	if (m_hPushAllowed == NULL) 
	{
		cout << "CreateEvent() failed for m_hPushAllowed " << endl;
	}

	m_hQueueEmpty = CreateEvent(	NULL,    // no security attributes
									TRUE,    // manual-reset event
									TRUE,    // initial state is not-signaled
									"queueEmpty" );    // lpName

	if (m_hQueueEmpty == NULL) 
	{
		cout << "CreateEvent() failed for m_hQueueEmpty " << endl;
	}

#endif
}

intFIFOQueue::~intFIFOQueue(void)
{
#ifdef USE_QUEUE_EVENTS
	if (NULL != m_hPushAllowed)
	{
		CloseHandle( m_hPushAllowed );
	}
	if (NULL != m_hQueueEmpty)
	{
		CloseHandle( m_hQueueEmpty );
	}
#endif
}

// incremetnSize calls are synchronized among producers
// decremetnSize calls are synchronized among consumers

// Further synchronization of m_bQueueEmpty and m_bPushAllowed 
// value changes will wipe out the advantages of the 
// "Standard-Multi-threaded-Dynamic-Queue"

// Foe more details see:
// http://www.codeproject.com/Articles/115859/A-Standard-Multi-threaded-Dynamic-Queue

void intFIFOQueue::incrementSize() 
{
	::InterlockedIncrement(&m_Size);

	if (m_bQueueEmpty && m_Size >= 0)
	{
		m_bQueueEmpty = false;
#ifdef USE_QUEUE_EVENTS
		ResetEvent (m_hQueueEmpty);
#endif
	}

	if (m_bPushAllowed && m_Size >= QUEUE_MAX_SIZE)
	{
		m_bPushAllowed = false;
#ifdef USE_QUEUE_EVENTS
		ResetEvent (m_hPushAllowed);
#endif
	}
}

void intFIFOQueue::decrementSize() 
{
	::InterlockedDecrement(&m_Size);

	if (!m_bQueueEmpty && m_Size == 0)
	{
		m_bQueueEmpty = true;
#ifdef USE_QUEUE_EVENTS
		SetEvent (m_hQueueEmpty);
#endif
	}

	if (!m_bPushAllowed && m_Size <= QUEUE_PUSH_ALLOWED_SIZE)
	{
		m_bPushAllowed = true;
#ifdef USE_QUEUE_EVENTS
		SetEvent (m_hPushAllowed);
#endif
	}
}
