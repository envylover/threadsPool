#pragma once
//--------------------------------------------------------------------
//	thread.h.
//	05/19/2021.				created.
//	05/19/2021.				lasted modified.
//--------------------------------------------------------------------


#ifndef _THREAD_H
#define _THREAD_H


                /*  header  */
//-------------------------------------------------------------
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
//-------------------------------------------------------------



namespace threadPool
{

	using Task = std::function<void(void)>;
	
	 class Thread 
		:public std::thread 
	{
	//--------------------------------------------------------------------

		Task                            _task;
		Task                            onFinish;
		std::mutex                      _mutex;
		std::condition_variable         _wakeup;
		std::atomic_bool                isAlive = true;

	//--------------------------------------------------------------------
	public:
		void wake_up(void);
		void killThread(void);
		void setTask(const Task& task);
		void setFinishCallback(const Task& task);
		Thread() :std::thread{ [this]()->void {
			while (true)
			{
				if (!isAlive)
					break;
				if(!_task)
				{
					std::unique_lock<std::mutex> ulock(_mutex);
					_wakeup.wait(ulock);
				}
				if (_task)
				{
					_task();
					Task invalid;
					_task.swap(invalid);
				}
				if (onFinish)
					onFinish();
			}
		} } { }
		~Thread() { }
	};
}

#endif // _THREAD_H


