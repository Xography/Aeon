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
#include <Aeon/TaskScheduler.h>

namespace Aeon {

	// -------------------------------------------------------------------------
	void TaskScheduler::Schedule(Task* task)
	{
		mutex.lock();
		tasks.push_back(task);
		mutex.unlock();
	}
	/*

	// -------------------------------------------------------------------------
	void TaskScheduler::ScheduleAsync(Task* task, uint32_t maxExecutionTimeMsec)
	{
		// TODO: Implement me
	}
	*/
	// -------------------------------------------------------------------------
	void TaskScheduler::OnUpdate(uint32_t /*msecElapsed*/)
	{
		mutex.lock();

		// Execute one task per update
		if(tasks.empty())
		{
			mutex.unlock();
			return;
		}

		Task* ptr = tasks.front();
		ptr->Run();
		tasks.pop_front();
		Task::Destroy(ptr);
		mutex.unlock();
	}
	/*

	// -------------------------------------------------------------------------
	void TaskScheduler::ExecuteAll()
	{
		// TOOD: Implement me
	}


	// -------------------------------------------------------------------------
	void TaskScheduler::DropTasks()
	{
		// TODO: Implement me
	}
	*/
}