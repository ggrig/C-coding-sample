#include "timer.h"

timer::timer(char * threadName): 
	atomicThread(threadName),
	m_hTimer(NULL) 
{
	m_hTimer = CreateEvent(	NULL,     // no security attributes
							FALSE,    // auto-reset event
							FALSE,    // initial state is non-signaled
							"timerEvent" );    // lpName

	if (m_hTimer == NULL) 
	{
		cout << "CreateEvent() failed for " << threadName << endl;
	}
}

timer::~timer(void)
{
	if (NULL != m_hTimer)
	{
		CloseHandle( m_hTimer );
	}
}

void timer::threadLoop(void)
{
	if (NULL == m_hTimer)
	{
		cout << "Timer not properly initialized" << endl;
		return;
	}

	while (m_bContinueProcessing)
	{
		Sleep(TIMER_TICK_PERIOD);
		SetEvent(m_hTimer);
	}
}

