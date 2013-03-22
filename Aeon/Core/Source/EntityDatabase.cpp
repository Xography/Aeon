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
#include <fstream>
#include <algorithm>

// We need to include this above to avoid macro collisions (LOGSEV_FATAL)
#include <Aeon/Serialization/EntityDB.pb.h>

#include <Aeon/EntityDatabase.h>
#include <Aeon/EntityFactory.h>
#include <Aeon/ComponentRegistry.h>

#define DB_VERSION "1.0"

namespace Aeon {
	// -------------------------------------------------------------------------
	// Wrapper to hide Protobuf implementation
	static struct DBWrapper 
	{
		DBWrapper():
			entityDB(new Serialize::EntityDB()),
			flag(false)
		{

		}

		Serialize::EntityDB* entityDB;
		String filename;
		bool flag;
	} db;

	// -------------------------------------------------------------------------
	// Descriptor Tables
	// 
	static EntityDatabase::EntityDescriptors descriptors;
	static Vector<EntityCategory*>::type categories;
	static  EntityDatabase::ComponentDescriptors componentDescriptors;

	// -------------------------------------------------------------------------
	using ::google::protobuf::RepeatedPtrField;

	// -------------------------------------------------------------------------
	//typedef std::map<const EntityID, std::map<const AttributeKey, IAttribute* > > AttributeTable;
	//static AttributeTable attributeCache;
	
	// -------------------------------------------------------------------------
	EntityDatabase::EntityDatabase():
		factory(new EntityFactory()), 
		registry(new ComponentRegistry()),
		lightweightLoader(false)
	{
		GOOGLE_PROTOBUF_VERIFY_VERSION;
		factory->SetDatabase(this);
	}

	// -------------------------------------------------------------------------
	EntityDatabase::EntityDatabase(const EntityDatabase& rhs):
		factory(rhs.factory), 
		registry(rhs.registry),
		lightweightLoader(rhs.lightweightLoader)
	{
		factory->SetDatabase(this);
	}
	// -------------------------------------------------------------------------
	void EntityDatabase::DeserializeDatabase(Serialize::EntityDB* entityDB)
	{
		// Load Category Descriptors
		if(!lightweightLoader)
		{
			for(auto it = entityDB->categories().begin(); it != entityDB->categories().end(); ++it)
			{
				const Serialize::CategoryDescriptor& desc = *it;
				LOG_DEBUG << "Loading Category Descriptor: " << desc.name();

				categories.push_back(EntityCategory::Create(desc));
			}

			if(categories.empty())
			{
				categories.push_back(EntityCategory::GetDefaultInstance());
			}
		}

		// Load Component Descriptors
		for(auto it = entityDB->components().begin(); it != entityDB->components().end(); ++it)
		{
			ComponentDescriptor* descriptor = ComponentDescriptor::Create(*it);
			if(!lightweightLoader)
			{
				descriptor->SetDescription((*it).description());
			}
			componentDescriptors[descriptor->GetUUID()] = descriptor;
		}

		// Load entity descriptors from file
		for(auto it = entityDB->entities().begin(); it != entityDB->entities().end(); ++it)
		{
			const Serialize::EntityDescriptor& src = *it;
			EntityDescriptor* entityDescriptor = EntityDescriptor::Create(src);

			// Assign component descriptors (with parameters)
			for(auto itComponent = src.components().begin(); itComponent != src.components().end(); ++itComponent)
			{
				ComponentDescriptor* componentDescriptor = componentDescriptors[UUID(*itComponent)];
				if(componentDescriptor) entityDescriptor->AddComponent(componentDescriptor);
				else LOG_WARNING << "Component Lookup Failed: " << UUID(*itComponent) << " (ID not present in Database).";
			}

			if(!lightweightLoader)
			{
				entityDescriptor->SetCategory(GetCategory(UUID(src.category())));
			}
			
			descriptors[entityDescriptor->GetUUID()] = entityDescriptor;
		}
	}
	// -------------------------------------------------------------------------
	bool EntityDatabase::Load(const String& file, bool omitMetaData)
	{	
		if(file.empty())
		{
			LOG_INFO << "Empty file parameter passed. No database can be loaded.";
			return true;
		}

		LOG_DEBUG << "Attempting to load entities from " << file;

		std::ifstream fileStream (file, std::ios::in|std::ios::binary);

		if(!fileStream.is_open())
		{
			LOG_ERROR << "FAILED to open Entity Database file: " << file;
			return false;
		}
		
		try 
		{
			db.entityDB->Clear();

			LOG_DEBUG << "EntityDB: Parsing into memory...";
			db.entityDB->ParsePartialFromIstream(&fileStream);
		}
		catch(::google::protobuf::exception& e) 
		{
			LOG_FATAL << "Error Parsing EntityDB: " << e.what();
			return false;
		}

		if(omitMetaData)
		{
			LOG_INFO << "Using lightweight runtime database loader.";
			lightweightLoader = true;
		} 
		else 
			lightweightLoader = false;
			

		DeserializeDatabase(db.entityDB);

		db.filename = file;
		LOG_DEBUG << "\t Loaded " << descriptors.size() << " entity descriptors.";
		LOG_DEBUG << "\t Loaded " << categories.size() << " entity categories.";
		LOG_DEBUG << "\t Loaded " << componentDescriptors.size() << " component descriptors.";

		LOG_DEBUG << GetMemoryUsage();

#ifdef DEBUG
	//	LOG_DEBUG << db.entityDB->DebugString();
#endif

		if(lightweightLoader)
		{
			db.entityDB->Clear();

		}
		return true;
	}

