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
#ifndef NullStream_h__
#define NullStream_h__

#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief A stream to nullify messages sent to a LOG object.
	 * 
	 * @note Do not use this class directly, use the LOG macros instead.
	 */
	class AEON_EXPORT NullStream
	{
	public:
		/**
		 * Constructor.
		 */
		NullStream()
		{
			
		}

		/**
		 * Copy constructor.
		 */
		NullStream(const NullStream& /*rhs*/)
		{

		}
		
		template<typename T> NullStream& operator<<(const T& value)
		{
			return *this;
		}
	};

	/**
	 * Insertion operator overload for null stream logging, used to send output to the void.
	 */
	template<typename T> inline NullStream& operator<<(NullStream& stream, const T& value)
	{
		return stream;
	}
}

#endif // NullStream_h__
