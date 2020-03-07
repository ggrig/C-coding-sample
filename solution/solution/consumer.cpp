#include "helpers.h"
#include "consumer.h"

consumer::consumer(char * threadName, intFIFOQueue& queue, txtFile& theFile):
	atomicThread(threadName),
	m_queue(queue),
	m_file(theFile)
#ifdef _DEBUG
	,m_nNum(0)
#endif
{
}

consumer::~consumer(void)
{
#ifdef _DEBUG
	cout << m_threadName << " got " << m_nNum << " numbers" << endl;
#endif
}

void consumer::threadLoop(void)
{
	while (m_bContinueProcessing)
	{
		// sleep randomly 0..100 milliseconds

		unsigned int random;
		if (!rand_from_range(&random,0,100)) //error
			continue;

		Sleep (random);

		// get a number from the queue 
		int value;
		if (m_queue.Pop(value))
		{
			// write the value to the file
			m_file << value;
#ifdef _DEBUG
			m_nNum++;
#endif
		}
	}
}

