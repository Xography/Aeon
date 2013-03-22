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
#ifndef XMLString_h__
#define XMLString_h__

#include <string>
#include <Aeon/XML/Export.h>

namespace Aeon { namespace XML {
	typedef std::string XMLString;

	namespace XMLUtility {
		/**
		 * Comparison function for XML strings.
		 */
		inline bool Equals(const XMLString& lhs, const XMLString& rhs)
		{
			return (rhs.compare(lhs) == 0);
		}
	}
}}

#endif // XMLString_h__
