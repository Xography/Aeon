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
#include <Aeon/Serialization/EntityDB.pb.h>

#include <Aeon/EntityCategory.h>

#include <Aeon/Types/UUID.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	static EntityCategory* defaultCat = EntityCategory::Create("General");

	// -------------------------------------------------------------------------
	EntityCategory::EntityCategory(const String& name) :
		name(name),
		id(UUID(name))
	{

	}

	// -------------------------------------------------------------------------
	EntityCategory::EntityCategory(const String& name, const UUID& id) :
		name(name),
		id(id)
	{

	}
	// -------------------------------------------------------------------------
	EntityCategory* EntityCategory::Create(const String& name)
	{
		return new EntityCategory(name);
	}
	
	// -------------------------------------------------------------------------
	EntityCategory* EntityCategory::Create(const Serialize::CategoryDescriptor& data)
	{
		return new EntityCategory(data.name(), UUID(data.id()));
	}

	// -------------------------------------------------------------------------
	void EntityCategory::Destroy(EntityCategory* ptr)
	{
		if(ptr == defaultCat) return;
		delete ptr;
	}

	// -------------------------------------------------------------------------
	EntityCategory* EntityCategory::GetDefaultInstance()
	{
		return defaultCat;
	}

	// -------------------------------------------------------------------------
	const String& EntityCategory::GetName() const
	{
		return name;
	}

	// -------------------------------------------------------------------------
	const UUID& EntityCategory::GetID() const
	{
		return id;
	}

	// -------------------------------------------------------------------------
	void EntityCategory::SetName(const String& name)
	{
		this->name = name;
	}

}
