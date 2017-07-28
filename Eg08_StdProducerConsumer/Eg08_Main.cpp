#include <iostream>
#include "PipeLine.hpp"
using namespace std;

int main()
{
	PipeLine<int, 4> buffer;
	const int nProductCount = 11;

	thread producer([&]()
	{
		for (int i = 0; i < nProductCount; ++i)
		{
			buffer.PutData(i);
		}
		buffer.finish();
	});

	thread procedure([&]()
	{
		while (buffer.ProcessData([](int& data)
		{
			data = data * data;
		}))
		{
		};
	});

	thread consumer([&]()
	{
		while (true)
		{
			int* data = buffer.TakeData();
			if (data == nullptr)
				break;
		}
		cout << "Finish..." << endl;
	});

	producer.detach();
	procedure.detach();
	consumer.detach();

	system("pause");
	return 0;
}