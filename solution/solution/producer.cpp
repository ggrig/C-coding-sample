#include "helpers.h"
#include "producer.h"

producer::producer(char * threadName, intFIFOQueue& queue):
	atomicThread(threadName),
	m_queue(queue)
#ifdef _DEBUG
	,m_nNum(0)
#endif
{
}

producer::~producer(void)
{
#ifdef _DEBUG
	cout << m_threadName << " pushed " << m_nNum << " numbers" << endl; 
#endif
}

void producer::threadLoop(void)
{
#ifdef USE_QUEUE_EVENTS
	HANDLE hPushAllowed = m_queue.GetPushAllowed();

	if (NULL == hPushAllowed)
	{
		cout << "PushAllowed has not been properly initialized" << endl;
		return;
	}
#endif

	while (m_bContinueProcessing)
	{
		// sleep randomly 0..100 milliseconds

		unsigned int random;
		if (!rand_from_range(&random,0,100)) //error
			continue;

		Sleep (random);

// blocking before the Sleep allows adding numbers to a full queue
#ifdef USE_QUEUE_EVENTS
		WaitForSingleObject(hPushAllowed,INFINITE);
#else
		if (m_queue.isPushAllowed())
#endif
		{
			// put a random 1..100 integer into the queue

			if (!rand_from_range(&random,1,100)) //error
				continue;

			m_queue.Push(random);

#ifdef _DEBUG
			m_nNum++;
#endif
		}
	}
}
