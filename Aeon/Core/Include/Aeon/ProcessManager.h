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
#ifndef ProcessManager_h__
#define ProcessManager_h__

#include <unordered_map>

#include <Aeon/AeonTypes.h>
#include <Aeon/Singleton.h>

#include <Aeon/Types/HashedKey.h>
#include <Aeon/Export.h>
#include <Aeon/Multicore/Types.h>

/**
 * These macro must be called by every thread created by the process manager at their start and their end(if they don't need a loop then this
 * isn't necessary). It checks whether other threads have requested that this thread be paused or stopped and waits or stops if necessary.
 */
#define THREAD_BEGIN_LOOP																\
	auto state = Aeon::ProcessManager::Instance().GetStateFor(THIS_THREAD_ID);			\
																						\
	for(;;)																				\
	{																					\
		do																				\
		{																				\
			if(state->stopRunning)														\
				return;																	\
		}																				\
		while(state->mustWait);

#define THREAD_END_LOOP																	\
	}

namespace Aeon {
	/**
	 *  @brief Process manager, uses std::thread (if available) or otherwise boost::thread (if supported).
	 */
	class AEON_EXPORT ProcessManager : public Singleton<ProcessManager>
	{
		friend class Singleton<ProcessManager>;

	public:
		/**
		 * @brief Represents the state of a thread.
		 */
		struct ThreadState
		{
			/**
			 * If set, the thread has to wait (preferably call YIELD) until this flag becomes false.
			 */
			volatile bool mustWait;

			/**
			 * If set, the thread has to terminate immediately (or as soon as possible).
			 */
			volatile bool stopRunning;
		}; 

	private:
		/**
		 * Used for bookkeeping.
		 */
		struct ThreadPair
		{
			Thread* thread;
			ThreadState* state;
		};

	public:
		typedef HashedKey ThreadID;
		typedef std::unordered_map<ThreadID, ThreadPair, AggregateKeyHash, AggregateKeyEquality> ThreadMap;

	protected:
		/**
		 * Constructor.
		 */
		ProcessManager();

		/**
		 * Constructor.
		 */
		ProcessManager(const ProcessManager&);

	public:
		/**
		 * Destructor.
		 */
		~ProcessManager();

	public:
		/**
		 * Retrieves the thread state object for the thread with the specified ID. This should not be used directly. Use THIS_THREAD_BEGIN as the first
		 * command at the start of the for-loop in your thread to check whether should be paused or not.
		 *
		 * @param id The internal ID (not the thread ID returned by the Create function) of the thread of which the state has to be retrieved. You can
		 * get the ID of the current thread by specifying THIS_THREAD_ID as parameter.
		 *
		 * @see THIS_THREAD_BEGIN
		 */
		const ThreadState* GetStateFor(const InternalThreadID& id) const;

	public:
		/**
		 * Creates a new thread.
		 *
		 * @param name The unique name to give to the thread.
		 * @param entryPoint The function where the thread starts running (it must return void and take no arguments). You can use the BIND macro (which
		 * equals std::bind or boost::bind) to create a correct value.
		 *
		 * @return The ID of the newly created thread. If its creation failed (i.e. because the name wasn't unique), its IsNull() function will return true.
		 */
		ThreadID Create(const String& name, FUNCTION<void()> entryPoint);

		/**
		 * Pauses the thread with the specified ID. Does nothing if the thread with the specified ID doesn't exist.
		 */
		void Pause(const ThreadID& id);

		/**
		 * Pauses all running processes.
		 */
		void PauseAll();

		/**
		 * Resumes the thread with the specified ID. Does nothing if the thread with the specified ID doesn't exist.
		 */
		void Resume(const ThreadID& id);

		/**
		 * Resumes all paused processes.
		 */
		void ResumeAll();

		/**
		 * Stops (kills) the specified process and cleans up. Does nothing if the thread with the specified ID doesn't exist.
		 */
		void Stop(const ThreadID& id);

		/**
		 * Stops (kills) all processes and cleans up.
		 */
		void StopAll();

	private:
		/**
		 * Maps thread ID's to actual lower level thread instances.
		 */
		ThreadMap threads;

		/**
		 * Mutex used to lock the process manager itself. This serves as a global lock and is used in all functions of this class.
		 */
		mutable Mutex lock;

		/**
		 * This lock is used to lock all thread-safe functionalities except GetStateFor. This serves as an *additional* lock next to 'lock'.
		 */
		mutable Mutex functionalityLock;
	};
}

#endif // ProcessManager_h__
