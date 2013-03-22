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
#ifndef WorldBase_h__
#define WorldBase_h__

#include <Aeon/EntityDescriptor.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/EntityFactory.h>
#include <Aeon/Entity.h>
#include <Aeon/Logging/Logging.h>
#include <Aeon/Util/STLHelpers.h>
#include <Aeon/Export.h>

namespace Aeon {
	
	/**
	 * The World ID.
	 */
	typedef HashedKey WorldID;

	/**
	 * Abstract base class for a "world". A world represents an interactive scene powered by entities. 
	 * Scene data is persisted in .wdb (world database) files, which this class can load and save. 
	 * 
	 * @note This class only provides the persistence layer for entities, the implementing class has to provide the 
	 * runtime storage for entities.
	 */
	class AEON_EXPORT WorldBase
	{
	public:
		/**
		 * Ctor.
		 */
		WorldBase();

		/**
		 * Dtor.
		 */
		virtual ~WorldBase();
	
	
	public:
		/**
		 * Attempts to load the given .wdb file.
		 * 
		 * @param filename full path (relative or absolute) to the .wdb file.
		 */
		bool Load(const String& filename);
	
		/**
		 * Attempts to save to .wdb  file to the given file.
		 * 
		 * @param filename full path (relative or absolute) to the .wdb file.
		 */
		bool Save(const String& filename);
	
	public:
		/**
		 * Request the world to setup its scene, loading and placing all entities and prepare them for being updated.
		 * 
		 * @see OnUpdate
		 */
		void OnSetupScene();

	public:
		/**
		 * Set the name of the world (identifier).
		 */
		void SetWorldName(const String& name);

		/**
		 * Returns the current world name (identifier).
		 */
		const String& GetWorldName(); 


		/**
		 * Returns the file (including path, if provided) that was used to load the .wdb from.
		 */
		const String& GetFilename();

		/**
		 * Returns the current world identifier ().
		 */
		const WorldID& GetID(); 

		/**
		 * Returns the unix timestamp of the last save.
		 */
		uint32_t GetLastSavedTimestamp();
		
		/**
		 * Returns an entity containing world parameters. The implementing class has to take care of the actual format.
		 */
		Entity* GetSettings();
		
		/**
		 * Set the entity containing world parameters. The implementing class has to take care of the actual format.
		 */		
		void SetSettings(Entity* settingsInstance);

		/**
		 * Returns true if the .wdb file has been successfully loaded.
		 */
		bool IsLoaded();
		
		/**
		 * Returns the number of entities serialited within the .wdb file.
		 */
		int SerializedEntitiesCount();

	public:
		/**
		 * Updates all entities / buckets in the scene.
		 */
		virtual void OnUpdate(float delta) = 0;
	protected:
		/**
		 * Callback that is being called every time the world is going to be saved to disk. This allows the
		 * implementing class to implement custom save handling (i.e. save game).
		 */
		virtual void OnSerializeEntities(Aeon::Serialize::World* target) = 0;

		/**
		 * Called whenever the world manager requests the world to set up its scene.
		 * This callback allows the implementing class to handle the (custom) instantiation of entities. 
		 * 
		 * @see OnSetupScene
		 */
		virtual void OnLoadEntity(const Aeon::Serialize::EntityInstance& data) = 0;

	private:
		/**
		 * Identifier for this world. 
		 */
		WorldID id;

		/**
		 * World parameters/settings.
		 */
		Entity* settings;

		/**
		 * World and file name.
		 */
		String filename, worldname;

		/**
		 * Last saved timestamp.
		 */
		uint32_t timestamp;

		/**
		 * Storage class implementation.
		 */
		Serialize::World* wdb;
	};
}

#endif // WorldBase_h__
