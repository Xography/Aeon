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
#include <Aeon/Message.h>
#include <Aeon/MessageDispatcher.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	void MessageDispatcher::DispatcherThread()
	{
		THREAD_BEGIN_LOOP
		{
			if(messageQueue.empty())
			{
				THREAD_YIELD;
				continue;
			}

			queueMutex.lock();

			for(auto i(messageQueue.begin()); i != messageQueue.end(); ++i)
			{
				if(channels[i->id])
				{
					LOG_DEBUG << "Sent an asynchronous message to channel " << i->id << "...";
					channels[i->id]->Send(i->m, i->filter);
				}
		
				else
					LOG_ERROR << "Channel " << i->id << " does not exist. Asynchronous message not sent.";

				Aeon::Message::Destroy(i->m);
			}

			messageQueue.clear();

			queueMutex.unlock();	
		}
		THREAD_END_LOOP
	}
	
	// -------------------------------------------------------------------------
	MessageDispatcher::MessageDispatcher():
		defaultChannel(MessageChannel::Create("Default"))
	{
		channels["Default"] = defaultChannel;

		LOG_DEBUG << "Starting asynchronous message dispatcher thread...";

		dispatcherThreadID = ProcessManager::Instance().Create("AsyncMessageDispatcher", BIND(&MessageDispatcher::DispatcherThread, this));
	}

	// -------------------------------------------------------------------------
	void MessageDispatcher::Shutdown()
	{
		LOG_DEBUG << "Stopping asynchronous message dispatcher thread...";

		ProcessManager::Instance().Stop(dispatcherThreadID);
	}

	// -------------------------------------------------------------------------
	void MessageDispatcher::CreateChannel(const MessageChannel::ChannelID& name)
	{
		queueMutex.lock();
		channels[name] = MessageChannel::Create(name);
		queueMutex.unlock();
	}

	// -------------------------------------------------------------------------
	MessageChannel* MessageDispatcher::GetChannel(const MessageChannel::ChannelID& name)
	{
		if(auto channel = channels[name])
			return channel;

		return defaultChannel;
	}

	// -------------------------------------------------------------------------
	void MessageDispatcher::SendMessage(const MessageChannel::ChannelID& name, Message* m, MessageChannel::Filter filter)
	{
		if(auto channel = channels[name])
			channel->Send(m, filter);
		
		else
			LOG_ERROR << "Channel " << name << " does not exist. Synchronous message not sent.";
	}

	// -------------------------------------------------------------------------
	void MessageDispatcher::SendMessageAsync(const MessageChannel::ChannelID& name, Message* m,  MessageChannel::Filter filter)
	{
		queueMutex.lock();
		MessagePacket packet = {name, filter, m};
		messageQueue.push_back(packet);
		queueMutex.unlock();
	}
}
