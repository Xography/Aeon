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
#ifndef EntityDescriptor_h__
#define EntityDescriptor_h__

#include <Aeon/Attribute.h>
#include <Aeon/AttributeContainer.h>
#include <Aeon/AeonForward.h>
#include <Aeon/ComponentDescriptor.h>
#include <Aeon/Entity.h>
#include <Aeon/EntityCategory.h>
#include <Aeon/Types/UUID.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief This heavy-weight class is used to describe an Entity in all its details. 
	 * Used by the Factory to create new entity instances and the editor to edit entity templates.
	 */
	class AEON_EXPORT EntityDescriptor : public AttributeContainer
	{
	private:
		/**
		 * Constructor.
		 */
		EntityDescriptor(const EntityID& id);
		
		/**
		 * Constructor.
		 */
		EntityDescriptor(const EntityID& id, const UUID& uuid);

	public:
		/**
		 * Destructor. 
		 */
		virtual ~EntityDescriptor() {}
		
	public:
		/**
		 * Creates a new, empty descriptor with the given name.
		 */
		static EntityDescriptor* Create(const String& name);
		
		/**
		 * Creates an empty descriptor with the given name and uuid.
		 */
		static EntityDescriptor* Create(const Serialize::EntityDescriptor& serializedData);

		/**
		 * Destroy the given entity descriptor.
		 */
		static void Destroy(EntityDescriptor* ptr);

	public:
		/**
		 * Get the ID of this entity descriptor.
		 */
		const EntityID& GetID() const;

		/**
		 * Get the UUID of this descriptor.
		 */
		const UUID& GetUUID() const;

		/**
		 * Get the name of this entity descriptor.
		 */
		const String& GetName() const;

		/**
		 * Sets the entity name. 
		 */
		void SetName(const String& name);

	public:
		/**
	 	 * Removes the given attribute from the entity. 
	 	 * 
	 	 * @param key the key of attribute to remove.
		 */
		void RemoveAttribute(const AttributeKey& key);

		/**
		 * Assign a map of attributes to this entity.
		 */
		void AssignAttributes(const Attributes& attributeMap);
		
	public:

		/**
		 * Add a component by its descriptor to this entity descriptor.
		 *
		 * @param comp the component to add.
		 */
		void AddComponent(ComponentDescriptor* comp);
		
		/**
		 * Remove the component with the given key from the entity (if attached).
		 * 
		 * @param comp the component to remove.
		 */
		void RemoveComponent(const UUID& id);

		/**
		 * Checks whether the entity has a component with the given key.
		 */
		bool HasComponent(const UUID& key);

		/**
		 * Get a reference to the list of components attached to this entity.
		 */
		const Vector<ComponentDescriptor*>::type& GetComponents() const;

		/**
		 * Clears the list of components
		 */
		void ClearComponents();
		
	public:
		/**
		 * Set this entity descriptors category. (Default: "Default")
		 */
		void SetCategory(EntityCategory* cat);
		
		/**
		 * Get the category of this descriptor.
		 */
		const EntityCategory* GetCategory() const;

		// Implements AttributeContainer
	public:
		void AttributeUpdated(const AttributeKey& key);
	
	private:
		/**
		 * The entity ID.
		 */
		EntityID id;

		/**
		 * The UUID.
		 */
		 UUID uuid;

		/**
		 * It is enough to just keep track of the components we're going to attach.
		 */
		Vector<ComponentDescriptor*>::type components;

		/**
		 * The entity category.
		 */
		EntityCategory* category;
	};
}

#endif // EntityDescriptor_h__