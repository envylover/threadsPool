#include "threadsManager.h"

#include <iostream>
#include <chrono>
int main()
{
	int i = 0;
	std::mutex mt;
	threadPool::threadsManager tm(10);
	for (int j = 0; j < 100; j++)
		tm.addTask([=, &i, &mt]()->void {
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::lock_guard<std::mutex> lck(mt);
		std::cout <<"thread id\t: "<< std::this_thread::get_id() << "\t: " << i++ << std::endl;
		});
	std::this_thread::sleep_for(std::chrono::milliseconds(520));
	tm.setMaxThreadsCount(6);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	tm.setMaxThreadsCount(15);
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	return 0;
}