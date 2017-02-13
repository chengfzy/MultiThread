//经典线程同步互斥问题,使用Event
#pragma once
#include "Constants.h"

//线程函数
unsigned int __stdcall EventThreadFunc(void* pPM)
{
	int nThreadNum = *(int*)pPM;
	SetEvent(g_hThreadEvent);		//触发事件

	Sleep(50);		//some work should to do

	EnterCriticalSection(&g_csThreadCode);
	++g_nNum;
	Sleep(0);		//some work should to do
	cout << "Thread ID = " << nThreadNum << ", Number = " << g_nNum << endl;
	LeaveCriticalSection(&g_csThreadCode);
	return 0;
}


void TestEvent()
{
	//Initialize Event and Critial Section
	g_hThreadEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];

	int i(0);
	while (i < THREAD_NUM)
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, EventThreadFunc, &i, 0, NULL);
		WaitForSingleObject(g_hThreadEvent, INFINITE);		//等待事件触发
		++i;
	}
	//保证子线程已全部运行结束
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	//销毁事件与关键段
	CloseHandle(g_hThreadEvent);
	DeleteCriticalSection(&g_csThreadCode);
}
