

//--------------------------------------------------------------------
//	threadsManager.cpp.
//	05/19/2021.				created.
//	05/19/2021.				lasted modified.
//--------------------------------------------------------------------


#include "threadsManager.h"


//--------------------------------------------------------------------
void threadPool::threadsManager::addTask(Task task)
{
	if (_threadQ.empty())
	{
		fncQueue.push(task);
	}
	else
	{
		auto _thread = _threadQ.front();
		_threadQ.pop();
		_thread->setTask(task);
		_thread->wake_up();
	}
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------

void threadPool::threadsManager::setMaxThreadsCount(size_t count)
{
	_maxThreadCount = count;
	std::lock_guard<std::mutex> lck(_mutexOfthrdQ);
	while (_threadQ.size()> count)
	{
		_threadQ.front()->killThread();
		_threadQ.pop();
		--_threadCount;
	}
	while (_threadCount < count)
	{

		std::shared_ptr<Thread> pt(new Thread);
		pt->setFinishCallback(std::bind(&threadsManager::finishCallback, this, pt));
		_threadQ.push(pt);
		pt->detach();
		++_threadCount;
	}
}

//--------------------------------------------------------------------


//--------------------------------------------------------------------
threadPool::threadsManager::threadsManager(size_t maxThreadCount)
	:_maxThreadCount(maxThreadCount)
{
	while (maxThreadCount-- > 0)
	{
		std::shared_ptr<Thread> pt(new Thread);
		pt->setFinishCallback(std::bind(&threadsManager::finishCallback, this, pt));
		_threadQ.push(pt);
	    ++_threadCount;
	}
}
//--------------------------------------------------------------------


//--------------------------------------------------------------------

void threadPool::threadsManager::finishCallback(std::shared_ptr<Thread> t)
{
	if (!fncQueue.empty())
	{
		std::lock_guard<std::mutex> lck(_mutexOfFunQ);
		t->setTask(fncQueue.front());
		fncQueue.pop();
	}
	else
	{
		std::lock_guard<std::mutex> lck(_mutexOfthrdQ);
		if (_threadQ.size() < _maxThreadCount)
		{
			_threadQ.push(t);
		}
		else
		{
			t->killThread();
			--_threadCount;
		}
	}
}

//--------------------------------------------------------------------
