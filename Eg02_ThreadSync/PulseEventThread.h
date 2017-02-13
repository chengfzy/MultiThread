//使用Pulse Event函数
// PulseEvent: 将事件触发后立即将事件设置为未触发，相当于触发一个事件脉冲
//	对于手动置位事件，所有正处于等待状态下线程都变为可调度状态
//	对于自动置位事件，所有正处于等待状态下线程只有一个可变成可调度状态
#pragma once
#include "Constants.h"

//快线程
unsigned int __stdcall FastThreadFunc(void *pPM)
{
	Sleep(10);		//用来保证各线程调用等待函数的次序有一定的随机性
	cout << (PSTR)pPM << " Start" << endl;
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	cout << (PSTR)pPM << " End" << endl;
	return 0;
}

//慢线程
unsigned int __stdcall SlotThreadFunc(void *pPM)
{
	Sleep(100);		//用来保证各线程调用等待函数的次序有一定的随机性
	cout << (PSTR)pPM << " Start" << endl;
	WaitForSingleObject(g_hThreadEvent, INFINITE);
	cout << (PSTR)pPM << " End" << endl;
	return 0;
}

void TestPulseEvent()
{
	BOOL bManualReset = FALSE;
	//创建事件,参数2:手动置位=TRUE,自动置位=FALSE
	g_hThreadEvent = CreateEvent(NULL, bManualReset, FALSE, NULL);
	if (TRUE == bManualReset)
		cout << "Manual Reset" << endl;
	else
		cout << "Auto Reset" << endl;

	//线程名
	char szFastThreadName[5][30] = {"Fast Thread 01", "Fast Thread 02", "Fast Thread 03", "Fast Thread 04", "Fast Thread 05"};
	char szSlowThreadName[2][30] = {"Slow Thread 01", "Slow Thread 02"};
	//启动线程
	for (int i = 0; i < 5; ++i)
		_beginthreadex(NULL, 0, FastThreadFunc, szFastThreadName[i], 0, NULL);
	for (int i = 0; i < 2; ++i)
		_beginthreadex(NULL, 0, SlotThreadFunc, szSlowThreadName[i], 0, NULL);

	Sleep(50);		//保证线程已经全部启动
	cout << "Start Pulse Event\n";
	PulseEvent(g_hThreadEvent);

	Sleep(3000);
	cout << "Timer Out, Main Process End\n";
	CloseHandle(g_hThreadEvent);
}
