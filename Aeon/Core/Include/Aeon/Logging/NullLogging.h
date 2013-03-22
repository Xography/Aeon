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
#ifndef NullLogging_h__
#define NullLogging_h__

#include <Aeon/Logging/ILogging.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 *  @brief Null implementation of the Logging subsystem.
	 */
	class AEON_EXPORT NullLogging : public ILogging
	{
	public:
		/**
		 * Overrides for ILogging functions.
		 *
		 * @see ILogging
		 */
		void SetLogLevel(int /*level*/) override { }
		void LogMessage(const String& /*msg*/, int /*severity*/) override { }
		void Shutdown() override {}
	};
}

#endif // NullLogging_h__
