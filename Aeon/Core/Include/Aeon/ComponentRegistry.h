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
#ifndef ComponentRegistry_h__
#define ComponentRegistry_h__

#include <map>

#include <Aeon/AeonTypes.h>
#include <Aeon/AeonForward.h>
#include <Aeon/Component.h>
#include <Aeon/Types/UUID.h>
#include <Aeon/Entity.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief This class holds the templates to all available components and constructs them on demand.
	 */
	class AEON_EXPORT ComponentRegistry
	{
		friend class EntityDatabase;
		typedef std::unordered_map<const UUID, Component*, UUIDKeyHashFunction> ComponentsMap;

	public:
		ComponentRegistry();

	public:
		/**
		 * Initialises the Registry.
		 */
		void Initialise(/*const String& componentDatabase*/);

		/**
		 * Shuts down the registry properly.
		 */
		void Shutdown();

		/**
		 * Create a new component instance from a template. If the component does not exist, a warning will be issued and 
		 * a mock instance will be returned instead.
		 * 
		 * @param id the id of the component template to use.
		 * @param parent the parent entity
		 * 
		 * @return a new instance.
		 */
		Component* CreateComponent(const String& id, Entity* parent);

		/**
		 * Creates a new component from a template, identified by the given key.
		 * 
		 * @see ComponentRegistry::CreateComponent 
		 */
		Component* CreateComponent(const UUID& id, Entity* parent);
		
		/**
		 * Creates a new component from serialized data.
		 * 
		 * @see ComponentRegistry::CreateComponent 
		 */
		Component* CreateComponent(const Serialize::ComponentInstance& instance, Entity* parent);

		/**
		 * Registers a component to the registry.
		 * 
		 * @param component the component to use.
		 */
		void RegisterComponent(Component* component);

		/**
		 * Get a list of the names of components in the registry. This list can be compared against the 
		 * Descriptor List from the entity database.
		 * 
		 * @see EntityDatabase::GetComponentDescriptors()
		 * 
		 * @return a copy of the vector with the names of the components that are currently registered. 
		 */
		std::vector<ComponentID> GetComponentNameList();
		
	private:
		/**
		 * Maps component ID's to component instances.
		 */
		ComponentsMap comps;
	};
}

#endif // ComponentRegistry_h__
