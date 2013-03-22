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
#ifndef Singleton_h__
#define Singleton_h__

#include <memory>

#include <Aeon/Uncopyable.h>
#include <Aeon/Multicore/Types.h>

namespace Aeon {
	/**
	 *  @brief Thread-safe Singleton implementation.
	 */
	template<class T> class Singleton : private Aeon::Uncopyable
	{
	protected:
		/**
		 * Constructor.
		 */
		Singleton()
		{

		}

		/**
		 * Destructor.
		 */
		~Singleton()
		{

		}

	public:
		/**
		 * Retrieves the singleton instance.
		 */
		static T& Instance()
		{
#ifdef AEON_USE_BOOST_THREADS
			boost::call_once(InitInstance, isInit);
#else
			std::call_once(isInit, InitInstance);
#endif // AEON_USE_BOOST_THREADS

			return *instance;
		}

		/**
		 * Initializes the instance.
		 */
		static void InitInstance() throw()
		{
			instance.reset(new T());
		}

	private:
		/**
		 * Sole instance of the singleton.
		 */
#ifdef AEON_USE_BOOST_THREADS
		static boost::scoped_ptr<T> instance;
#else
		static std::unique_ptr<T> instance;
#endif // AEON_USE_BOOST_THREADS

		/**
		 * Used to make sure the instance is only created once.
		 */
		static Aeon::OnceFlag isInit;
	};

#ifdef AEON_USE_BOOST_THREADS
	template<typename T> boost::scoped_ptr<T> Aeon::Singleton<T>::instance(nullptr);
	template<typename T> Aeon::OnceFlag Singleton<T>::isInit = BOOST_ONCE_INIT;
#else
	template<typename T> std::unique_ptr<T> Aeon::Singleton<T>::instance(nullptr);
	template<typename T> Aeon::OnceFlag Singleton<T>::isInit; // std::once_flag is automatically initialized to _ONCE_FLAG_CPP_INIT (which is 0).
#endif // AEON_USE_BOOST_THREADS
}

#endif // Singleton_h__
