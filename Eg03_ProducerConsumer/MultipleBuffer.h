#pragma once
#include <iostream>
#include <process.h>
#include <Windows.h>
#include "Constants.h"
using namespace std;

const int BUFFER_SIZE = 4;				//缓冲区个数
int g_BufferArray[BUFFER_SIZE];		//缓冲池
int g_i, g_j;

//生产者线程函数
unsigned int __stdcall MultiProducerThreadFunc(LPVOID pM)
{
	for (int i = 0; i < END_PRODUCE_NUMBER; ++i)
	{
		//等待有空的缓冲区出现
		WaitForSingleObject(g_hSemaphoreBufferEmpty, INFINITE);

		//互斥的访问缓冲区
		EnterCriticalSection(&g_cs);
		g_BufferArray[g_i] = i;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Producer Put Data into Buffer: Data[" << g_i << "] = " << g_BufferArray[g_i] << endl;		
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		g_i = (g_i + 1) % BUFFER_SIZE;
		LeaveCriticalSection(&g_cs);

		//通知消费者有新数据了
		ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);		
	}

	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "Producer Thread END" << endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return 0;
}


//消费者线程函数
unsigned int __stdcall MultiConsumerThreadFunc(PVOID pM)
{
	while (true)
	{
		//等待非空的缓冲区出现
		WaitForSingleObject(g_hSemaphoreBufferFull, INFINITE);

		//互斥的访问缓冲区
		EnterCriticalSection(&g_cs);
		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "Consumer Get Data from Buffer: ThreadId = " << GetCurrentThreadId() << " Data[" << g_j << "] = " << g_BufferArray[g_j] << endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		if (g_BufferArray[g_j] == END_PRODUCE_NUMBER - 1)
		{
			LeaveCriticalSection(&g_cs);
			//通知其他消费者有新数据了(结束标志)
			ReleaseSemaphore(g_hSemaphoreBufferFull, 1, NULL);
			break;
		}
		g_j = (g_j + 1) % BUFFER_SIZE;
		LeaveCriticalSection(&g_cs);

		Sleep(50);		//some other work should to do
		ReleaseSemaphore(g_hSemaphoreBufferEmpty, 1, NULL);
	}

	SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "Consumer Thread End, ThreadId = " << GetCurrentThreadId() << endl;
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	return 0;
}


//1生产者，2消费者，4缓冲区
void MultiBufferTest()
{
	cout << "======== Producer - Consumer Problem: 1 Producer, 2 Consumer, 4 Buffer ========" << endl;

	InitializeCriticalSection(&g_cs);
	//初始化信号量，一个记录有产品的缓冲区个数，另一个记录空缓冲区个数
	g_hSemaphoreBufferEmpty = CreateSemaphore(NULL, 4, 4, NULL);
	g_hSemaphoreBufferFull = CreateSemaphore(NULL, 0, 4, NULL);
	
	g_i = 0;
	g_j = 0;
	memset(g_BufferArray, 0, sizeof(g_BufferArray));

	const int THREAD_NUM = 3;
	HANDLE hThread[THREAD_NUM];
	//Producer Thread
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, MultiProducerThreadFunc, NULL, 0, NULL);
	//Consumer Thread
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, MultiConsumerThreadFunc, NULL, 0, NULL);
	hThread[2] = (HANDLE)_beginthreadex(NULL, 0, MultiConsumerThreadFunc, NULL, 0, NULL);
	WaitForMultipleObjects(THREAD_NUM, hThread, TRUE, INFINITE);
	for (int i = 0; i < THREAD_NUM; ++i)
		CloseHandle(hThread[i]);

	//销毁事件和关键段
	CloseHandle(g_hSemaphoreBufferEmpty);
	CloseHandle(g_hSemaphoreBufferFull);
	DeleteCriticalSection(&g_cs);
}
