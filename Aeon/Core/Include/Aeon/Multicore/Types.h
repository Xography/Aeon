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
#ifndef AeonMulticore_h__
#define AeonMulticore_h__

#include <Aeon/Platform/Platform.h>

// Older Visual C++ compilers don't support C++11 threading yet and need boost. Fortunately C++11's concurrency classes are an
// almost exact replica of boost's variants.
#if (defined(AEON_WINDOWS) && _MSC_VER < 1700) || (defined(AEON_WINDOWS) && defined(__GNUC__)) || (defined(AEON_LINUX) && (__GNUC__ < 4 || __GNUC_MINOR__ < 7))
#	define AEON_USE_BOOST_THREADS

#	include <boost/bind/bind.hpp>
#	include <boost/bind/placeholders.hpp>

#	include <boost/thread.hpp>
#	include <boost/function.hpp>
#	include <boost/scoped_ptr.hpp>

#	include <boost/thread/once.hpp>
#	include <boost/thread/mutex.hpp>
#	include <boost/thread/condition_variable.hpp>
#else
#	include <mutex>
#	include <thread>
#	include <functional>
#	include <condition_variable>
#endif // _MSC_VER < 1700

namespace Aeon {
#ifdef AEON_USE_BOOST_THREADS
#	define BIND				boost::bind
#	define PLACEHOLDERS		// In the latest boost versions (at least 1.51 and up), placeholders were moved to an unnamed namespace, so "::_1" is enough.
#	define FUNCTION			boost::function
#	define THREAD_YIELD			boost::this_thread::yield()
#	define THIS_THREAD_ID	boost::this_thread::get_id()

	typedef boost::mutex Mutex;
	typedef boost::thread Thread;
	typedef boost::once_flag OnceFlag;
	typedef boost::thread::id InternalThreadID;
	typedef boost::condition_variable ConditionVariable;

	/**
	 * @brief Unique thread lock .
	 */
	template<typename T> struct UniqueLock
	{
		typedef boost::unique_lock<T> type;
	};
#else
#	define BIND				std::bind
#	define PLACEHOLDERS		std::tr1::placeholders
#	define FUNCTION			std::function
#	define THREAD_YIELD			std::this_thread::yield()
#	define THIS_THREAD_ID	std::this_thread::get_id()

	// Typedefs to abstract from actual thread implementation.
	typedef std::mutex Mutex;
	typedef std::thread Thread;
	typedef std::once_flag OnceFlag;
	typedef std::thread::id InternalThreadID;
	typedef std::condition_variable ConditionVariable;
	
	/**
	 * @brief Unique thread lock .
	 */
	template<typename T> struct UniqueLock
	{
		// TODO: Use C++11's template<typename T> typedef boost::unique_lock<T> UniqueLock for this when it is available (which is not in VS 2012 yet).
		typedef std::unique_lock<T> type;
	};
#endif // AEON_USE_BOOST_THREADS
}

#endif // AeonMulticore_h__
