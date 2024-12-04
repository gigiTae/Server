#pragma once

#include <chrono>
#include <condition_variable>
#include <vector>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <future>
#include <cstdio>
#include <functional>

namespace common
{
	/// <summary>
	/// 쓰레드 풀
	/// </summary>
	class ThreadPool
	{
	public:
		static ThreadPool* Get();
		static void Delete();

	public:
		template <class F, class... Args>
		std::future<std::invoke_result_t<F, Args...>> EnqueueJob(F&& function, Args&&... args);
		
	private:
		ThreadPool(size_t InNumThread);
		~ThreadPool();

		void WorkerThread(size_t index);

	private:
		inline static ThreadPool* Instance = nullptr;

		size_t NumThread;
		std::vector<std::thread> Worker;
		std::queue<std::function<void()>> Jobs;
		std::condition_variable JobCV;
		std::mutex JobMutex;
		bool bIsStopAll;
	};


	template<class F, class ...Args>
	inline std::future<std::invoke_result_t<F, Args...>> ThreadPool::EnqueueJob(F&& function, Args && ...args)
	{
		using return_type = typename std::invoke_result_t<F, Args...>;

		auto job = std::make_shared<std::packaged_task<return_type()>>
			(std::bind(std::forward<F>(function), std::forward<Args>(args)...));

		std::future<return_type> jobFuture = job->get_future();

		std::lock_guard<std::mutex> lock(JobMutex);
		Jobs.push([job]() {(*job)(); });

		JobCV.notify_one();

		return jobFuture;
	}
}
