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
#include <algorithm>

#include <Aeon/MessageChannel.h>
#include <Aeon/IMessageHandler.h>


namespace Aeon {
	// -------------------------------------------------------------------------
	// unary comparator to find Pointers in vector
	struct PointerValueEquality
	{
		const IMessageHandler* toFind;

		bool operator()(const IMessageHandler* other) const
		{
			return (toFind == other);
		}
	};

	// -------------------------------------------------------------------------
	MessageChannel::MessageChannel(const ChannelID& id ):
		id(id)
	{

	}

	// -------------------------------------------------------------------------
	MessageChannel* MessageChannel::Create(const ChannelID& id)
	{
		return new MessageChannel(id);
	}

	// -------------------------------------------------------------------------
	const MessageChannel::ChannelID& MessageChannel::GetChannelId()
	{
		return id;
	}

	// -------------------------------------------------------------------------
	void MessageChannel::Subscribe(IMessageHandler* handler)
	{
		if(handler)
		{
			LOG_DEBUG << "MessageChannel("<< id << "): Subscribing instance " << &(*handler);
			sendLock.lock();
			targets.push_back(handler);
			sendLock.unlock();
		}

		else
			LOG_WARNING << "Attempted to subscribe null message handler to channel!";
	}

	// -------------------------------------------------------------------------
	void MessageChannel::Unsubscribe(IMessageHandler* handler)
	{
		if(handler)
		{
			LOG_DEBUG << "MessageChannel("<< id << "): Unsubscribing instance " << &(*handler);

			sendLock.lock();

			const PointerValueEquality pve = { handler };
			auto iter = std::find_if(targets.begin(), targets.end(), pve);

			if(iter != targets.end())
				targets.erase(iter);

			sendLock.unlock();
		}
	}

	// -------------------------------------------------------------------------
	void MessageChannel::Send(Message* m, Filter filter)
	{
		sendLock.lock();

		for(auto i(targets.begin()); i != targets.end(); ++i)
			if(!filter || (filter && filter(*i)))
				(*i)->OnMessage(m);

		sendLock.unlock();
	}
}
