// solution.cpp : Defines the entry point for the console application.
//

#include "common.h"
#include "timer.h"
#include "watchDog.h"
#include "txtFile.h"
#include "producer.h"
#include "consumer.h"

#define MAX_PRODUCERS 10
#define MAX_CONSUMERS 10 

bool checkArgs (int argc, char* argv[], int &numProducers, int &numConsumers)
{
	bool ret = true;

	int producers;
	int consumers;

	numProducers = 0;
	numConsumers = 0;

	if (3 != argc)
	{
		ret = false;
	}

	if (ret)
	{
		producers = atoi(argv[1]);
		consumers = atoi(argv[2]);

		if (producers < 1 || producers > MAX_PRODUCERS)
		{
			ret = false;
		}

		if (consumers < 1 || consumers > MAX_CONSUMERS)
		{
			ret = false;
		}
	}

	if (ret)
	{
		numProducers = producers;
		numConsumers = consumers;
	}
	else
	{
		cout << "Allowed command line format is:" << endl;
		cout << endl << "      solution numProducers numConsumers" << endl << endl;
		cout << "where numProducers is an integer in range from 1 to " <<  MAX_PRODUCERS << endl;
		cout << "and numConsumers is an integer in range from 1 to " << MAX_CONSUMERS << endl;
	}

	return ret;
}

int main(int argc, char* argv[])
{
	int numProducers = 0;
	int numConsumers = 0;

	if (!checkArgs(argc, argv, numProducers, numConsumers))
	{
		return 1;
	}

	cout << "Starting..." << endl << endl;

// Initialize the threads

	txtFile dataTxt("data.txt");
	intFIFOQueue theQueue;

	timer theTimer("timer");
	watchDog theWatchDog("watchDog",theTimer.getTimer(),theQueue);

// Create the treads

	if (!theTimer.createThread(startThreadLoop<timer>) ||
		!theWatchDog.createThread(startThreadLoop<watchDog>))
	{
		cout << "Failed to create the threads" << endl;
		return 1;
	}

	char name[20];
	
	producer ** pr_array = new producer*[numProducers];
	for (int idx = 0; idx < numProducers; idx++)
	{
		sprintf_s(name,"producer_%d",idx + 1);
		pr_array[idx] = new producer(name,theQueue);
		pr_array[idx]->createThread(startThreadLoop<producer>);
	}

	consumer ** cn_array = new consumer*[numConsumers];
	for (int idx = 0; idx < numConsumers; idx++)
	{
		sprintf_s(name,"consumer_%d",idx + 1);
		cn_array[idx] = new consumer(name,theQueue,dataTxt);
		cn_array[idx]->createThread(startThreadLoop<consumer>);
	}

	cout << "To stop the application press \'S\'" << endl ; 

// Start the treads
	theTimer.resumeThread();
	theWatchDog.resumeThread();

	for (int idx = 0; idx < numProducers; idx++)
	{
		pr_array[idx]->resumeThread();
	}

	for (int idx = 0; idx < numConsumers; idx++)
	{
		cn_array[idx]->resumeThread();
	}

// wait for STOP
	while (true)
	{
		char ch = _getch();
		if ('S' == ch || 's' == ch)
		{
			cout << "Stopping..." << endl;
			break;
		}
	}

// stop the producers
	cout << "Stopping producers..." << endl;

#ifdef _DEBUG
	int nPushedNums = 0;
	for (int idx = 0; idx < numProducers; idx++)
	{
		pr_array[idx]->stopThread();
		nPushedNums += pr_array[idx]->m_nNum;
	}
	cout << "pushed " <<  nPushedNums << endl;
#endif

	for (int idx = 0; idx < numProducers; idx++)
	{
		delete pr_array[idx];
	}

	delete[] pr_array;

// wait till queue is empty
	cout << "Waiting for queue to empty..." << endl;

#ifdef USE_QUEUE_EVENTS
	HANDLE hQueueEmpty = theQueue.GetQueueEmpty();
	WaitForSingleObject(hQueueEmpty,INFINITE);
#else
	while (!theQueue.isQueueEmpty())
	{
		Sleep(10);
	}
#endif

// stop the consumrs
	cout << "Stopping consumers..." << endl;

#ifdef _DEBUG
	int nPoppedNums = 0;
	for (int idx = 0; idx < numConsumers; idx++)
	{
		cn_array[idx]->stopThread();
		nPoppedNums += cn_array[idx]->m_nNum;
	}
	cout << "popped " <<  nPoppedNums << endl;
#endif

	for (int idx = 0; idx < numConsumers; idx++)
	{
		delete cn_array[idx];
	}
	delete[] cn_array;

// stop the helper threads
	cout << "Stopping the timer and the watchdog..." << endl;

#ifdef _DEBUG
	cout << "Press <Enter> to finish" << endl;
	_getch();
#endif
	return 0;
}

