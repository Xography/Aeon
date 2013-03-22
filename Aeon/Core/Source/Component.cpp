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
#include <Aeon/Component.h>
#include <Aeon/ComponentDescriptor.h>
#include <Aeon/MessageDispatcher.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	Component::Component(const ComponentID& id):
		parent(nullptr),
		id(id), 
		uuid(UUID(id.key)),
		parentUUID(UUID::Null())
	{

	}

	// -------------------------------------------------------------------------
	Component::Component(const ComponentID& id, Entity* parent):
		parent(parent),
		id(id),
		uuid(UUID(id.key)),
		parentUUID(UUID::Null())
	{
		
	}
		
	// -------------------------------------------------------------------------
	Component::Component(const ComponentID& id, Entity* parent, const UUID& uuid):
		parent(parent),
		id(id),
		uuid(uuid),
		parentUUID(UUID::Null())
	{

	}

	// -------------------------------------------------------------------------
	Component::~Component()
	{
		
	}

	// -------------------------------------------------------------------------
	const ComponentID& Component::GetID() const
	{
		return id;
	};

	// -------------------------------------------------------------------------
	void Component::SetComponentID(const ComponentID& id)
	{
		this->id = id;
	}

	// -------------------------------------------------------------------------
	const UUID& Component::GetUUID() const
	{
		return uuid;
	}

	// -------------------------------------------------------------------------
	Entity* Component::GetParent() const
	{
		return parent;
	}


	// -------------------------------------------------------------------------
	void Component::SetParent(Entity* ent)
	{
		parent = ent;
	}

	// -------------------------------------------------------------------------
	void Component::CleanupChannels()
	{
		// Automatically unsubscribe from all channels. Do *NOT* move this to the destructor! If an async message is being sent to a channel
		// this component is subscribed to, this can cause undefined behaviour! Even though async messaging and subscriptions are locked,
		// if the message gets precedence over the Unsubscribe call, it will attempt to call OnMessage(), which is a virtual function and
		// can cause a pure virtual function call (because the destructor has already been entered)!
		for(size_t i(0); i < subscriptions.size(); ++i)
			this->Unsubscribe(subscriptions[i]);

		subscriptions.clear();
	}

	// -------------------------------------------------------------------------
	void Component::Subscribe(const MessageChannel::ChannelID& id)
	{
		if(std::find(subscriptions.begin(), subscriptions.end(), id) == subscriptions.end())
		{
			MessageDispatcher::Instance().GetChannel(id)->Subscribe(this);
			subscriptions.push_back(id);
		}

		else
			LOG_WARNING << "Component " << id << "(UUID " << uuid << ") Attempted to subscribe twice to channel " << id << "!";
	}

	// -------------------------------------------------------------------------
	void Component::Unsubscribe(const MessageChannel::ChannelID& id)
	{
		MessageDispatcher::Instance().GetChannel(id)->Unsubscribe(this);
	}

	// -------------------------------------------------------------------------
	const UUID& Component::GetDescriptorUUID()
	{
		return parentUUID;
	}

	// -------------------------------------------------------------------------
	void Component::SetDescriptorUUID(const UUID& parent)
	{
		this->parentUUID = parent;
	}

	// -------------------------------------------------------------------------
	void Component::SetUUID(const UUID& thisUUID)
	{
		uuid = thisUUID;
	}

}
