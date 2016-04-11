// Spinlock.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <mutex>
#include <time.h> 
#include <thread>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <algorithm>

#include "Spinlock.h"


static std::mutex m {};
static spinlock_mutex splm {};
const unsigned int ITERS = 1000000;
const static int thread_vec_size = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 6;



template <typename T>
void push_function(std::vector<int> & vec, T & _m) 
{
	std::lock_guard<T> lock(_m);
	vec.push_back(rand() % 100000);
}

template <typename T>
void pop_function(std::vector<int> & vec,  T & _m)
{
	std::lock_guard<T> lock(_m);
	vec.pop_back();
}

template <typename T>
void run(std::vector<int> & v, T & _m) 
{
	for (int i = 0; i < ITERS; i++)
	{
		push_function<T>(v, _m);
	}
	for (int i = 0; i < ITERS; i++)
	{
		pop_function<T>(v, _m);
	}
}



int main()
{

	clock_t t;
	srand((unsigned int)time(NULL));

	std::vector<std::thread> mutex_thread_vec;
	std::vector<std::thread> spinlock_thread_vec;

	std::vector<int> test_vec;

	/*You don't really need all the threads in the example as it will consume the CPU*/
	t = clock();
	for (int i  = 0; i < thread_vec_size/2; i++) 
	{
		spinlock_thread_vec.push_back(std::thread([&] 
		{
			run<spinlock_mutex>(test_vec,splm);
		}));
	}
	std::for_each(spinlock_thread_vec.begin(), spinlock_thread_vec.end(), [&] (std::thread & t)
	{
		t.join();
	});
	t = clock() - t;

	std::cout << "The spinlock took " << t/CLOCKS_PER_SEC << " clicks to complete" << std::endl;
	test_vec.clear();

	t = clock();
	for (int i = 0; i < thread_vec_size; i++)
	{
		mutex_thread_vec.push_back(std::thread([&]
		{
			run<std::mutex>(test_vec, m);
		}));
	}
	std::for_each(mutex_thread_vec.begin(), mutex_thread_vec.end(), [&](std::thread & t)
	{
		t.join();
	});
	t = clock() - t;
	std::cout << "The mutex took " << t/CLOCKS_PER_SEC << " clicks to complete" << std::endl;

    return 0;
}

