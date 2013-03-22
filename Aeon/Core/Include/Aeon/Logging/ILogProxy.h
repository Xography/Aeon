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
#ifndef ILogProxy_h__
#define ILogProxy_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Logging/Logging.h>

namespace Aeon {
	/**
	 * A logging proxy, messages are being redirected to this handler instead of being logged to a file if it is registered with a Logger.
	 */
	class ILogProxy
	{
	public:
		/**
		 * Called whenever a message is being logged somewhere. 
		 */
		virtual void OnMessageLogged(const String& message, int severity) = 0;
	};
}

#endif // ILogProxy_h__
