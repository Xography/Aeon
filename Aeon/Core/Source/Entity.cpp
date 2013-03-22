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

#include <Aeon/Serialization/Entity.pb.h>

#include <Aeon/AttributeContainer.h>
#include <Aeon/ComponentDescriptor.h>
#include <Aeon/Entity.h>
#include <Aeon/EntityDescriptor.h>
#include <Aeon/Multicore/Types.h>
#include <Aeon/SystemMessages.h>
#include <Aeon/Types/UUID.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	Entity::Entity(const EntityID& id, const EntityDescriptor* descriptor):
		updateFlag(false),
		messageFlag(false),
		uniqueID(UUID(id.key)),
		descriptor(descriptor),
		entityID(id)
	{
		
	}

	// -------------------------------------------------------------------------
	Entity::Entity(const Entity& rhs):
		uniqueID(rhs.entityID.key),
		descriptor(rhs.descriptor),
		entityID(rhs.entityID)
	{

	}
	// -------------------------------------------------------------------------
	const UUID& Entity::GetUniqueID() const
	{
		return uniqueID;
	}

	// -------------------------------------------------------------------------
	const EntityID& Entity::GetID() const
	{
		return entityID;
	}

	// -------------------------------------------------------------------------
	void Entity::RemoveComponent(const ComponentID& id)
	{
		auto it = std::find_if(components.begin(), components.end(), [&id](const Component* data)
		{
			return data ? data->GetID()==id : false;
		});

		if(it != components.end())
		{
			LOG_DEBUG << "Removing component " << id << " from entity " << uniqueID;
			Component* ptr = *it;

			if(ptr)
			{
				ptr->Destroy();
				ptr->CleanupChannels();
			}

			delete ptr;

			components.erase(it);
		}
	}
	
	// -------------------------------------------------------------------------
	void Entity::AttributeUpdated(const AttributeKey& key)
	{
		if(!updateFlag)
			return;

		MessageAttributeUpdated* m = Message::Create<MessageAttributeUpdated>();

		LOG_DEBUG << GetUniqueID() << ": Attribute " << key << " updated. Notifying components.";

		m->SetAttribute(ATTR("Key"), key.hash);

		for(Components::iterator it = components.begin(); it != components.end(); ++it) 
		{
			if(*it)
				(*it)->OnMessage(m);
		}

		Message::Destroy(m);
	}

	// -------------------------------------------------------------------------
	void Entity::Destroy()
	{
		Components::iterator componentsIterator = components.begin();
		while(componentsIterator !=components.end()) 
		{
			LOG_DEBUG << "Destroying Component: " << (*componentsIterator)->GetUUID();
			Component* ptr = *componentsIterator;
			componentsIterator = components.erase(componentsIterator);

			if(ptr) 
			{
				ptr->Destroy();
				ptr->CleanupChannels();
			}

			delete ptr;
		}

		Attributes::iterator attributesIterator = attributes.begin();
		while(attributesIterator != attributes.end())
		{
			LOG_DEBUG << "Destroying Attribute: " << attributesIterator->first;
			IAttribute* ptr = attributesIterator->second;					
			attributesIterator = attributes.erase(attributesIterator);

			DestroyAttribute(ptr);
		}
	}

	// -------------------------------------------------------------------------
	void Entity::SetUpdateFlag( bool flag )
	{
		updateFlag = flag;
	}

	// -------------------------------------------------------------------------
	const Entity::Components & Entity::GetComponents() const
	{
		return components;
	}
	
	// -------------------------------------------------------------------------
	void Entity::AttachComponentInstance(Component* ptr)
	{
		if(!ptr)
		{
			LOG_WARNING << "Null pointer passed";
			return;
		}

		components.push_back(ptr);
	}
	
	// -------------------------------------------------------------------------
	Component* Entity::GetComponent(const ComponentID& id)
	{
		auto it = std::find_if(components.begin(), components.end(), [&id](const Component* data)
		{
			return data? data->GetID()==id : false;
		});

		if(it != components.end() && *it)
			return *it;
	
		LOG_WARNING << "Component " << id << " doesn't seem to be attached to " << entityID << " - returning 0!";
		return 0;
	}

	// -------------------------------------------------------------------------
	void Entity::OnSerialize(Serialize::EntityInstance* target) const
	{
		if(!target) return;
		
		UUID::Serialize(uniqueID, target->mutable_id());
		UUID::Serialize(descriptor ? descriptor->GetUUID() : UUID::Null(), target->mutable_entity_descriptor());

		Attributes::const_iterator valIt;
		for(valIt = GetAttributes().begin(); valIt != GetAttributes().end(); ++valIt)
		{
			Aeon::Serialize::AttributeInstance* instance =  target->add_values();
			instance->set_key(valIt->first.hash);
			IAttribute::SerializeValue(valIt->second, instance->mutable_value());
		}

		Components::const_iterator it;

		for(it = components.begin(); it != components.end(); ++it)
		{
			Component* component = *it;
			Aeon::Serialize::ComponentInstance* instance = target->add_components();
			UUID::Serialize(component->GetUUID(), instance->mutable_this_id());
			UUID::Serialize(component->GetDescriptorUUID(), instance->mutable_descriptor_id());
		}

	}

	// -------------------------------------------------------------------------
	bool Entity::HasComponent(const ComponentID& id)
	{
		auto it = std::find_if(components.begin(), components.end(), [&id](const Component* data)
		{
			return data ? data->GetID()==id : false;
		});

		return it != components.end();
	}

	// -------------------------------------------------------------------------
	const EntityDescriptor* Entity::GetDescriptor()
	{
		return descriptor;
	}

	// -------------------------------------------------------------------------
	void Entity::OnMessage(Message* msg)
	{
		if(!messageFlag)
			return;

		for(size_t i(0); i < components.size(); ++i)
		{
			if(!messageFlag)
				break;

			if(Component* ptr = components[i])
				ptr->OnMessage(msg);
		}
	}

	// -------------------------------------------------------------------------
	void Entity::StopMessageHandler()
	{
		messageFlag = false;
	}

	// -------------------------------------------------------------------------
	void Entity::StartMessageHandler()
	{
		messageFlag = true;
	}

}
