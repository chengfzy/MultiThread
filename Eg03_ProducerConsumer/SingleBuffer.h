#pragma once
#include <iostream>
#include <process.h>
#include <Windows.h>
#include "Constants.h"
using namespace std;

int g_Buffer;							//缓冲区

//生产者线程函数
unsigned int __stdcall SingleProducerThreadFunc(PVOID pM)
{
	for (int i = 0; i < END_PRODUCE_NUMBER; ++i)
	{
		//等待缓冲区为空
		WaitForSingleObject(g_hEventBufferEmpty, INFINITE);

		//互斥的访问缓冲区
		EnterCriticalSection(&g_cs);
		g_Buffer = i;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "Producer Put Data into Buffer: Data = " << i << endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		LeaveCriticalSection(&g_cs);

		//通知缓冲区有新数据了
		SetEvent(g_hEventBufferFull);
	}
	return 0;
}

//消费者线程函数
unsigned int __stdcall SingleConsumerThreadFunc(PVOID pM)
{
	volatile bool flag = true;
	while (flag)
	{
		//等待缓冲区有数据
		WaitForSingleObject(g_hEventBufferFull, INFINITE);

		//互斥的访问缓冲区
		EnterCriticalSection(&g_cs);
		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "Consumer Get Data from Buffer: Data = " << g_Buffer << endl;
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		if (g_Buffer == END_PRODUCE_NUMBER - 1)
			flag = false;
		LeaveCriticalSection(&g_cs);

		//通知缓冲区已空
		SetEvent(g_hEventBufferEmpty);

		Sleep(10);		//some other work should to do
	}
	return 0;
}

//1生产者，1消费者，1缓冲区
void SingleBufferTest()
{
	cout << "======== Producer - Consumer Problem: 1 Producer, 1 Consumer, 1 Buffer ========" << endl;

	InitializeCriticalSection(&g_cs);
	//创建二个自动复位事件，一个表示缓冲区是否为空，一个表示缓冲区是否已经处理
	g_hEventBufferEmpty = CreateEvent(NULL, FALSE, TRUE, NULL);
	g_hEventBufferFull = CreateEvent(NULL, FALSE, FALSE, NULL);

	const int THREAD_NUM = 2;
	HANDLE hThread[THREAD_NUM];

	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, SingleProducerThreadFunc, NULL, 0, NULL);
	hThread[1] = (HANDLE)_beginthreadex(NULL, 0, SingleConsumerThreadFunc, NULL, 0, NULL);
	WaitForMultipleObjects(THREAD_NUM, hThread, TRUE, INFINITE);
	CloseHandle(hThread[0]);
	CloseHandle(hThread[1]);

	//销毁事件和关键段
	CloseHandle(g_hEventBufferEmpty);
	CloseHandle(g_hEventBufferFull);
	DeleteCriticalSection(&g_cs);
}