	// -------------------------------------------------------------------------

	void EntityDatabase::SerializeDatabase(Serialize::EntityDB* entityDB)
	{
		// Serialize Categories
		for(auto it = categories.begin(); it != categories.end(); ++it)
		{
			if(!(*it))	continue;

			Aeon::Serialize::CategoryDescriptor* cat = entityDB->add_categories();
			cat->set_name((*it)->GetName());
			UUID::Serialize((*it)->GetID(), cat->mutable_id());
		}

		// Serialize Component Descriptors
		for(auto it = componentDescriptors.begin(); it != componentDescriptors.end(); ++it)
		{
			if(!it->second)	continue;

			Serialize::ComponentDescriptor* comp = entityDB->add_components();
			ComponentDescriptor* src = it->second;
			comp->set_type(Serialize::ComponentDescriptor_ComponentType_NATIVE);
			comp->set_name(src->GetComponentID().key);
			comp->set_description(src->GetDescription());
			UUID::Serialize(src->GetUUID(), comp->mutable_id());
			for(auto paramIter = src->GetParameters().begin(); paramIter != src->GetParameters().end(); ++paramIter)
			{
				Serialize::ComponentDescriptor_Parameter* param = comp->add_parameters();
				param->set_key(paramIter->first);
				param->set_type((Serialize::AttributeType)paramIter->second);
			}
		}

		// Serialize Entity Descriptors
		for(auto it = descriptors.begin(); it != descriptors.end(); ++it)
		{
			if(!it->second)	continue;

			Serialize::EntityDescriptor* entity = entityDB->add_entities();
			EntityDescriptor* src = it->second;
			UUID::Serialize(src->GetUUID(), entity->mutable_id());
			entity->set_name(src->GetName());
			UUID::Serialize(src->GetCategory()->GetID(), entity->mutable_category());

			for(Attributes::const_iterator attribIter = src->GetAttributes().begin(); attribIter != src->GetAttributes().end(); ++attribIter)
			{
				IAttribute::SerializeAttribute(attribIter->second, entity->add_attributes());
			}

			for(Vector<ComponentDescriptor*>::type::const_iterator compIter = src->GetComponents().begin(); compIter != src->GetComponents().end(); ++compIter)
			{
				UUID::Serialize((*compIter)->GetUUID(), entity->add_components());
			}
		}

	}
	// -------------------------------------------------------------------------
	bool EntityDatabase::Save(const String& file)
	{
		if(lightweightLoader)
		{
			LOG_ERROR << "The database was loaded using the lightweight loader which omits meta data! The database can only be saved when loading it with full meta information.";
			return false;
		}

		std::ofstream fileStream(file, std::ios::out | std::ios::binary);
		if(!fileStream)
		{
			LOG_FATAL << "Failed to open Entity Database for writing: " << file;
			return false;
		}

		LOG_DEBUG << "Storing Entity Database to " << file;
		Serialize::DBDescriptor* header = db.entityDB->mutable_dbdescriptor();
		std::stringstream sstr; 
		sstr << std::time(0);
		header->set_modified(sstr.str());

		db.entityDB->clear_components();
		db.entityDB->clear_categories();
		db.entityDB->clear_entities();

		SerializeDatabase(db.entityDB);
		
		try {
			db.entityDB->SerializePartialToOstream(&fileStream);
		}
		catch(::google::protobuf::exception& e)
		{
			LOG_FATAL << "Failed to save entity database: " << e.what();
			fileStream.close();
			return false;
		}

		LOG_DEBUG << "\t Stored " << descriptors.size() << " entity descriptors.";
		LOG_DEBUG << "\t Stored " << categories.size() << " entity categories.";
		LOG_DEBUG << "\t Stored " << componentDescriptors.size() << " component descriptors.";

		fileStream.close();
#ifdef DEBUG
		LOG_DEBUG << db.entityDB->DebugString();
#endif
		return true;
	}


