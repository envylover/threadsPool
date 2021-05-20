#pragma once

//--------------------------------------------------------------------
//	threadsManager.h.
//	05/19/2021.				created.
//	05/19/2021.				lasted modified.
//--------------------------------------------------------------------



#ifndef _THREADSMANAGER_H
#define _THREADSMANAGER_H

#include "thread.h"
#include <queue>
#include <assert.h>
#include <memory>
#include <atomic>

namespace threadPool
{
	class threadsManager
	{
		std::queue<Task>                     fncQueue;
		std::queue<std::shared_ptr<Thread>>  _threadQ;
		std::mutex                           _mutexOfFunQ;
		std::mutex                           _mutexOfthrdQ;
		size_t                               _maxThreadCount;
		std::atomic<size_t>                  _threadCount = 0;
	public:
		void addTask(Task task);
		void setMaxThreadsCount(size_t count);
		threadsManager(size_t maxThreadCount);
		threadsManager();

	private:
		void finishCallback(std::shared_ptr<Thread> t);
	};
}


#endif // !_THREADSMANAGER_H
