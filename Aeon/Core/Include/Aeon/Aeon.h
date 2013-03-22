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
#ifndef Aeon_h__
#define Aeon_h__

#include <Aeon/Platform/Platform.h>
#include <Aeon/Export.h>
#include <Aeon/Logging/Logging.h>

namespace Aeon {
	/**
	 * Shuts down the Aeon framework. Shuts down the message dispatcher, destroys all the entities, destroys the subsystems, ...
	 */
	void AEON_EXPORT Shutdown();
}

#endif // Aeon_h__