	// -------------------------------------------------------------------------
	bool EntityDatabase::Merge(const String& /*fileA*/, const String& /*fileB*/)
	{
		LOG_WARNING << "Merging entity databases not implemented!";
		return false;
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::InitNew(const String& name, const String& description)
	{
		// Drop all data, if need be.
		if(db.entityDB)
		{
			ClearAll();
			db.entityDB->Clear();
		}

		// Re-Initialize Database.
		db.entityDB = new Serialize::EntityDB();
		Serialize::DBDescriptor* header = db.entityDB->mutable_dbdescriptor();
		header->set_name(name);
		header->set_description(description);
		header->set_version(DB_VERSION);

		// Set modified Timestamp
		std::stringstream sstr; 
		sstr << std::time(0);
		header->set_modified(sstr.str());

		lightweightLoader = false;
	}

	// -------------------------------------------------------------------------
	const EntityDescriptor* EntityDatabase::GetEntityDescriptor(const UUID& id) const
	{
		auto it = descriptors.find(id);

		if(it == descriptors.end())
		{
			LOG_WARNING << "Entity Descriptor \"" << id << "\" does not exist in database.";
			return 0;
		}

		return it->second;
	}

	// -------------------------------------------------------------------------
	EntityDescriptor* EntityDatabase::GetMutableEntityDescriptor(const UUID& id)
	{
		auto it = descriptors.find(id);

		if(it == descriptors.end())
		{
			LOG_WARNING << "Entity Descriptor \"" << id << "\" does not exist in database.";
			return 0;
		}

		return it->second;
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::AddEntityDescriptor(EntityDescriptor* tpl)
	{
		if(!tpl) return;

		LOG_DEBUG << "Adding entity descriptor " << tpl->GetID() << " to database.";
		descriptors[tpl->GetUUID()] = tpl;
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::RemoveEntityDescriptor(const UUID& id)
	{
		auto it = descriptors.find(id);

		if(it != descriptors.end())
		{
			LOG_DEBUG << "Deleting entity descriptor " << id;
			EntityDescriptor::Destroy(it->second);
			descriptors.erase(it);
		}

		LOG_DEBUG << "Entity descriptor \"" << id << "\" not found in database. Nothing to delete.";
	}
	// -------------------------------------------------------------------------
	void EntityDatabase::ResetChanges()
	{
		// Yep, thats cheap. But it works :)
		this->ClearAll();
		this->Load(db.filename);
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::ClearAll()
	{
		LOG_DEBUG << "Clearing Database.";

		// Clear Entity Descriptors
		if(descriptors.size() > 0)
		{
			LOG_DEBUG << "\t Deleting " << descriptors.size() << " entity descriptors.";
			
			auto descriptorIterator = descriptors.begin();
			while(descriptorIterator != descriptors.end())
			{
				EntityDescriptor* ptr = descriptorIterator->second;
				descriptorIterator = descriptors.erase(descriptorIterator);

				EntityDescriptor::Destroy(ptr);
			}
		}

		// Clear Categories
		if(categories.size() > 0)
		{

			LOG_DEBUG << "\t Deleting " << categories.size() << " entity categories.";

			auto categoryIterator = categories.begin();
			while(categoryIterator != categories.end())
			{
				EntityCategory* ptr = *categoryIterator;
				categoryIterator = categories.erase(categoryIterator);

				EntityCategory::Destroy(ptr);	
			}
		}
				
		// Clear Component Descriptors
		if(componentDescriptors.size() > 0)
		{
			LOG_DEBUG << "\t Deleting " << componentDescriptors.size() << " component descriptors.";

			auto componentsIterator = componentDescriptors.begin(); 
			while(componentsIterator != componentDescriptors.end())
			{
				ComponentDescriptor* ptr = componentsIterator->second;
				componentsIterator = componentDescriptors.erase(componentsIterator);

				ComponentDescriptor::Destroy(ptr);
			}
		}

		LOG_DEBUG << "Cleared Database.";
	}

	// -------------------------------------------------------------------------
	bool EntityDatabase::HasEntityDescriptor(const UUID& id)
	{
		return descriptors.find(id) != descriptors.end();
	}

	// -------------------------------------------------------------------------
	const EntityID& EntityDatabase::LookupEntityID(const UUID& id) const
	{
		auto it = descriptors.find(id);
		if(it == descriptors.end())
		{
			static const EntityID id;
			return id;
		}

		return it->second->GetID();
	}

	// -------------------------------------------------------------------------
	EntityFactory* EntityDatabase::GetFactory()
	{
		return factory;
	}

	// -------------------------------------------------------------------------
	const EntityDatabase::EntityDescriptors* EntityDatabase::GetEntityDescriptors() const
	{
		return &descriptors;
	}

	// -------------------------------------------------------------------------
	ComponentRegistry* EntityDatabase::GetComponentRegistry()
	{
		return registry;
	}

	// -------------------------------------------------------------------------
	const Vector<EntityCategory*>::type& EntityDatabase::GetCategories()
	{
		return categories;
	}

	// -------------------------------------------------------------------------
	EntityCategory* EntityDatabase::AddCategory(const String& categoryName)
	{
		EntityCategory* ptr = EntityCategory::Create(categoryName);
		categories.push_back(ptr);
		return ptr;
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::RemoveCategory(const String& categoryName)
	{
		auto it = std::find_if(categories.begin(),categories.end(), 
			[&categoryName](EntityCategory* cat) { 
				return cat->GetName()==categoryName; 
		});

		if(it == categories.end()) return;

		UUID categoryID = (*it)->GetID();
		EntityCategory* defaultCat = GetCategory("General");
		
		if(defaultCat->GetID() == categoryID)
		{
			LOG_WARNING << "Cannot remove default category.";
			return;
		}

		for(EntityDescriptors::iterator entityIt = descriptors.begin(); entityIt != descriptors.end(); ++entityIt)
		{
			if(entityIt->second->GetCategory()->GetID() == categoryID)
			{
				entityIt->second->SetCategory(defaultCat);
			}
		}

		EntityCategory* cat = (*it);
		categories.erase(it);
		EntityCategory::Destroy(cat);
	}

	// -------------------------------------------------------------------------
	EntityCategory* EntityDatabase::GetCategory(const String& categoryName)
	{
		auto it = std::find_if(categories.begin(),categories.end(), 
			[&categoryName](EntityCategory* cat) { 
				return cat->GetName()==categoryName; 
		});

		return it != categories.end() ? *it : EntityCategory::GetDefaultInstance();
	}

	// -------------------------------------------------------------------------
	EntityCategory* EntityDatabase::GetCategory(const UUID& id)
	{
		auto it = std::find_if(categories.begin(),categories.end(), 
			[&id](EntityCategory* cat) { 
				return cat->GetID()==id; 
		});

		return it != categories.end() ? *it : EntityCategory::GetDefaultInstance();
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::AddComponentDescriptor(ComponentDescriptor* comp)
	{
		if(!comp) return;

		LOG_DEBUG << "Registered component descriptor for " << comp->GetComponentID();
		componentDescriptors[comp->GetUUID()] = comp;
	}

	// -------------------------------------------------------------------------
	bool EntityDatabase::HasComponentDescriptor(const UUID& key)
	{
		return componentDescriptors.find(key) != componentDescriptors.end();
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::RemoveComponentDescriptor(const UUID& id)
	{
		auto it = componentDescriptors.find(id);
		if(it != componentDescriptors.end())
		{
			ComponentDescriptor* ptr = it->second;
			LOG_DEBUG << "Removing " << ptr->GetComponentID() << " from database.";
			componentDescriptors.erase(it);
			ComponentDescriptor::Destroy(ptr);	
		}
	}

	// -------------------------------------------------------------------------
	const EntityDatabase::ComponentDescriptors* EntityDatabase::GetComponentDescriptors() const
	{
		return &componentDescriptors;
	}

	// -------------------------------------------------------------------------
	ComponentDescriptor* EntityDatabase::GetComponentDescriptor(const UUID& key)
	{
		auto it = componentDescriptors.find(key);
		if(it != componentDescriptors.end())
		{
			LOG_DEBUG << "Returning component descriptor " << key;
			return it->second;
		}

		LOG_WARNING << "Cannot find component descriptor with id " << key << ". Returning null.";
		return 0;
	}

	static String empty;
	// -------------------------------------------------------------------------
	const String& EntityDatabase::GetDatabaseDescription() const
	{
		return db.entityDB->has_dbdescriptor() ? db.entityDB->dbdescriptor().description() : empty;
	}

	// -------------------------------------------------------------------------
	const String& EntityDatabase::GetDatabaseName() const
	{
		return db.entityDB->has_dbdescriptor() ? db.entityDB->dbdescriptor().name() : empty;

	}

	// -------------------------------------------------------------------------
	const String& EntityDatabase::GetLastModifiedTimestamp() const
	{
		return db.entityDB->has_dbdescriptor() ?  db.entityDB->dbdescriptor().modified() : empty;
	}

	// -------------------------------------------------------------------------
	const String& EntityDatabase::GetDatabaseAPIVersion() const
	{
		return db.entityDB->has_dbdescriptor() ?  db.entityDB->dbdescriptor().version() : empty;
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::SetDatabaseName(const String& name)
	{
		if(db.entityDB->IsInitialized())
		{
			db.entityDB->mutable_dbdescriptor()->set_name(name);
		}
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::SetDatabaseDescription(const String& description)
	{
		if(db.entityDB->IsInitialized())
		{
			db.entityDB->mutable_dbdescriptor()->set_description(description);
		}
	}

	// -------------------------------------------------------------------------
	void EntityDatabase::Shutdown()
	{
		LOG_INFO << "Shutting down Entity Database";

		// Clear the database contents, freeing all pointers.
		ClearAll();

		factory->Shutdown();
		registry->Shutdown();

		// Clear Serialization structure.
		db.entityDB->Clear();

		// Ensure that protocol buffers clears all caches too.
		::google::protobuf::ShutdownProtobufLibrary();	
	}

	// -------------------------------------------------------------------------
	EntityDatabase::~EntityDatabase()
	{
		Shutdown();
		delete factory;
		delete registry;
	}

	// -------------------------------------------------------------------------
	Aeon::String EntityDatabase::GetMemoryUsage()
	{
		StringStream str;

		str << std::endl << "\tActive DB Memory (Cached): " << db.entityDB->SpaceUsed() 
			<< " (" << db.entityDB->GetCachedSize() << ") Bytes" << std::endl
			<< "\tEntity Memory: " << db.entityDB->mutable_entities()->SpaceUsedExcludingSelf() << " Bytes";

		return str.str();
	}

	// -------------------------------------------------------------------------
	const UUID& EntityDatabase::LookupEntityUUID(const EntityID& key) const
	{
		//Passed key when searching : key = {key="UserIntefaceWidget" hash=306941735 }
		//Key from descriptor in DB : key = {key="UserInterfaceWidget" hash=3606516946 }

		auto it = std::find_if(descriptors.begin(), descriptors.end(), [&key](const std::pair<UUID, EntityDescriptor*>& data)
		{
				return data.second->GetID() == key;
		});

		return it != descriptors.end() ? it->second->GetUUID() : UUID::Null();
	}

	// -------------------------------------------------------------------------
	const UUID& EntityDatabase::LookupComponentUUID(const ComponentID& id)
	{
		auto it = std::find_if(componentDescriptors.begin(), componentDescriptors.end(), [&id](const std::pair<UUID, ComponentDescriptor*>& data)
		{
				return data.second->GetComponentID() == id;
		});

		return  it != componentDescriptors.end() ? it->second->GetUUID() : UUID::Null();

	}
	
	// -------------------------------------------------------------------------
	bool EntityDatabase::IsLightweightMode()
	{
		return lightweightLoader;
	}

}
