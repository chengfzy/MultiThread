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
CRITICAL_SECTION g_cs, g_csWriterCount;
HANDLE g_hEventWriter, g_hEventNoReader;
int g_nReaderCount;

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
	//等待写者完成
	WaitForSingleObject(g_hEventWriter, INFINITE);

	//读者个数增加
	EnterCriticalSection(&g_csWriterCount);
	++g_nReaderCount;
	if (g_nReaderCount == 1)
		ResetEvent(g_hEventNoReader);
	LeaveCriticalSection(&g_csWriterCount);

	//读取文件
	ReaderPrintf("Reader ID = %d Start to Read...\n", GetCurrentThreadId());
	
	Sleep(rand() % 100);

	//结束阅读，读者个数减小，空位增加
	ReaderPrintf("Read ID = %d End Read\n", GetCurrentThreadId());

	//读者个数减少
	EnterCriticalSection(&g_csWriterCount);
	--g_nReaderCount;
	if (g_nReaderCount == 0)
		SetEvent(g_hEventNoReader);
	LeaveCriticalSection(&g_csWriterCount);

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
	//等待读文件的读者为零
	WaitForSingleObject(g_hEventNoReader, INFINITE);
	//标记写者正在写文件
	ResetEvent(g_hEventWriter);

	//写文件
	WriterPrintf("Writer Start to Write...\n");
	Sleep(rand() % 100);
	WriterPrintf("Writer End Write\n");

	//标记写者结束写文件
	SetEvent(g_hEventWriter);
	return 0;
}


int main()
{
	cout << "Writer & Reader Problem" << endl;
	
	//初始化事件和信号量
	InitializeCriticalSection(&g_cs);
	InitializeCriticalSection(&g_csWriterCount);
	//手动置位，初始已触发
	g_hEventWriter = CreateEvent(NULL, TRUE, TRUE, NULL);
	g_hEventNoReader = CreateEvent(NULL, TRUE, TRUE, NULL);	
	g_nReaderCount = 0;
		
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

	//销毁事件和信号量
	CloseHandle(g_hEventWriter);
	CloseHandle(g_hEventNoReader);
	DeleteCriticalSection(&g_cs);
	DeleteCriticalSection(&g_csWriterCount);

	system("pause");
	return 0;
}
