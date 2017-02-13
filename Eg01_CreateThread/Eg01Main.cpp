#include <iostream>
#include <Windows.h>
#include <process.h>		//_beginthreadex

using namespace std;

int g_nCount;					//全局函数
const int THREAD_NUM = 64 ;		//Thread Number

//子线程函数for CreateThread
DWORD WINAPI CreateThreadFun(LPVOID pM)
{
	Sleep(100);
	//++g_nCount;
	InterlockedIncrement((unsigned int*)&g_nCount);	//改成原子操作InterlockedXXXX

	//cout << "The ID of CreateThreadFun = " << GetCurrentThreadId() << "\tCount = " << g_nCount << endl;
	return 0;
}

//子线程函数for _beginthreadex
unsigned int _stdcall BeginThreadFun(PVOID pM)
{
	Sleep(100);
	//++g_nCount;
	InterlockedIncrement((unsigned int*)&g_nCount);	//改成原子操作InterlockedXXXX

	//cout << "The ID of BeginThreadFun = " << GetCurrentThreadId() << "\tCount = " << g_nCount << endl;
	return 0;
}


//test for CreateThread function
void TestCreateThread()
{
	cout << "CreateThread()" << endl;
	int num(20);
	while (num--)
	{
		g_nCount = 0;
		HANDLE handle[THREAD_NUM];
		for (int i = 0; i < THREAD_NUM; ++i)
			handle[i] = CreateThread(NULL, 0, CreateThreadFun, NULL, 0, NULL);
		WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
		cout << "Count = " << g_nCount << endl;
	}
}

//test for BeginThread function
void TestBeginThread()
{
	cout << "\nBeginThread()" << endl;
	int num(20);
	while (num--)
	{
		g_nCount = 0;
		HANDLE handle[THREAD_NUM];
		for (int i = 0; i < THREAD_NUM; ++i)
			handle[i] = (HANDLE)_beginthreadex(NULL, 0, BeginThreadFun, NULL, 0, NULL); 
		WaitForMultipleObjects(THREAD_NUM, handle, TRUE, INFINITE);
		cout << "Count = " << g_nCount << endl;
	}
}


//主线程函数
int main()
{	
	TestCreateThread();
	TestBeginThread();

	system("pause");
	return 0;
}
