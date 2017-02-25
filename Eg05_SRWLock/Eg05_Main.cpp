#include <iostream>
#include <Windows.h>
#include <process.h>
using namespace std;

//设置控制台输出颜色
BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

const int READER_NUM = 5;			//读者个数
//关键段和事件
CRITICAL_SECTION g_cs;
SRWLOCK g_srwLock;

//读者线程输出函数
void ReaderPrintf(char* pszFormat, ...)
{
	va_list pArgList;
	va_start(pArgList, pszFormat);
	EnterCriticalSection(&g_cs);
	vfprintf(stdout, pszFormat, pArgList);
	LeaveCriticalSection(&g_cs);
	va_end(pArgList);
}

//读者线程函数
unsigned int __stdcall ReaderThreadFunc(PVOID pM)
{
	ReaderPrintf("Reader ID = %d waiting...\n", GetCurrentThreadId());
	//读者申请读取文件
	AcquireSRWLockShared(&g_srwLock);

	//读取文件
	ReaderPrintf("Reader ID = %d Start to Read...\n", GetCurrentThreadId());	
	Sleep(rand() % 100);
	ReaderPrintf("Read ID = %d End Read\n", GetCurrentThreadId());

	//结束读取文件
	ReleaseSRWLockShared(&g_srwLock);
	return 0;
}


//写者线程输出函数
void WriterPrintf(char* pszStr)
{
	EnterCriticalSection(&g_cs);
	SetConsoleColor(FOREGROUND_GREEN);
	printf("\t%s\n", pszStr);
	SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	LeaveCriticalSection(&g_cs);
}

//写者线程函数
unsigned int __stdcall WriterThreadFunc(PVOID pM)
{
	WriterPrintf("Writer Waiting...\n");
	//写者申请写文件
	AcquireSRWLockExclusive(&g_srwLock);

	//写文件
	WriterPrintf("Writer Start to Write...\n");
	Sleep(rand() % 100);
	WriterPrintf("Writer End Write\n");

	//写者结束写文件
	ReleaseSRWLockExclusive(&g_srwLock);
	return 0;
}


int main()
{
	cout << "Writer & Reader Problem, Using SRWLock" << endl;
	
	//初始化读写锁和关键段
	InitializeCriticalSection(&g_cs);
	InitializeSRWLock(&g_srwLock);
		
	HANDLE hThread[READER_NUM + 1];
	//先启动2个读者线程
	for (int i = 1; i <= 2; ++i)
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFunc, NULL, 0, NULL);
	//启动写者线程
	hThread[0] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadFunc, NULL, 0, NULL);
	Sleep(50);
	//最后启动其他读者线程
	for (int i = 3; i <= READER_NUM; ++i)
		hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFunc, NULL, 0, NULL);
	WaitForMultipleObjects(READER_NUM + 1, hThread, TRUE, INFINITE);
	for (int i = 0; i <= READER_NUM; ++i)
		CloseHandle(hThread[i]);

	//销毁关键段
	DeleteCriticalSection(&g_cs);

	system("pause");
	return 0;
}
