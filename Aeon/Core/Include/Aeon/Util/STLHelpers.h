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
#ifndef STLHelpers_h__
#define STLHelpers_h__

#include <Aeon/AeonTypes.h>

namespace Aeon {
	namespace Util {
		/**
		 * Converts the specified 'str' to an integer.
		 *
		 * @param str The string to convert.
		 * @param succeeded If this pointer is set, a boolean indicating if the conversion succeeded will be written to the pointer.
		 *
		 * @return The converted integer, or an undefined value if the conversion fails.
		 */
		static inline int StringToInt(const String& str, bool *succeeded = nullptr)
		{
			int x(0);
			std::istringstream s(str);

			s >> x;

			if(succeeded)
				(*succeeded) = (!s.fail() && !s.bad());

			return x;
		}
		
		/**
		 * Converts the specified integer to a string.
		 */
		static inline String IntToString(int i)
		{
			std::ostringstream s;
			s << i;
			return s.str();
		}
	}
}

#endif // STLHelpers_h__
