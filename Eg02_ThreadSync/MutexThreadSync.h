//经典线程同步互斥问题,使用互斥量Mutex
#pragma once
#include "Constants.h"
#include <conio.h>

//线程函数
unsigned int __stdcall MutexThreadFunc(void *pPM)
{
	int nThreadNum = *(int*)pPM;
	ReleaseMutex(g_hThreadMutex);	//触发互斥量

	Sleep(50);		//some work should to do

	EnterCriticalSection(&g_csThreadCode);
	++g_nNum;
	Sleep(0);		//some work should to do
	cout << "Thread ID = " << nThreadNum << ", Number = " << g_nNum << endl;
	LeaveCriticalSection(&g_csThreadCode);
	return 0;
}

void TestMutex()
{
	//初始化互斥量与关键段,第二个参数为True表示互斥量为创建线程所有
	g_hThreadMutex = CreateMutex(NULL, FALSE, NULL);
	InitializeCriticalSection(&g_csThreadCode);

	g_nNum = 0;
	HANDLE handle[THREAD_NUM];
	int i(0);
	while (i < THREAD_NUM)
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, MutexThreadFunc, &i, 0, NULL);
		WaitForSingleObject(g_hThreadMutex, INFINITE);		//等待互斥量被触发
		++i;
	}

	//保证子线程已全部运行结束
	WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);

	//销毁互斥量和关键段
	CloseHandle(g_hThreadMutex);
	DeleteCriticalSection(&g_csThreadCode);
	for (i = 0; i < THREAD_NUM; ++i)
		CloseHandle(handle[i]);
}


//以下两段Code必须为两个不同的exe，才能做测试
const char MUTEX_NAME[] = "Mutex_MoreWindows";

//互斥量测试:产生abandoned mutex
void CreateAbandonedMutext()
{
	HANDLE hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);	//创建互斥量
	cout << "Create Mutex, Press Any Key to Start" << endl;
	getch();
	//exit(0);
	ReleaseMutex(hMutex);
	cout << "Release Mutex" << endl;
	CloseHandle(hMutex);
}

//接收abandoned mutex
void WaitAbandonedMutex()
{
	HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, TRUE, MUTEX_NAME);
	if (hMutex == NULL)
	{
		cout << "Open Mutex Failed" << endl;
		return;
	}
	cout << "Waiting..." << endl;

	DWORD dwResult = WaitForSingleObject(hMutex, 5 * 1000);	//等待互斥量被触发
	switch (dwResult)
	{
		case WAIT_ABANDONED:
			cout << "Wait Abandoned, 拥有互斥量的进程意外终止" << endl;
			break;
		case WAIT_OBJECT_0:
			cout << "Accept Mutex, 已经收到信号" << endl;
			break;
		case WAIT_TIMEOUT:
			cout << "Timeout: 信号未在规定的时间内送到" << endl;
			break;
	}
	CloseHandle(hMutex);
}
