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
#ifndef EntityDatabase_h__
#define EntityDatabase_h__

#include <unordered_map>

#include <Aeon/EntityFactory.h>
#include <Aeon/EntityDescriptor.h>
#include <Aeon/Singleton.h>
#include <Aeon/Types/HashedKey.h>
#include <Aeon/Export.h>

namespace Aeon {	
	/**
	 * @brief Database for Entity and Component Descriptors (aka templates, classes), allowing to persist
	 * all registered templates to a file and load from a file respectively.
	 */
	class AEON_EXPORT EntityDatabase : public Singleton<EntityDatabase> 
	{
		friend class Singleton<EntityDatabase>;

	public:
		typedef std::unordered_map<const UUID, EntityDescriptor*, UUIDKeyHashFunction> EntityDescriptors;
		typedef std::unordered_map<const UUID, ComponentDescriptor*, UUIDKeyHashFunction> ComponentDescriptors;

	protected:
		/**
		 * Constructor.
		 */
		EntityDatabase();

		/**
		 * Copy constructor.
		 */
		EntityDatabase(const EntityDatabase& rhs);

	public:
		/**
		 * Destructor.
		 */
		~EntityDatabase();

	public:
		/**
		 * Loads the given database from a file.
		 * 
		 * @param file the file to load
		 * @param omitMetaData omits meta information to minimize memory usage and string allocations. Makes database lookups faster.
		 * 
		 * @return true if the database loaded successfully, false if an error occurred.
		 */
		bool Load(const String& file, bool omitMetaData = true);

		/**
		 * Saves the database to a file.
		 */
		bool Save(const String& file);

		/**
		 * Attempt to merge two database files on load. Returns false if merging fails, else true.
		 * The merged result will then be loaded into memory and available.
		 */
		bool Merge(const String& fileA, const String& fileB);

		/**
		 * Initializes a new, empty database. 
		 * 
		 * @param name database name.
		 * @param description database description.
		 *
		 * @note This will drop all currently loaded data structures from memory and wipe the registered entities.
		 */
		void InitNew(const String& name, const String& description);
		
		/**
		 * Shuts down the database, freeing all allocated memory. Does not save any modifications.
		 * 
		 * @note Be careful when using this, this will invalidate all Components and Entities that are still floating 
		 * around, so you should shutdown the EntityFactory and ComponentRegistry first (and destroy any instances 
		 * created with these factories).
		 * 
		 * @see EntityFactory
		 * @see ComponentRegistry
		 */
		void Shutdown();

		/**
		 * Resets the database to the status it had while on disk, so all changes after the Load() operation will be 
		 * rolled back when calling this method.
		 */
		void ResetChanges();
	
		/**
		 * Clears the whole entity descriptor table, effectively dropping all allocated memory, making the database
		 * empty if it is saved again without registering new templates.
		 */
		void ClearAll();


		/**
		 * Returns true if the database has been loaded in lightweight mode without meta data.
		 */
		bool IsLightweightMode();
	
	public:
		/**
		 * Get the database description field.
		 */
		const String& GetDatabaseDescription() const;
	
		/**
		 * Get the database name.
		 */
		const String& GetDatabaseName() const;
		
		/**
		 * Get the UNIX timestamp of when the the last time Save() has been performed.
		 */
		const String& GetLastModifiedTimestamp() const; 
		
		/**
		 * Get the current database api version
		 */
		const String& GetDatabaseAPIVersion() const;
		
		/**
		 * Sets the database name.
		 */
		void SetDatabaseName(const String& name);
		
		/**
		 * Sets the database description field.
		 */
		void SetDatabaseDescription(const String& description);

	public:
		/**
		 * Get the attached factory that can be used to construct new entities.
		 */
		EntityFactory* GetFactory();

		/**
		 * Get a entity template.
		 * 
		 * @param found indicates whether the template was found or the fallback (default instance) has been used.
		 * @param id the id to lookup.
		 */
		const EntityDescriptor* GetEntityDescriptor(const UUID& id) const;

		/**
		 * Get a mutable entity descriptor.
		 */
		EntityDescriptor* GetMutableEntityDescriptor(const UUID& id);

		/**
		 * Registers the given entity as a template.
		 * This is an expensive operation since it will instantiate a new template class and copy all attributes. Use sparingly.
		 */
		void AddEntityDescriptor(EntityDescriptor* tpl);
		
		/**
		 * Removes the entity template with the given id from the db.
		 */
		void RemoveEntityDescriptor(const UUID& id);

		/**
		 * Check whether the database already contains a template with the given name.
		 */
		bool HasEntityDescriptor(const UUID& id);
		
		/**
		 * Look up the Entity ID by its UUID.
		 */
		const EntityID& LookupEntityID(const UUID& uuid) const;
		
		/**
		 * Look up an Entity UUID by its key.
		 */
		const UUID& LookupEntityUUID(const EntityID& key) const;

		/**
		 * Get a map of all descriptors.
		 */
		const EntityDatabase::EntityDescriptors* GetEntityDescriptors() const;

	public:
		/**
		 * Get a pointer to the component registry (factory).
		 */
		ComponentRegistry* GetComponentRegistry();

		/**
		 * Get the component descriptor with the given key - or 0 if it doesn't exist.
		 */
		ComponentDescriptor* GetComponentDescriptor(const UUID& key);

		/**
		 * Add a component descriptor to the database.
		 */
		void AddComponentDescriptor(ComponentDescriptor* comp);

		/**
		 * Check whether the database contains the component descriptor identified by the given key.
		 */
		bool HasComponentDescriptor(const UUID& key);
		
		/**
		 * Get a list of all component descriptors currently registered with the database.
		 */
		const EntityDatabase::ComponentDescriptors* GetComponentDescriptors() const;

		/**
		 *
		 */
		void RemoveComponentDescriptor(const UUID& id);

		/**
		 * Reverse lookup function to retrieve the Component's UUID based on its ID.
		 */
		const UUID& LookupComponentUUID(const ComponentID& id);
	
	public:
		/**
		 * Get some memory statistics.
		 */
		String GetMemoryUsage();

	public:
		/**
		 * Get a list of all categories that are currently present in the database.
		 */
		const Vector<EntityCategory*>::type& GetCategories();
	
		/**
		 * Adds a new category to the db.
		 */
		EntityCategory* AddCategory(const String& categoryName);

		/**
		 * Get the category with the given name.
		 */
		EntityCategory* GetCategory(const String& categoryName);

		/**
		 * Get the category with the given uuid.
		 */
		EntityCategory* GetCategory(const UUID& id);

		/**
		 * Remove the category with the given name. Will assign all EntityDescriptors back to the "Default" category.
		 */
		void RemoveCategory(const String& categoryName);
		
	private:
		/**
		 * Helper function to deserialize the database from a file.
		 */
		void DeserializeDatabase(Serialize::EntityDB* entityDB);

		/**
		 * Helper function to serialize the database to a file.
		 */
		void SerializeDatabase(Serialize::EntityDB* entityDB);

	private:
		/**
		 * The factory used to spawn new entities.
		 */
		EntityFactory* factory;

		/**
		 * Component registry.
		 */
		ComponentRegistry* registry;

		/**
		 * Lightweigt loader flag
		 */
		bool lightweightLoader;
	};
}

#endif // EntityDatabase_h__
