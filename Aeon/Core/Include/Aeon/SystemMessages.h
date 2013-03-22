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
#ifndef SystemMessages_h__
#define SystemMessages_h__

#include <Aeon/Export.h>
#include <Aeon/Message.h>

#undef SendMessage
#undef SendMessageA

namespace Aeon {
	namespace Messages {
		// move me
	}

	/**
	 *  @brief A message sent by the render system when the render window has been created.
	 */
	BEGIN_DECLARE_MESSAGE_EXTERNAL(MessageAttributeUpdated, AEON_EXPORT)
	{
		DECLARE_ATTRIBUTE(int32_t, "Key")
	}
	END_DECLARE_MESSAGE()
	
	/**
	 *  @brief A message sent by the render system when the render window has been created.
	 */
	BEGIN_DECLARE_MESSAGE_EXTERNAL(MessageRenderWindowCreated, AEON_EXPORT)
	{
		DECLARE_ATTRIBUTE(int64_t, "WindowHandle")
	}
	END_DECLARE_MESSAGE()
}

#endif // SystemMessages_h__
