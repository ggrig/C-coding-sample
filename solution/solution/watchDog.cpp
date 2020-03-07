#include "helpers.h"
#include "watchDog.h"

watchDog::watchDog(char * threadName, HANDLE hTimerEvent, intFIFOQueue& queue):
	atomicThread(threadName),
	m_hTimerEvent(hTimerEvent),
	m_queue(queue)
{
}

watchDog::~watchDog(void)
{
}

void watchDog::threadLoop(void)
{
	if (NULL == m_hTimerEvent)
	{
		cout <<  "Timer event is not properly initialized." << endl;
		return;
	}

	int seconds = 0;
	COORD startPos = getCursorPos();

	while (m_bContinueProcessing)
	{
        DWORD   dwWaitResult = WaitForSingleObject( m_hTimerEvent, 2000 );

        if ( dwWaitResult == WAIT_TIMEOUT )   // WAIT_TIMEOUT = 258
        {
			cout << "WatchDog timed out while waiting for time event" << endl;
            break; 
        }
        else if ( dwWaitResult == WAIT_OBJECT_0 )  // WAIT_OBJECT_0 = 0
        {
			long currentSize = m_queue.GetSize();
			seconds++;

			//This an auto-reset event, so no need for the following:
			//ResetEvent(m_hTimerEvent);

			// we do not synchronize queue size reads and writes 
			// instead just do a sanity check
			// if it passes we output the value
			if (currentSize >= 0 && currentSize <= 10 * QUEUE_MAX_SIZE)
			{
				COORD currentPos = getCursorPos();
				SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), startPos );
				cout << endl;
				cout << "Seconds passed: " << setw ( 10 ) << seconds << endl;
				cout << "Queue size is:  " << setw ( 10 ) << currentSize << endl;
				cout << endl;
				if (seconds > 1)
				{
					SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), currentPos );
				}
			}
        } 
	}
}
