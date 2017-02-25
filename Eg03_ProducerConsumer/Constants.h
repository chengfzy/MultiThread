#pragma once
#include <Windows.h>

const int END_PRODUCE_NUMBER = 10;						//生产产品个数

CRITICAL_SECTION g_cs;									//Critical Section
HANDLE g_hEventBufferEmpty, g_hEventBufferFull;			//Event
HANDLE g_hSemaphoreBufferEmpty, g_hSemaphoreBufferFull;	//Semaphore

//设置控制台输出颜色
BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

