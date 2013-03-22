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
#ifndef MessageDispatcher_h__
#define MessageDispatcher_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Singleton.h>
#include <Aeon/ProcessManager.h>
#include <Aeon/MessageChannel.h>
#include <Aeon/Export.h>

#include <deque>
#include <map>

#undef SendMessage
#undef SendMessageA

namespace Aeon {
	class Message;

	/**
	 * @brief Very basic central message dispatcher to send messages to specific channels.
	 */
	class AEON_EXPORT MessageDispatcher : public Singleton<MessageDispatcher>
	{
		friend class Singleton<MessageDispatcher>;

	private:
		/**
		 * Describes a message "packet", contains information that the dispatcher thread needs to properly dispatch messages.
		 */
		struct MessagePacket
		{
			MessageChannel::ChannelID id;
			MessageChannel::Filter filter;
			Message* m;
		};

	private:
		/**
		 * Constructor.
		 */
		MessageDispatcher();

	public:
		/**
		 * Shuts down the message dispatcher. This needs to be called before any subsystems are shut down or entities and components are being destroyed.
		 */
		void Shutdown();

	public:
		/**
		 * Creates a new Messaging Channel.
		 */
		void CreateChannel(const MessageChannel::ChannelID& name);

		/**
		 * Get Channel (or default channel if the requested channel does not exist)
		 */
		MessageChannel* GetChannel(const MessageChannel::ChannelID& name);

		/**
		 * Synchronously sends a message to a specific channel. The message will not be sent if the channel does not exist. The caller must
		 * take care of destroying the message when this function returns!
		 *
		 * @param name The name of the channel to send this message to.
		 * @param m The message to send to the channel.
		 * @param filter The filter to apply to the components on the channel. If not set, the message will be sent to all components subscribed to the channel. You can either
		 * use BIND to generate a filter or use C++11 lambda's.
		 */
		void SendMessage(const MessageChannel::ChannelID& name, Message* m, MessageChannel::Filter filter = NO_FILTER);

		/**
		 * Asynchronously sends a message to a specific channel. The message will not be sent if the channel does not exist. This function
		 * will immediately return, regardless if the message is received or not. No assumptions should be made on whether the message
		 * was received or when it's being sent. It will be placed in a queue and handled as soon as possible.
		 *
		 * The destruction of this message will be handled by this function, do not in any case destroy the message after this function
		 * returns!
		 *
		 * @param name The name of the channel to send this message to.
		 * @param m The message to send to the channel.
		 * @param filter The filter to apply to the components on the channel. If not set, the message will be sent to all components subscribed to the channel. You can either
		 * use BIND to generate a filter or use C++11 lambda's.
		 */
		void SendMessageAsync(const MessageChannel::ChannelID& name, Message* m, MessageChannel::Filter filter = NO_FILTER);

	private:
		/**
		 * Thread used for dispatching asynchronous messages.
		 */
		void DispatcherThread();

	private:
		/**
		 * Maps channel ID's to the actual channels.
		 */
		std::map<MessageChannel::ChannelID, MessageChannel*> channels;

		/**
		 * Queue of messages waiting to be processed. Only used for asynchronous messaging.
		 */
		std::deque<MessagePacket> messageQueue;

		/**
		 * Mutex used to ensure thread-safe access to the messaging queue.
		 */
		Mutex queueMutex;

		/**
		 * The default channel to dispatch messages that don't have anywhere else to go to to.
		 */
		MessageChannel* defaultChannel;

		/**
		 * The ID of the dispatcher thread.
		 */
		Aeon::ProcessManager::ThreadID dispatcherThreadID;
	};
}
#endif // MessageDispatcher_h__
