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
#ifndef TaskScheduler_h__
#define TaskScheduler_h__

#include <Aeon/Aeon.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/Export.h>
#include <Aeon/Singleton.h>
#include <Aeon/Multicore/Types.h>

#include <deque>

namespace Aeon {

	/**
	 * @brief Base class for implementing tasks.
	 */
	class AEON_EXPORT Task
	{
	public:
		virtual ~Task() {}
		
	public:
		/**
		 * Destroys the task
		 */
		static void Destroy(Task* ptr)
		{
			delete ptr;
		}
		
	public: 
		/**
		 * Called by the task manager when the task should be executed.
		 */
		virtual void Run() = 0;
	};
	
	/**
	 * @brief This task scheduler offers a set of methods which allow scheduling tasks for later execution.
	 * 
	 * TODO: Fairly incomplete right now.
	 * 
	 */
	class AEON_EXPORT TaskScheduler : public Aeon::Singleton<TaskScheduler>
	{
		friend class Aeon::Singleton<TaskScheduler>;

	public:

		/**
		 * Schedule a synchronous task to be executed the next update cycle.
		 */
		void Schedule(Task* task);

		/**
		 * Schedules a task to be executed in a seperate thread somewhere in the future.
		 * The time of execution depends on the amount of tasks in the queue and the length of execution for the remaining tasks.
		 */
		//void ScheduleAsync(Task* task, uint32_t maxExecutionTimeMsec = 1000);
		//void ScheduleTimed(uint32_t waitUntilExecuttion, Task task);
		
		/**
		 * Drops all tasks without executing them
		 */
		//void DropTasks();
		
		/**
		 * Executes all remaining tasks in the queue in the order they were added.
		 */
		//void ExecuteAll();

	public:
		/**
		 * Updates the task scheduler.
		 */
		void OnUpdate(uint32_t msecElapsed);

	private:
		/**
		 * List of synchronous tasks
		 */
		std::deque<Task*> tasks;

		/**
		 * Elapsed time.
		 */
		float timeElapsed;

		/**
		 * Mutex
		 */
		Aeon::Mutex mutex;
	};
}

#endif // TaskScheduler_h__