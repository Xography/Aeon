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
#ifndef MessageChannel_h__
#define MessageChannel_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Multicore/Types.h>
#include <Aeon/Export.h>
#include <Aeon/Message.h>

namespace Aeon {
	class IMessageHandler;

	#ifdef AEON_USE_BOOST_THREADS
		/**
		 * Acts as a filter that literally is "no filter at all". This is needed because boost does not support
		 * assigning nullptr or null to a boost::function object like std::function does.
		 */
		static bool NoFilterWrapper(const IMessageHandler*) { return true; }
	#	define NO_FILTER	&NoFilterWrapper
	#else
	#	define NO_FILTER	nullptr
	#endif // AEON_USE_BOOST_THREADS

	/**
	 *  @brief Class representing a very basic Message channel to which components can subscribe to.
	 */
	class AEON_EXPORT MessageChannel
	{
	public:
		typedef String ChannelID;
		typedef FUNCTION<bool(const IMessageHandler*)> Filter;

	private:
		/**
		 * Constructor.
		 */
		MessageChannel(const ChannelID& id);

	public:
		/**
		 * Creates a message channel with the given ID.
		 */
		static MessageChannel* Create(const ChannelID& id);

		/**
		 * Get the ID of the channel.
		 */
		const MessageChannel::ChannelID& GetChannelId();

		/**
		 * Subscribe a component to this channel. Don't use this directly, use Component::Subscribe instead.
		 */
		void Subscribe(IMessageHandler* comp);

		/**
		 * Unsubscribe a component from this channel. Don't use this directly, use Component::Unsubscribe instead.
		 */
		void Unsubscribe(IMessageHandler* comp);
		
		/**
		 * Send a message to all subscribers of this channel.
		 */
		void Send(Aeon::Message* m, Filter filter = NO_FILTER);

	private:
		/**
		 * The ID of this message channel.
		 */
		ChannelID id;

		/**
		 * Mutex used to provide thread-safe channel message sending whilst still allowing (un)subscribing.
		 */
		Mutex sendLock;

		/**
		 * The components that are subscribed to this channel.
		 */
		std::vector<IMessageHandler*> targets;
	};
}

#endif // MessageChannel_h__
