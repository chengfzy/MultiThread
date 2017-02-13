//经典线程同步互斥问题
#include "CriticalSectionThreadSync.h"
#include "EventThreadSync.h"
#include "PulseEventThread.h"
#include "MutexThreadSync.h"
#include "SemaphoreThreadSync.h"

int main()
{
	//1.使用Critial Section
	cout << "=========================== Critial Section ===========================" << endl;
	TestCritialSection();

	//2.使用Event
	cout << "\n=========================== Event ===========================" << endl;
	TestEvent();

	//3.使用PulseEvent
	cout << "\n=========================== Pulse Event ===========================" << endl;
	TestPulseEvent();

	//4.使用Mutex
	cout << "\n=========================== Mutex ===========================" << endl;
	TestMutex();
	
	//5.使用Semaphore
	cout << "\n=========================== Semaphore ===========================" << endl;
	TestSemaphore();

	system("pause");
	return 0;
}
