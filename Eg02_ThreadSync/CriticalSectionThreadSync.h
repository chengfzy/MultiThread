//经典线程同步互斥问题,使用Critial Section
#pragma once
#include "Constants.h"

//线程函数
unsigned int __stdcall CSThreadFunc(void* pPM)
{
	int nThreadNum = *(int*)pPM;
	LeaveCriticalSection(&g_csThreadParameter);

	Sleep(50);		//some work should to do
	++g_nNum;
	Sleep(0);		//some work should to do
	cout << "Thread ID = " << nThreadNum << ", Number = " << g_nNum << endl;
	LeaveCriticalSection(&g_csThreadCode);
	return 0;
}


void TestCritialSection()
{
	//Initialize Critial Section
	InitializeCriticalSection(&g_csThreadParameter);
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];

	int i(0);
	while (i < THREAD_NUM)
	{
		EnterCriticalSection(&g_csThreadParameter);	//enter critical section
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, CSThreadFunc, &i, 0, NULL);
		++i;
	}
	//保证子线程已全部运行结束
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	//Destory Critical Section
	DeleteCriticalSection(&g_csThreadParameter);
	DeleteCriticalSection(&g_csThreadCode);
}
