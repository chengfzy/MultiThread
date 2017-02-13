#pragma once
#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

long g_nNum;					//全局资源
const int THREAD_NUM = 10;		//子线程个数

//关键段, Critical Section
CRITICAL_SECTION g_csThreadParameter, g_csThreadCode;

//事件
HANDLE g_hThreadEvent;

//互斥量
HANDLE g_hThreadMutex;

//信号量
HANDLE g_hThreadSemaphore;
