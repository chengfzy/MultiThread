//经典线程同步互斥问题,使用信号量Semaphore
#pragma once
#include "Constants.h"

//线程函数
unsigned int __stdcall SemaphoreThreadFunc(void *pPM)
{
	int nThreadNum = *(int*)pPM;
	ReleaseSemaphore(g_hThreadSemaphore, 1, NULL);	//信号量+1

	Sleep(50);		//some work should to do

	EnterCriticalSection(&g_csThreadCode);
	++g_nNum;
	Sleep(0);		//some work should to do
	cout << "Thread ID = " << nThreadNum << ", Number = " << g_nNum << endl;
	LeaveCriticalSection(&g_csThreadCode);
	return 0;
}

void TestSemaphore()
{
	//初始化信号量与关键段
	g_hThreadSemaphore = CreateSemaphore(NULL, 0, 1, NULL);	//当前0个资源，最大允许1个同时访问
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	int i(0);
	while (i < THREAD_NUM)
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, SemaphoreThreadFunc, &i, 0, NULL);
		WaitForSingleObject(g_hThreadSemaphore, INFINITE);		//等待信号量>0
		++i;
	}

	//保证子线程已全部运行结束
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	//销毁信号量和关键段
	CloseHandle(g_hThreadSemaphore);
	DeleteCriticalSection(&g_csThreadCode);
	for (i = 0; i < THREAD_NUM; ++i)
		CloseHandle(handle[i]);
}
