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
#include "DatabaseManager.h"

using namespace Aeon;

DatabaseManager* DatabaseManager::instance = 0;

//------------------------------------------------------------
DatabaseManager::DatabaseManager(void) :
	database(EntityDatabase::Instance()),
	entitiesList(0),
	componentsList(0),
	isOpened(false)
{
}

//------------------------------------------------------------
DatabaseManager::~DatabaseManager(void)
{
}

//------------------------------------------------------------
DatabaseManager* DatabaseManager::GetInstance()
{
	if(!instance)
		instance = new DatabaseManager();

	return instance;
}

//------------------------------------------------------------
bool DatabaseManager::Open(const Aeon::String& filename)
{
	if(isOpened)
	{
		LOG_DEBUG << "Trying to open database while another already opened";
		return false;
	}

	if(!database.Load(filename, false))
		return false;

	entitiesList = database.GetEntityDescriptors();
	componentsList = database.GetComponentDescriptors();

	openedDatabaseFileName = filename;

	isOpened = true;

	emit database_opened();
	return true;
}

//------------------------------------------------------------
void DatabaseManager::Close()
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to close database while no database opened";
		return;
	}

	database.ClearAll();
	entitiesList = 0;
	componentsList = 0;

	isOpened = false;
	emit database_closed();
}

//------------------------------------------------------------
void DatabaseManager::Create(const String &name, const String &description)
{
	if(isOpened)
	{
		LOG_DEBUG << "Init new database while another one is opened. Closing.";
		Close();
	}

	database.InitNew(name, description);

	openedDatabaseFileName.clear();

	entitiesList = database.GetEntityDescriptors();
	componentsList = database.GetComponentDescriptors();

	isOpened = true;
	emit database_created();
}

//------------------------------------------------------------
bool DatabaseManager::Save()
{
	return Save(openedDatabaseFileName);
}

//------------------------------------------------------------
bool DatabaseManager::Save(const Aeon::String &filename)
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to save database while didn't opened";
		return false;
	}

	if(!database.Save(filename))
		return false;

	openedDatabaseFileName = filename;
	emit database_saved();
	return true;
}

//------------------------------------------------------------
int DatabaseManager::GetEntitiesCount()
{
	LOG_DEBUG << "Entities count is: " << entitiesList->size();
	return isOpened ? entitiesList->size() : 0;
}

//------------------------------------------------------------
EntityDescriptor* DatabaseManager::GetEntityDescriptor(quint32 position)
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to get entity while database isn't opened";
		return 0;
	}

	if(position >= entitiesList->size())
	{
		LOG_DEBUG << "Getting entity more than entities count";
		return 0;
	}

	static int i = 0;
	
	for(auto it = entitiesList->begin(); it != entitiesList->end(); ++it)
	{
		if(position == static_cast<quint32>(i))
			return it->second;

		i++;
	} 

	return 0;
}

//------------------------------------------------------------
EntityDescriptor* DatabaseManager::GetEntityDescriptor(const String &name)
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to get entity while database isn't opened";
		return 0;
	}

	return Aeon::EntityDatabase::Instance().GetMutableEntityDescriptor(EntityLookup(QString::fromStdString(name)));
}

//------------------------------------------------------------
EntityDescriptor* DatabaseManager::AddNewEntity(const String &name)
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to add new entity while database isn't opened";
		return 0;
	}

	EntityDescriptor *descriptor = EntityDescriptor::Create(name);
	database.AddEntityDescriptor(descriptor);

	return descriptor;
}

//------------------------------------------------------------
bool DatabaseManager::DeleteEntity(EntityDescriptor *entity)
{
	if(!isOpened)
	{
		LOG_DEBUG << "Trying to delete entity while database isn't opened";
		return false;
	}

	if(!entity)
	{
		return false;
	}

	database.RemoveEntityDescriptor(entity->GetUUID());

	return true;
}

// -------------------------------------------------------------------------
int DatabaseManager::GetComponentDescriptorCount()
{
	return isOpened? componentsList->size() : 0;
}

// -------------------------------------------------------------------------
Aeon::ComponentDescriptor* DatabaseManager::GetComponentDescriptor(quint32 pos)
{
	if(!isOpened) return 0;
	if(pos >= componentsList->size()) return 0;

	int i = 0;
	EntityDatabase::ComponentDescriptors::const_iterator it = componentsList->begin();
	
	for(; it != componentsList->end(); ++it)
	{
		if(pos == static_cast<quint32>(i))
			return it->second;	

		i++;
	}
	return 0;
}

// -------------------------------------------------------------------------
Aeon::ComponentDescriptor* DatabaseManager::GetComponentDescriptor(const QString& name)
{
	const UUID& uuid = ComponentLookup(name);
	return EntityDatabase::Instance().GetComponentDescriptor(uuid);
}

// -------------------------------------------------------------------------
bool DatabaseManager::HasComponentDescriptor(const QString& str)
{
	const UUID& uuid = ComponentLookup(str);
	return uuid != UUID::Null();
}

// -------------------------------------------------------------------------
void DatabaseManager::AddComponentDescriptor(Aeon::ComponentDescriptor* ptr)
{
	EntityDatabase::Instance().AddComponentDescriptor(ptr);
}

// -------------------------------------------------------------------------
void DatabaseManager::RemoveComponentDescriptor(const QString& name)
{
	const UUID& uuid = ComponentLookup(name);

	if(uuid != UUID::Null())
		EntityDatabase::Instance().RemoveComponentDescriptor(uuid);
}

// -------------------------------------------------------------------------
const Aeon::UUID& DatabaseManager::EntityLookup(const QString& id)
{
	return EntityDatabase::Instance().LookupEntityUUID(EntityID(id.toAscii().data()));
}

// -------------------------------------------------------------------------
const Aeon::UUID& DatabaseManager::ComponentLookup(const QString& id)
{
	return EntityDatabase::Instance().LookupComponentUUID(ComponentID(id.toAscii().data()));
}

// -------------------------------------------------------------------------
bool DatabaseManager::HasEntityDescriptor(const QString& name)
{
	return EntityDatabase::Instance().HasEntityDescriptor(EntityLookup(name));
}

