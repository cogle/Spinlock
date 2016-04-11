#pragma once
#include <atomic>

class spinlock_mutex
{
public:

	spinlock_mutex()
	{
		flag.clear();
	}

	~spinlock_mutex() {};

	spinlock_mutex(const spinlock_mutex&) = delete;
	spinlock_mutex(const spinlock_mutex&&) = delete;
	spinlock_mutex & operator=(const spinlock_mutex&) = delete;


	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}

	void unlock()
	{
		flag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag flag;

};