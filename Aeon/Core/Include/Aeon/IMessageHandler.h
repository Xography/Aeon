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
#ifndef IMessageHandler_h__
#define IMessageHandler_h__

#include <Aeon/Export.h>
namespace Aeon 
{
	class Message;

	/**
	 * Implementing this interface makes the class capable of subscribing to message channels and to receive Messages.
	 * @see Message
	 * @see MessageChannel
	 */
	class AEON_EXPORT IMessageHandler
	{
	public:
		/**
		 * Constructor.
		 */
		IMessageHandler();

		/**
		 * Destructor.
		 */
		virtual ~IMessageHandler();
	public:
		/**
		 * Called whenever a message is received by the handler.
		 * 
		 * Please note that the message could come from another thread, so don't attempt to modify its data.
		 */
		virtual void OnMessage(Aeon::Message* message) = 0;
	};
}
#endif // IMessageHandler_h__
