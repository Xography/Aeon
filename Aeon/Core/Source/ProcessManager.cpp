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
#include <Aeon/ProcessManager.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	ProcessManager::ProcessManager()
	{
		
	}

	// -------------------------------------------------------------------------
	ProcessManager::ProcessManager(const ProcessManager&)
	{
		
	}

	// -------------------------------------------------------------------------
	ProcessManager::~ProcessManager()
	{

	}

	// -------------------------------------------------------------------------
	const ProcessManager::ThreadState* ProcessManager::GetStateFor(const InternalThreadID& id) const
	{
		lock.lock();

		for(auto i(threads.begin()); i != threads.end(); ++i)
		{
			if(i->second.thread->get_id() == id)
			{
				lock.unlock();
				return i->second.state;
			}
		}

		lock.unlock();

		return nullptr;
	}

	// -------------------------------------------------------------------------
	ProcessManager::ThreadID ProcessManager::Create(const String& name, FUNCTION<void()> entryPoint)
	{
		lock.lock();
		functionalityLock.lock();
		
		ThreadID id(name);
		
		if(threads.find(id) == threads.end())
		{
			auto& i = threads[id];

			ThreadState* state = new ThreadState();
			state->mustWait = false;
			state->stopRunning = false;

			i.state = state;
			i.thread = new Thread(entryPoint);

			functionalityLock.unlock();
			lock.unlock();

			return id;
		}

		functionalityLock.unlock();
		lock.unlock();

		return ThreadID();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::Pause(const ThreadID& id)
	{
		lock.lock();
		functionalityLock.lock();

		auto i = threads.find(id);

		if(i != threads.end())
			i->second.state->mustWait = true;

		functionalityLock.unlock();
		lock.unlock();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::PauseAll()
	{
		lock.lock();
		functionalityLock.lock();

		for(auto i(threads.begin()); i != threads.end(); ++i)
			i->second.state->mustWait = true;

		functionalityLock.unlock();
		lock.unlock();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::Resume(const ThreadID& id)
	{
		lock.lock();
		functionalityLock.lock();

		auto i = threads.find(id);

		if(i != threads.end())
			i->second.state->mustWait = false;

		functionalityLock.unlock();
		lock.unlock();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::ResumeAll()
	{
		lock.lock();
		functionalityLock.lock();

		for(auto i(threads.begin()); i != threads.end(); ++i)
			i->second.state->mustWait = false;

		functionalityLock.unlock();
		lock.unlock();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::Stop(const ThreadID& id)
	{
		lock.lock();
		functionalityLock.lock();

		auto i = threads.find(id);

		if(i != threads.end())
		{
			// Note: See the source to StopAll for an explanation.
			i->second.state->mustWait = true;
			i->second.state->stopRunning = true;

			lock.unlock();
			i->second.thread->join();
			lock.lock();

			delete i->second.thread;

			threads.erase(i);
		}

		functionalityLock.unlock();
		lock.unlock();
	}

	// -------------------------------------------------------------------------
	void ProcessManager::StopAll()
	{
		lock.lock();
		functionalityLock.lock();

		// Tell all the active threads to stop running (they are either working or waiting for lock access at this time).
		for(auto i(threads.begin()); i != threads.end(); ++i)
		{
			i->second.state->mustWait = true;
			i->second.state->stopRunning = true;
		}

		// Unlock the global lock so all threads can access their state and see they need to stop running.
		lock.unlock();

		// Wait for all the threads to actually stop.
		for(auto i(threads.begin()); i != threads.end(); ++i)
			i->second.thread->join();

		// Lock again and finally kill the threads (they must have been joined or detached for this to work).
		lock.lock();

		for(auto i(threads.begin()); i != threads.end(); ++i)
			delete i->second.thread;

		threads.clear();

		functionalityLock.unlock();
		lock.unlock();
	}
}
