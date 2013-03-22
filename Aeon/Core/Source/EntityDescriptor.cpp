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
#include <Aeon/EntityDescriptor.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/ComponentDescriptor.h>
#include <Aeon/Types/UUID.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	EntityDescriptor::EntityDescriptor(const EntityID& id):
		id(id),
		uuid(UUID(id.key)),
		category(EntityCategory::GetDefaultInstance())
	{

	}

	// -------------------------------------------------------------------------
	EntityDescriptor::EntityDescriptor(const EntityID& id, const UUID& uuid):
		id(id),
		uuid(uuid),
		category(EntityCategory::GetDefaultInstance())
	{

	}

	// -------------------------------------------------------------------------
	EntityDescriptor* EntityDescriptor::Create(const String& name)
	{
		return new EntityDescriptor(EntityID(name));
	}

	// -------------------------------------------------------------------------
	EntityDescriptor* EntityDescriptor::Create(const Serialize::EntityDescriptor& serializedData)
	{
		EntityDescriptor* entityDescriptor;

		if(!serializedData.id().lo() && !serializedData.id().hi())
		{	
			LOG_WARNING << "[Backwards compatibility] Entity Descriptor " << serializedData.name() << " does not have an UUID yet, generating this.";
			entityDescriptor = new EntityDescriptor(EntityID(serializedData.name()), UUID(serializedData.name()));
		}
		else entityDescriptor = new EntityDescriptor(EntityID(serializedData.name()), UUID(serializedData.id()));

		LOG_DEBUG << "\t Reading " << serializedData.attributes_size() << " attributes."; 

		// Assign attributes
		for(auto itAttribute = serializedData.attributes().begin(); itAttribute !=  serializedData.attributes().end(); ++itAttribute)
		{
			const Serialize::Attribute& attrib = *itAttribute;

			// We're delibarately setting the parent entity to 0, since its just a descriptor.
			IAttribute* attr = IAttribute::DeserializeAttribute(AttributeKey(attrib.key().name()), attrib.value(), 0);

			if(!attr || !attr->GetType())
			{
				LOG_DEBUG << "Invalid attribute in entity descriptor " << entityDescriptor->GetID();
				continue;
			}

			entityDescriptor->AssignAttribute(attr);
		}
	
		return entityDescriptor;
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::SetCategory(EntityCategory* cat)
	{
		category = cat;
	}

	// -------------------------------------------------------------------------
	const EntityCategory* EntityDescriptor::GetCategory() const
	{
		return category;
	}

	// -------------------------------------------------------------------------
	bool EntityDescriptor::HasComponent(const UUID& id)
	{
		return std::find_if(components.begin(),components.end(),[&id](ComponentDescriptor* ptr)
		{ 
			return ptr->GetUUID() == id;
		}) 
		!= components.end();
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::RemoveComponent(const UUID& id)
	{
		auto iter = std::find_if(components.begin(),components.end(),[&id](ComponentDescriptor* ptr)
			{ 
				return ptr->GetUUID() == id;
			}
		);
		if(iter == components.end())
			return;
		
		ComponentDescriptor* component = *iter;
		components.erase(iter);

		auto it = attributes.begin(); 
		while(it != attributes.end())
		{
			if(it->first.key.find(component->GetComponentID().key) != String::npos)
			{
				IAttribute* ptr = it->second;
				it = attributes.erase(it);
				DestroyAttribute(ptr);
			} 
			else
			{
				++it;
			}
		}
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::AddComponent(ComponentDescriptor* comp)
	{
		if(!comp)
		{
			LOG_WARNING << "Attempting to attach null component to " << this->id;
			return;
		}

		if(HasComponent(comp->GetUUID()))
		{
			LOG_INFO << "Entity descriptor " << this->GetName() << " already has component " << comp->GetComponentID() << ", skipping.";
			return;
		}

		const ComponentDescriptor::Parameters& params = comp->GetParameters();
		const String& name = comp->GetComponentID().key;
		
		for(auto it = params.begin(); it != params.end(); ++it)
		{
			const AttributeKey attName = AttributeKey(name+"."+it->first); 
			
			// If we already have that attribute set, skip.
			if(CheckAttribute(attName)) continue;

			switch(it->second)
			{
			case IAttribute::BLOB:
			{
				AddAttribute<Blob>(attName);
				break;
			}
			case IAttribute::BOOL:
			{
				AddAttribute<bool>(attName);
				break;
			}
			case IAttribute::DOUBLE:
			{
				AddAttribute<double>(attName);
				break;
			}
			case IAttribute::FLOAT:
			{
				AddAttribute<float>(attName);
				break;
			}
			case IAttribute::INT32:
			{
				AddAttribute<int32_t>(attName);
				break;
			}
			case IAttribute::INT64:
			{
				AddAttribute<int64_t>(attName);
				break;
			}

			case IAttribute::MATRIX4X4:
			{
				AddAttribute<Matrix4x4>(attName);
				break;
			}
			case IAttribute::QUATERNION:
			{
				AddAttribute<Quaternion>(attName);
				break;
			}
			case IAttribute::RANGE:
			{
				AddAttribute<Range>(attName);
				break;
			}
			case IAttribute::STRING:
			{
				AddAttribute<String>(attName);
				break;
			}
			case IAttribute::VECTOR2:
			{
				AddAttribute<Vector2>(attName);
				break;
			}
			case IAttribute::VECTOR3:
			{
				AddAttribute<Vector3>(attName);
				break;
			}
			case IAttribute::VECTOR4:
			{
				AddAttribute<Vector4>(attName);
				break;
			}
			default:
				break;
			}
		}
		
		components.push_back(comp);
	}

	// -------------------------------------------------------------------------
	const Vector<ComponentDescriptor*>::type& EntityDescriptor::GetComponents() const
	{
		return components;
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::AttributeUpdated(const AttributeKey& /*key*/)
	{
		// No need to handle this callback in a descriptor.
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::Destroy(EntityDescriptor* ptr)
	{
		if(!ptr) return;
		
		ptr->ClearComponents();
		ptr->ClearAttributes();
		delete ptr;
	}

	// -------------------------------------------------------------------------
	const EntityID& EntityDescriptor::GetID() const
	{
		return id;
	}

	// -------------------------------------------------------------------------
	const String& EntityDescriptor::GetName() const
	{
		return id.key;
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::SetName(const String& id)
	{
		this->id = EntityID(id);
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::RemoveAttribute(const AttributeKey& key)
	{
		auto it = attributes.find(key);

		if(it != attributes.end())
		{
			IAttribute* ptr = it->second;
			attributes.erase(it);
			DestroyAttribute(ptr);
		}
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::AssignAttributes(const Attributes& /*attributeMap*/)
	{
		// Copy Map
	}

	// -------------------------------------------------------------------------
	void EntityDescriptor::ClearComponents()
	{
		components.clear();
	}

	// -------------------------------------------------------------------------
	const UUID& EntityDescriptor::GetUUID() const
	{
		return uuid;
	}

}
