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
#ifndef FintiteStateMachine_h__
#define FintiteStateMachine_h__

#include <Aeon/Multicore/Types.h>

namespace Aeon {
	/**
	 * @brief Thread-safe finite state machine class.
	 */
	template<typename T> class FiniteStateMachine
	{
	public:
		/**
		 * Constructor.
		 */
		FiniteStateMachine(const T& startValue, FUNCTION<void(const T&, const T&)> callback):
			callback(callback),
			state(startValue)
		{

		}

		/**
		 * Destructor.
		 */
		~FiniteStateMachine()
		{	

		}

	private:
		/**
		 * Copy constructor.
		 */
		FiniteStateMachine(const FiniteStateMachine<T>& x)
		{

		}

		/**
		 * Assignment operator.
		 */
		FiniteStateMachine<T>& operator=(const FiniteStateMachine<T>& x)
		{

		}

	public:
		/**
		 * Retrieves the current state. If another thread is busy modifying the state, this may or may not return the state being set (you could use SetState(same_state) to check).
		 */
		const T& GetState() const
		{
			return state;
		}

		/**
		 * Changes the state to the specified value. Only one thread can change the state at a time.
		 *
		 * @param value The new state to set. If the state is the same as it was, nothing happens.
		 * @param wait If set and another thread is busy modifying the state, this function will wait until it can change the state and proceed. Otherwise it will return immediately and fail.
		 *
		 * @return 'true' if the requested state was set (even if the state was the same) and 'false' if not (i.e. because wait was set to false and another thread is modifying the state).
		 */
		bool SetState(const T& value, bool wait = true)
		{
			if(wait)
				mutex.lock();

			else if(!mutex.try_lock())
				return false; // Boo, no lock!

			if(state != value)
			{
				callback(value, state);
				state = value;
			}

			mutex.unlock();

			return true;
		}

	public:
		/**
		 * Callback to use when states change.
		 */
		FUNCTION<void(const T&, const T&)> callback;

		/**
		 * The current state value.
		 */
		T state;

		/**
		 * Mutex used to ensure only one instance can modify the state at a time.
		 */
		Aeon::Mutex mutex;
	};
}

#endif // FintiteStateMachine_h__
