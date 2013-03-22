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
#ifndef Uncopyable_h__
#define Uncopyable_h__

#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief Abstract class that serves as base class for classes that require uncopyable instances.
	 *
	 * @note Inspired by boost::noncopyable.
	 */
	class AEON_EXPORT Uncopyable
	{
	protected:
		/**
		 * Constructor.
		 */
		Uncopyable()
		{

		}

		/**
		 * Copy constructor.
		 */
		Uncopyable(const Uncopyable& /*right*/)
		{

		}

		/**
		 * Destructor.
		 */
		~Uncopyable()
		{

		}

	private:
		/**
		 * Assignment operator.
		 */
		const Uncopyable& operator=(const Uncopyable& /*right*/)
		{
			return (*this);
		}
	};
}

#endif // Uncopyable_h__
