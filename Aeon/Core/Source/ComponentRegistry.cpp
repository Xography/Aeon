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
#include <Aeon/Serialization/Component.pb.h>

#include <algorithm>
#include <Aeon/ComponentRegistry.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	// A Null component, returned whenever a component does not exist.
	// This is done to avoid having "null" pointers when creating an entity.
	class NullComponent : public Component
	{
	public:
		NullComponent(const String& id):
			Component(ComponentID(id))
		{
		}

		NullComponent(const String& id, const UUID& uuid):
			Component(ComponentID(id))
		{
			SetDescriptorUUID(uuid);
		}

		void OnUpdate(float /*delta*/)
		{
			
		}

		void OnMessage(Message* m)
		{
			LOG_DEBUG << "NullComponent: Received Message of type " << m->GetType();
		}

		Component* Create(Entity* /*parent*/)
		{
			return new NullComponent(GetID().key, parentUUID);
		}

		void Destroy() 
		{

		}

		void OnCreateComponentInstance() 
		{

		}

		void OnInitialise(Entity* parent) 
		{

		}

	};
	
	// -------------------------------------------------------------------------
	ComponentRegistry::ComponentRegistry()
	{

	}

	// -------------------------------------------------------------------------
	void ComponentRegistry::Initialise(/*const String& componentDatabase*/)
	{
		LOG_DEBUG << "Initializing Component Registry";
		// For later: maybe load scripted components :)
	}

	// -------------------------------------------------------------------------
	void ComponentRegistry::Shutdown()
	{
		if(comps.size() > 0)
		{
			LOG_INFO << "Shutting down Component Registry. Destroying all registered templates.";
			ComponentsMap::iterator it = comps.begin();
			while(it != comps.end()) 
			{
				Component* ptr = it->second;
				
				it = comps.erase(it);

				// This function only iterates the component templates, they don't need to shutdown.
				if(ptr)
				{
					ptr->OnUnregisterTemplate();
				} else continue;

				delete ptr;
			}

		}

		comps.clear();
	}

	// -------------------------------------------------------------------------
	void ComponentRegistry::RegisterComponent(Component* component)
	{
		if(!component) 
		{
			LOG_WARNING << "Attempt to register null pointer at component registry.";
			return;
		}

		LOG_DEBUG << "Registering Component: " << component->GetID();
		
		// We need to look up the UUID of the descriptor and wire it to this instance.
		const UUID& uuid = EntityDatabase::Instance().LookupComponentUUID(component->GetID());

		if(uuid == UUID::Null())
		{
			LOG_WARNING << "Attempt to register component " << component->GetID() << " without valid descriptor! "
						<< "Component will be registered, but can't be looked up via the database.";
			comps[component->GetUUID()] = component;
			LOG_WARNING << "Registered "  << component->GetID() << " with UUID " << component->GetUUID(); 
			return;
		} 

		component->SetDescriptorUUID(uuid);
		comps[uuid] = component;
		component->OnRegisterAsTemplate();
	}

	// -------------------------------------------------------------------------
	Component* ComponentRegistry::CreateComponent(const String& id, Entity* parent)
	{
		ComponentID cID(id);
		ComponentsMap::iterator it = std::find_if(comps.begin(), comps.end(), [&cID](const std::pair<const UUID, Component*>& data)
		{ return data.second->GetID()==cID;});

		if(it != comps.end())
		{
			Component* ptr = it->second->Create(parent);
			ptr->SetDescriptorUUID(it->first);
			return ptr;
		}

		LOG_WARNING << "Component \"" << id << "\" does not exist. Returning null object instead.";

		return new NullComponent(id);
	}

	// -------------------------------------------------------------------------
	Component* ComponentRegistry::CreateComponent(const UUID& id, Entity* parent)
	{
		ComponentsMap::iterator it = comps.find(id);

		if(it != comps.end())
		{
			Component* ptr = it->second->Create(parent);
			ptr->SetDescriptorUUID(it->first);
			return ptr;
		}

		ComponentDescriptor* descriptor = EntityDatabase::Instance().GetComponentDescriptor(id);
		if(descriptor) {
			Component* nullComp = new NullComponent(descriptor->GetComponentID().key);
			nullComp->SetDescriptorUUID(id);
			return nullComp;
		}
		else return new NullComponent("NULL", id);
	}

	// -------------------------------------------------------------------------
	Component* ComponentRegistry::CreateComponent(const Serialize::ComponentInstance& instance, Entity* parent)
	{
		Component* ptr = CreateComponent(UUID(instance.descriptor_id()), parent);
		ptr->SetUUID(UUID(instance.this_id()));
		return ptr;
	}

	// -------------------------------------------------------------------------
	std::vector<ComponentID> ComponentRegistry::GetComponentNameList()
	{
		std::vector<ComponentID> componentNames;
		for(ComponentsMap::iterator it = comps.begin(); 
			it != comps.end(); ++it) 
		{
			componentNames.push_back(it->second->GetID());
		}

		return componentNames;
	}
}