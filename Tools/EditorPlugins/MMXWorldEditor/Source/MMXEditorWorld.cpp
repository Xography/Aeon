/*******************************************************************************
 *
 * This file is part of Sample Game.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 *
 * See LICENSE.txt for licensing details.
 *
 ******************************************************************************/
 #include <Aeon/Serialization/World.pb.h>
 #include "AeonEditorWorld.h"

#include "AeonWorldEditorWidget.h"

#include <Aeon/AeonTypes.h>

// -------------------------------------------------------------------------
AeonEditorWorld::AeonEditorWorld():
	worldEditor(nullptr)
{

}

// -------------------------------------------------------------------------
void AeonEditorWorld::OnUpdate(float /*delta*/)
{

}

// -------------------------------------------------------------------------
void AeonEditorWorld::OnSerializeEntities(Aeon::Serialize::World* target)
{
	auto it = entities.begin();
	for(; it != entities.end(); ++it)
	{
		it->second->OnSerialize(target->add_entity_table());
	}
}

// -------------------------------------------------------------------------
void AeonEditorWorld::OnLoadEntity(const Aeon::Serialize::EntityInstance& data)
{
	using namespace Aeon;

	if(!worldEditor) return;

	// We want to examine each entity instance we're going to instance
	Aeon::Entity* ent = Aeon::EntityDatabase::Instance().GetFactory()->CreateEntity(data);
	
	OnValidateEntity(ent);

	worldEditor->AddEntityToTree(ent);
	entities[ent->GetUniqueID()] = ent;
}

// -------------------------------------------------------------------------
void AeonEditorWorld::SetWorldEditor(AeonWorldEditorWidget* widget)
{
	worldEditor = widget;
}

// -------------------------------------------------------------------------
void AeonEditorWorld::SetParsedVEngineObjects(const QStringList& list)
{
	parsedVEngineObjects = list;
}

// -------------------------------------------------------------------------
void AeonEditorWorld::AddEntity(Aeon::Entity* entity)
{
	entities[entity->GetUniqueID()] = entity;
}

// -------------------------------------------------------------------------
void AeonEditorWorld::RemoveEntity(const Aeon::UUID& uuid)
{
	auto it = entities.find(uuid);

	if(it != entities.end())
	{
		Aeon::Entity* ptr = it->second;
		entities.erase(it);
		Aeon::EntityDatabase::Instance().GetFactory()->DestroyEntity(ptr);
	}
}

// -------------------------------------------------------------------------
Aeon::Entity* AeonEditorWorld::GetEntity(const Aeon::UUID& uuid)
{
	auto it = entities.find(uuid);

	if(it != entities.end())
	{
		return it->second;
	}

	return 0;
}

// -------------------------------------------------------------------------
void AeonEditorWorld::OnValidateEntity(Aeon::Entity* ent)
{		
	// TODO: Proper validation code
	/*
	// Check whether the entity is missing any components
	if(ent->GetDescriptor())
	{
		const Aeon::Vector<Aeon::ComponentDescriptor*> components& = ent->GetDescriptor()->GetComponents();
		for(auto it = components.begin(); it != components.end(); ++it)
		{
			if(!ent->HasComponent((*it)->GetUUID()))
			{
				ent->AttachComponentInstance(Aeon::EntityDatabase::Instance().GetComponentRegistry()->CreateComponent((*it)->GetUUID(),ent));
				// Attach attributes
			}
		}
	}
	// Check whether any components were removed
	
	// Perform checks with attributes
	
	*/

	// If any VEngine object have been removed, remove them.
	if(ent->CheckAttribute(ATTR("VEngineObjectBehaviour.VEngineObject")) == Aeon::IAttribute::STRING)
	{
		Aeon::String str = ent->GetAttribute<Aeon::String>(ATTR("VEngineObjectBehaviour.VEngineObject"));
		if(!parsedVEngineObjects.contains(QString::fromStdString(str)))
		{
			ent->SetAttribute<Aeon::String>(ATTR("VEngineObjectBehaviour.VEngineObject"), "");
		}
	}
}
// -------------------------------------------------------------------------
void AeonEditorWorld::RefreshCallback()
{
	auto it = entities.begin();

	for(; it != entities.end(); ++it)
	{
		worldEditor->AddEntityToTree(it->second);
	}
}

// -------------------------------------------------------------------------
void AeonEditorWorld::ValidateEntities()
{
	for(auto it = entities.begin(); it != entities.end(); ++it)
	{
		OnValidateEntity(it->second);
	}
}

// -------------------------------------------------------------------------
void AeonEditorWorld::ClearAll()
{
	this->parsedVEngineObjects.clear();

	auto it = entities.begin();
	while(it != entities.end())
	{
		Aeon::Entity* ptr = it->second;
		Aeon::EntityDatabase::Instance().GetFactory()->DestroyEntity(ptr);
		it = entities.erase(it);
	}
}

