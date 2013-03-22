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
#ifndef EntityFactory_h__
#define EntityFactory_h__

#include <Aeon/Entity.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/AeonForward.h>
#include <Aeon/Singleton.h>
#include <Aeon/MessageDispatcher.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 *  @brief Class providing factory functionality to instantiate entities from descriptors.
	 */
	class AEON_EXPORT EntityFactory
	{
		friend class EntityDatabase;
		
	private:		
		/**
		 * Constructor.
		 */
		EntityFactory();

	public:
		/**
		 * Shuts down the entity factory.
		 */
		void Shutdown();

	public:
		/**
		 * Creates a new entity instance from a template.
		 */
		Entity* CreateEntity(const String& key);
	
		/**
		 * Creates a new entity instance from the given template id.
		 */
		Entity* CreateEntity(const UUID& key);
		
		/**
		 * Creates a new entity instance from the the given serialization data.
		 */
		Entity* CreateEntity(const Serialize::EntityInstance& data);

		/**
		 * Creates a new, empty entity instance.
		 * Can be used to construct entities during runtime.
		 */
		Entity* New(const String& identifier);

		/**
		 * Destroys the given entity and frees all attached data.
		 */
		void DestroyEntity(Entity* ptr);

		/**
		 * Get a vector with all currently instanced entities. Be careful when using, CreateEntity and DestroyEntity change
		 * the contents of this vector.
		 * 
		 * @return a vector with the currently instanced entities
		 */
		const Vector<Entity*>::type& GetInstancedEntities() const;

		/**
		 * Destroys all entities the factory has instanced (and not already destroyed) so far.
		 */
		void DestroyAll();
	
	private:
		/**
		 * Set the parent database used to look up templates.
		 */
		void SetDatabase(EntityDatabase* database);

		/**
		 * Instantiates a "default" instance in case an entity does not exist.
		 **/
		Entity* DefaultInstance(const String& id);

	private:
		/**
		 * The actual entity database, used to store the entities.
		 */
		EntityDatabase* database;
	};
}

#endif // EntityFactory_h__
