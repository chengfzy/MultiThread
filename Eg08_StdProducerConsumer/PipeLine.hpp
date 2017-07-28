#pragma once
#include <iostream>
#include <thread>
#include <mutex>


// Pipeline Buffer: Producer - Processor(Procedure Line) - Consumer
template<typename T = int, int N = 10>
class PipeLine
{
public:
	PipeLine()
		: m_nProducerPos(0), m_nProcedurePos(0), m_nConsumerPos(0), m_nCount(0), m_bProduceFinish(false), m_bProcessFinish(false)
	{
		for (int i = 0; i < N; ++i)
			m_bProcessed[i] = false;
	}

	PipeLine(const PipeLine&) = delete;
	PipeLine& operator=(const PipeLine&) = delete;
	
public:
	/**
	 * put data into buffer.
	 *
	 * @param	data	The data.
	 */
	void PutData(const T& data)
	{
		std::unique_lock<std::mutex> lock(m_bufferMutex);
		m_spaceAvailable.wait(lock, [this] {return m_nCount < N; });
		m_buffer[m_nProducerPos] = data;
		m_bProcessed[m_nProducerPos] = false;
		++m_nCount;
		cout << "Put Data into Buffer: Data[" << m_nProducerPos << "] = " << data << endl;
		lock.unlock();

		m_nProducerPos = (m_nProducerPos + 1) % N;
		m_rawDataAvailable.notify_all();
	}


	/**
	 * Process the data.
	 *
	 * @param	process	The process function.
	 *
	 * @return	True if it succeeds, false if pipeline finished and has nothing to do
	 */
	bool ProcessData(std::function<void(T&)> process)
	{
		// if pipeline not finished, take data from buffer
		std::unique_lock<std::mutex> lock(m_bufferMutex);
		m_rawDataAvailable.wait(lock, [this] { return (m_nCount > 0 && !m_bProcessed[m_nProcedurePos]) || m_bProduceFinish; });	
		if (m_nCount > 0 && !m_bProcessed[m_nProcedurePos])
		{
			T& data = m_buffer[m_nProcedurePos];
			lock.unlock();

			// processs data
			process(data);

			// finish process
			lock.lock();
			m_bProcessed[m_nProcedurePos] = true;
			m_nProcedurePos = (m_nProcedurePos + 1) % N;
			cout << "Process Data: Data[" << m_nProcedurePos << "] = " << data << endl;
			m_productAvailable.notify_all();
			return true;
		}
		else
		{
			// if pipeline finish, return false
			m_bProcessFinish = true;
			lock.unlock();
			return false;
		}
	}

	/**
	 * take data from buffer, return nullptr if finish pipeline.
	 *
	 * @return	nullptr if pipeline is finished, else return data pointer
	 */
	T* TakeData()
	{
		T* data = nullptr;
		
		// if finish pipeline, return nullptr, else take data from buffer
		std::unique_lock<std::mutex> lock(m_bufferMutex);
		m_productAvailable.wait(lock, [this] { return (m_nCount > 0 && m_bProcessed[m_nConsumerPos])|| m_bProcessFinish; });
		if (m_nCount > 0 && m_bProcessed[m_nConsumerPos])
		{
			data = &m_buffer[m_nConsumerPos];
			--m_nCount;
			cout << "Get Data from Buffer: Data[" << m_nConsumerPos << "] = " << *data << endl;
			lock.unlock();

			m_nConsumerPos = (m_nConsumerPos + 1) % N;
			m_spaceAvailable.notify_all();
		}

		return data;
	}

	/** pipeline finished. */
	void finish()
	{
		std::unique_lock<std::mutex> lock(m_bufferMutex);
		m_bProduceFinish = true;
		lock.unlock();
	}

private:
	T m_buffer[N];

	int m_nProducerPos;
	int m_nProcedurePos;
	int m_nConsumerPos;
	int m_nCount;

	bool m_bProduceFinish;
	bool m_bProcessed[N];
	bool m_bProcessFinish;
	

	std::mutex m_bufferMutex;
	std::condition_variable m_spaceAvailable;
	std::condition_variable m_rawDataAvailable;
	std::condition_variable m_productAvailable;
};
