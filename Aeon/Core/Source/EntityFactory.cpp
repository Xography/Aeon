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

#include <Aeon/Serialization/EntityDB.pb.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/EntityFactory.h>
#include <Aeon/ComponentRegistry.h>

namespace Aeon {
	static std::vector<Entity*> entities;

	// -------------------------------------------------------------------------
	// unary comparator to find Pointers in vector
	struct PointerValueEquality
	{
		const Entity* toFind;

		bool operator()(const Entity* other) const
		{
			return &*toFind == &*other;
		}
	};

	
	// -------------------------------------------------------------------------
	EntityFactory::EntityFactory():
		database(0)
	{

	}

	// -------------------------------------------------------------------------
	Entity* EntityFactory::CreateEntity(const String& key)
	{
		const UUID& uuid = database->LookupEntityUUID(EntityID(key));

		if(uuid == UUID::Null())
		{
			LOG_WARNING << "Cannot find entity descriptor \"" << key << " in database. Returning empty entity.";
			return DefaultInstance(key);
		}

		return CreateEntity(uuid);
	}

	// -------------------------------------------------------------------------
	Entity* EntityFactory::CreateEntity(const UUID& key)
	{
		const EntityDescriptor* tpl = database->GetEntityDescriptor(key);

		if(!tpl)
		{
			LOG_WARNING << "Entity Descriptor for " << key << " does not exist in database. Returning empty entity.";

			// Mark entity as "invalid" in its name, so it can be found quite easily
			return DefaultInstance("UnitializedEntity");
		}


		LOG_DEBUG << "Creating Entity: " << tpl->GetID();

		Entity* ent = new Entity(tpl->GetID(), tpl);

		LOG_DEBUG << "UUID: " << ent->GetUniqueID();

		// Assign Attributes
		const Attributes& attributes = tpl->GetAttributes();
		for(Attributes::const_iterator i = attributes.begin(); i != attributes.end(); ++i)
		{
			LOG_DEBUG << "\tAssigning attribute: " << i->first;
			ent->AssignCopy(i->second);
		}

		// Assign Components
		const Vector<ComponentDescriptor*>::type& components = tpl->GetComponents();
		for(Vector<ComponentDescriptor*>::type::const_iterator i = components.begin(); i != components.end(); ++i)
		{
			LOG_DEBUG << "\tAssigning component: " << *i;
			ent->AttachComponentInstance(database->GetComponentRegistry()->CreateComponent((*i)->GetUUID(), ent));
		}

		entities.push_back(ent);

		// Mark entity as ready, so it sends messages to its attached components in case an attribute is changed. 
		ent->SetUpdateFlag(true);
		ent->StartMessageHandler();

		return ent;
	}

	// -------------------------------------------------------------------------
	Entity* EntityFactory::CreateEntity(const Serialize::EntityInstance& data)
	{
		if(!data.IsInitialized())
		{
			LOG_WARNING << "Attempted to create entity to from uninitialized data!";
			return DefaultInstance("UnitializedEntity");
		}

		const EntityDescriptor* tpl = database->GetEntityDescriptor(UUID(data.entity_descriptor()));
		Entity* ent = new Entity(tpl->GetID(), tpl);

		// Assign attributes with default value
		Attributes::const_iterator it = tpl->GetAttributes().begin();
	
		for(; it != tpl->GetAttributes().end(); ++it)
		{
			bool found = false;
			// Check whether this value was serialized
			for(int i = 0; i < data.values_size(); i++)
			{
				const Serialize::AttributeInstance& instance = data.values(i);
				if(it->first.hash == instance.key())
				{
					ent->AssignAttribute(IAttribute::DeserializeAttribute(it->first, instance.value(), ent));
					found = true;
					break;
				}
			}

			// Else assign the default value.
			if(!found)
			{
				ent->AssignCopy(it->second);
			}
		}
			
		// Assign components by looking up their IDs
		const Vector<ComponentDescriptor*>::type& comps = tpl->GetComponents();
		for(Vector<ComponentDescriptor*>::type::const_iterator compIt = comps.begin(); compIt != comps.end(); ++compIt)
		{
			bool found = false;
			for(int i = 0; i < data.components_size(); i++)
			{
				if(UUID(data.components(i).descriptor_id())==(*compIt)->GetUUID())
				{
					ent->AttachComponentInstance(database->GetComponentRegistry()->CreateComponent(data.components(i), ent));
					found = true;
					break;
				}
			}

			if(!found)
			{
				ent->AttachComponentInstance(database->GetComponentRegistry()->CreateComponent((*compIt)->GetUUID(),ent));
			}
		}

		ent->SetUpdateFlag(true);
		ent->StartMessageHandler();

		return ent;
	}

	// -------------------------------------------------------------------------
	Entity* EntityFactory::New(const String& identifier)
	{
		return new Entity(EntityID(identifier), 0);
	}

	// -------------------------------------------------------------------------
	void EntityFactory::DestroyEntity(Entity* ptr)
	{
		if(!ptr)
			return;
		
		const PointerValueEquality pve = {ptr};
		std::vector<Entity*>::iterator it = std::find_if(entities.begin(), entities.end(),pve);

		if(it != entities.end())
			it = entities.erase(it);

		ptr->Destroy();
		delete ptr;
	}
	
	// -------------------------------------------------------------------------
	void EntityFactory::SetDatabase( EntityDatabase* database )
	{
		this->database = database;
	}

	// -------------------------------------------------------------------------
	void EntityFactory::DestroyAll()
	{
		std::vector<Entity*>::iterator it = entities.begin();
		while(it != entities.end())
		{
			Entity* ptr = *it;
			it = entities.erase(it);
			ptr->Destroy();
			delete ptr;
		}
	}

	// -------------------------------------------------------------------------
	const Vector<Entity*>::type& EntityFactory::GetInstancedEntities() const
	{
		return entities;
	}

	// -------------------------------------------------------------------------
	void EntityFactory::Shutdown()
	{
		if(!entities.empty())
		{
			LOG_INFO << "Shutting down Entity Factory. Destroying all remaining instances.";
			DestroyAll();
		}
	}

	// -------------------------------------------------------------------------
	Entity* EntityFactory::DefaultInstance(const String& id)
	{
		Entity* ent = new Entity(EntityID("INVALID-"+id), 0);
		entities.push_back(ent);
		return ent;
	}

}
