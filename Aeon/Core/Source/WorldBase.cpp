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
#include <ctime>

#include <Aeon/Serialization/World.pb.h>
#include <Aeon/WorldBase.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	WorldBase::WorldBase() :
		id(WorldID::Null()),
		wdb(new Serialize::World())
	{

	}

	// -------------------------------------------------------------------------
	WorldBase::~WorldBase()
	{
		// Let the factory destroy the entity, so it is properly removed from the entity system
		EntityDatabase::Instance().GetFactory()->DestroyEntity(settings);
	}

	// -------------------------------------------------------------------------
	bool WorldBase::Load(const String& filename)
	{
		if(filename.empty())
		{
			LOG_INFO << "Empty file parameter passed. WDB can't be loaded.";
			return true;
		}

		LOG_DEBUG << "Attempting to load WDB file: " << filename;

		std::ifstream fileStream (filename, std::ios::in|std::ios::binary);

		if(!fileStream.is_open())
		{
			LOG_ERROR << "FAILED to open wdb file: " << filename;
			return false;
		}

		try 
		{
			wdb->Clear();

			LOG_DEBUG << "WDB: Parsing into memory...";
			wdb->ParseFromIstream(&fileStream);
		}
		catch(::google::protobuf::exception&) 
		{
			LOG_FATAL << "Error Parsing WDB: " << wdb->InitializationErrorString();
			return false;
		}

		EntityDatabase& database = EntityDatabase::Instance();

		// Load standard data
		worldname = wdb->world_name();
		id = WorldID(worldname);
		this->filename = filename;
		timestamp = wdb->timestamp();

		// Load custom world settings
		const Serialize::EntityInstance& settingsData = wdb->settings();

		if(UUID(settingsData.entity_descriptor()) == UUID::Null())
		{
			LOG_ERROR << "Invalid World Settings UUID.";
			return false;	
		}

		if(database.HasEntityDescriptor(UUID(settingsData.entity_descriptor())))
		{
			settings = database.GetFactory()->CreateEntity(settingsData);
			LOG_DEBUG << "Instantiated World Settings... ";
		}

		else 
		{
			LOG_ERROR << "Cannot find the world settings entity required for this WDB file in the database: " << UUID(settingsData.entity_descriptor());
			return false;
		}

		fileStream.close();
		
		return true;
	}

	// -------------------------------------------------------------------------
	bool WorldBase::Save(const String& filename)
	{
		std::ofstream fileStream(filename, std::ios::out | std::ios::binary);

		if(!fileStream)
		{
			LOG_FATAL << "Failed to open wdb file for writing: " << filename;
			return false;
		}

		LOG_DEBUG << "Storing wdb level data into to " << filename;
		wdb->Clear();

		std::stringstream sstr; 
		sstr << std::time(0);
		wdb->set_timestamp(Aeon::Util::StringToInt(sstr.str()));
		wdb->set_world_name(worldname);
		settings->OnSerialize(wdb->mutable_settings());

		OnSerializeEntities(wdb);

		try
		{
			wdb->SerializeToOstream(&fileStream);
		}

		catch(::google::protobuf::exception&)
		{
			LOG_FATAL << "Failed to save level data into wdb file: " << wdb->InitializationErrorString();
			fileStream.close();
			return false;
		}

		fileStream.close();

		LOG_DEBUG << wdb->DebugString();

		return true;
	}

	// -------------------------------------------------------------------------
	void WorldBase::OnSetupScene()
	{
		if(!wdb->IsInitialized()) 
		{	
			LOG_ERROR << "World Database not initialized. Can not set up scene.";
			return;
		}

		for(int i = 0; i < wdb->entity_table_size(); i++)
		{
			OnLoadEntity(wdb->entity_table(i));	
		}
	}

	// -------------------------------------------------------------------------
	void WorldBase::SetWorldName(const String& name)
	{
		worldname = name;
		id = WorldID(name);
	}

	// -------------------------------------------------------------------------
	const String& WorldBase::GetWorldName()
	{
		return worldname;
	}

	// -------------------------------------------------------------------------
	const String& WorldBase::GetFilename()
	{
		return filename;
	}

	// -------------------------------------------------------------------------
	const WorldID& WorldBase::GetID()
	{
		return id;
	}

	// -------------------------------------------------------------------------
	uint32_t WorldBase::GetLastSavedTimestamp()
	{
		return timestamp;
	}

	// -------------------------------------------------------------------------
	Entity* WorldBase::GetSettings()
	{
		return settings;
	}

	// -------------------------------------------------------------------------
	void WorldBase::SetSettings(Entity* settingsInstance)
	{
		settings = settingsInstance;
	}

	// -------------------------------------------------------------------------
	bool WorldBase::IsLoaded()
	{
		return wdb->IsInitialized();
	}

	// -------------------------------------------------------------------------
	int WorldBase::SerializedEntitiesCount()
	{
		return wdb->entity_table_size();
	}
}
