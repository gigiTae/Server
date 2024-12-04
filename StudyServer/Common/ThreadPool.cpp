#include "ThreadPool.h"


common::ThreadPool* common::ThreadPool::Get()
{
	if (Instance == nullptr)
	{
		Instance = new ThreadPool(std::thread::hardware_concurrency());
	}

	return Instance;
}

common::ThreadPool::ThreadPool(size_t InNumThread)
	:NumThread(InNumThread)
	,bIsStopAll(false)
{
	Worker.reserve(NumThread);

	for (size_t i = 0; i < NumThread; ++i)
	{
		Worker.push_back(std::thread([i, this]()
			{
				this->WorkerThread(i);
			}));
	}
}

common::ThreadPool::~ThreadPool()
{
	bIsStopAll = true;
	JobCV.notify_all();

	for (auto& thread : Worker)
	{
		thread.join();
	}
}

void common::ThreadPool::WorkerThread(size_t index)
{
	while (true)
	{
		std::unique_lock<std::mutex> lock(JobMutex);
		JobCV.wait(lock, [this, index]()
			{
				return !Jobs.empty() || bIsStopAll;
			});
		
		// ������ ����
		if (bIsStopAll && Jobs.empty())
		{
			return;
		}

		std::function<void()> job = std::move(Jobs.front());
		Jobs.pop();
		lock.unlock();
		job();
	}
}
