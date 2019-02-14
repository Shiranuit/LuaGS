#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SharedQueue
{
public:
	SharedQueue();
	~SharedQueue();

	T& front();
	void pop_front();

	void push_back(const T& item);
	void push_back(T&& item);

	int size();
	bool empty();

private:
	std::deque<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};
