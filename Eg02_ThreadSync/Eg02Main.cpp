//经典线程同步互斥问题
#include "CriticalSectionThreadSync.h"
#include "EventThreadSync.h"
#include "PulseEventThread.h"

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

	system("pause");
	return 0;
}
