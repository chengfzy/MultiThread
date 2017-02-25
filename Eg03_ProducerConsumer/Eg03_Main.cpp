#include <iostream>
#include "SingleBuffer.h"
#include "MultipleBuffer.h"
using namespace std;


int main()
{
	//1生产者，1消费者，1缓冲区
	//SingleBufferTest();

	//1生产者，2消费者，4缓冲区
	MultiBufferTest();
	
	system("pause");
	return 0;
}
