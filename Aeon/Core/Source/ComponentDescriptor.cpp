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
#include <Aeon/ComponentDescriptor.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	ComponentDescriptor::ComponentDescriptor(const String& componentName):
		componentID(ComponentID(componentName)),
		uuid(UUID(componentName))
	{

	}

	// -------------------------------------------------------------------------
	ComponentDescriptor::ComponentDescriptor(const String& componentName, const UUID& uuid):
		componentID(ComponentID(componentName)),
		uuid(uuid)
	{

	}

	// -------------------------------------------------------------------------
	const ComponentID& ComponentDescriptor::GetComponentID()
	{
		return componentID;
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::SetParameter(const String& name, IAttribute::Type type)
	{
		parameters[name] = type;
	}

	// -------------------------------------------------------------------------
	const ComponentDescriptor::Parameters& ComponentDescriptor::GetParameters() const
	{
		return parameters;
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::RemoveParameter(const String& name)
	{
		auto it = parameters.find(name);
		if(it != parameters.end())
			parameters.erase(it);
		else
			LOG_WARNING << "Attempting to remove non-existent parameter \"" << name << "\" from component descriptor \"" << this->componentID << "\"!";
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::ClearParameters()
	{
		parameters.clear();
	}

	// -------------------------------------------------------------------------
	ComponentDescriptor* ComponentDescriptor::Create(const String& name)
	{
		return new ComponentDescriptor(name);
	}

	// -------------------------------------------------------------------------
	ComponentDescriptor* ComponentDescriptor::Create(const Serialize::ComponentDescriptor& serializedData)
	{
		LOG_DEBUG << "Loading Component Descriptor: " << serializedData.name();

		ComponentDescriptor* component = new ComponentDescriptor(serializedData.name(), UUID(serializedData.id()));

		LOG_DEBUG << "\t Reading " << serializedData.parameters_size() << " parameters."; 
		for(auto paramIter = serializedData.parameters().begin(); paramIter != serializedData.parameters().end(); ++paramIter)
		{
			const Serialize::ComponentDescriptor_Parameter& param = *paramIter;
			component->SetParameter(param.key(),(IAttribute::Type)param.type());
		}

		return component;
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::Destroy(ComponentDescriptor* ptr)
	{
		if(!ptr) return;

		// The map contains only atomic types
		ptr->ClearParameters();
		delete ptr;
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::SetName(const String& name)
	{
		this->componentID = ComponentID(name);
	}

	// -------------------------------------------------------------------------
	IAttribute::Type ComponentDescriptor::GetParameter(const String& key)
	{
		return parameters[key];
	}

	// -------------------------------------------------------------------------
	const String& ComponentDescriptor::GetDescription()
	{
		return description;
	}

	// -------------------------------------------------------------------------
	void ComponentDescriptor::SetDescription(const String& description)
	{
		this->description = description;
	}

	// -------------------------------------------------------------------------
	bool ComponentDescriptor::HasParamater(const String& key)
	{
		return parameters.find(key) != parameters.end();
	}

	// -------------------------------------------------------------------------
	const UUID& ComponentDescriptor::GetUUID()
	{
		return uuid;
	}

}
