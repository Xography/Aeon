/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#ifndef ThreadsafeQueue_h__
#define ThreadsafeQueue_h__

#include <deque>

#include <Aeon/Multicore/Types.h>

namespace Aeon {
	/**
	 * @brief A basic thread-safe queue implementation.
	 */
	template<typename T> class ThreadsafeQueue
	{
	public:
		/**
		 * Explicit constructor.
		 *
		 * @param capacity The maximum size of the queue.
		 */
		explicit ThreadsafeQueue(std::size_t capacity):
			capacity(capacity)
		{
			
		}

	private:
		/**
		 * Copy constructor.
		 */
		ThreadsafeQueue(const ThreadsafeQueue&)
		{
			
		}

		/**
		 * Assign constructor.
		 */
		ThreadsafeQueue& operator=(const ThreadsafeQueue&)
		{
			
		}

	public:
		/**
		 * Pushes the specified item to the back of the queue.
		 */
		void PushBack(T item)
		{
			Aeon::UniqueLock<Aeon::Mutex>::type lock(mutex);
			notFull.wait(lock, BIND(&ThreadsafeQueue<T>::IsNotFull, this));
			
			container.push_back(item);
			lock.unlock();

			notEmpty.notify_one();
		}

		/**
		 * Removes the first item in the queue and returns it.
		 */
		T PopFront()
		{
			Aeon::UniqueLock<Aeon::Mutex>::type lock(mutex);
			notEmpty.wait(lock, BIND(&ThreadsafeQueue<T>::IsNotEmpty, this));

			T item = container.front();

			container.pop_front();
			lock.unlock();

			notFull.notify_one();

			return item;
		}

	public:
		/**
		 * Indicates if the queue contains items.
		 */
		bool IsNotEmpty()
		{
			return (container.size() > 0);
		}

		/**
		 * Indicates if the queue can support additional items.
		 */
		bool IsNotFull()
		{
			return (container.size() < capacity);
		}

	private:
		/**
		 * The mutex used for locking.
		 */
		Aeon::Mutex mutex;

		/**
		 * The maximum capacity of the queue.
		 */
		std::size_t capacity;

		/**
		 * The actual queue.
		 */
		std::deque<T> container;

		/**
		 * Conditional variables.
		 */
		Aeon::ConditionVariable notEmpty, notFull;
	};
}

#endif // ThreadsafeQueue_h__
