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
#ifndef Entity_h__
#define Entity_h__

#include <map>

#include <Aeon/AeonForward.h>
#include <Aeon/Attribute.h>
#include <Aeon/AttributeContainer.h>
#include <Aeon/MessageChannel.h>
#include <Aeon/MessageDispatcher.h>
#include <Aeon/Types/HashedKey.h>
#include <Aeon/Export.h>

namespace Aeon {
	typedef HashedKey EntityID;
	typedef HashedKey ComponentID;
	class EntityDescriptor;
	class Component;

	/**
	 * @brief Class representing an entity, consisting of a set of attributes (key-value pairs) and attached components. 
	 * Compared to an descriptor this is a more lightweight object without unnecessary attributes - yet they rely on a descriptor in order to be instantiated.
	 * 
	 * @note Since this class is serving merely as a container for attributes and components, entities usually are instantiated as copies
	 * from predefined templates loaded from files.
	 * @see EntityDescriptor
	 */
	class AEON_EXPORT Entity : public AttributeContainer
	{
		friend class EntityFactory;

	public:
		/**
		 * Vector of components.
		 */
		typedef Vector<Component*>::type Components;

	public:
		/**
		 * Construct a new Entity with the given ID.
		 * 
		 * @param id entity id.
		 * @param descriptor the descriptor of this entity.
		 */
		Entity(const EntityID& id, const EntityDescriptor* descriptor);

	private:
		/**
		 * Copy constructor.
		 */
		Entity(const Entity& rhs);
		
	public:
		/**
		 * Destructor. 
		 */
		virtual ~Entity() {}

	public:
		/**
		 * Destroys the entity and all attached components / attributes.
		 */
		void Destroy();

		/**
		 * Get the Unique Id of this instance.
		 */
		const UUID& GetUniqueID() const;

		/**
		 * Get the non-unique entity identifier (name).
		 */
		const EntityID& GetID() const;
		
		/**
		 * Serializes this instances entity data into the given serialization pointer.
		 */
		 void OnSerialize(Serialize::EntityInstance* target) const;

		 /**
		  * Returns the descriptor that was used to construct this entity.
		  */
		  const EntityDescriptor* GetDescriptor();

	public:
		/**
		 * Checks whether a component with the given id is attached to this entity.
		 */
		bool HasComponent(const ComponentID& id);

		/**
		 * Returns the component with the given id (or 0).
		 */
		Component* GetComponent(const ComponentID& id);

		/**
		 * Get all components currently attached to the entity.
		 *
		 * @return reference to the component map.
		 */
		const Entity::Components& GetComponents() const;

		/**
		 * Removes the given component from the entity.
		 */
		void RemoveComponent(const ComponentID& id);

		/**
		 * Sends a message directly to this entity and forwards them to the attached components.
		 */
		void OnMessage(Message* msg);

	public:
		/**
		 * Set the "Update" flag. if set to true, all components will be notified if an attribute is changed.
		 */
		void SetUpdateFlag(bool flag);

		/**
		 * Prevents the entity from handling messages
		 */
		void StopMessageHandler();
		
		/**
		 * Enabled the entity to receive messages.
		 */
		void StartMessageHandler();
		
	public:
		/**
		 * AttributeContainer overrides.
		 *
		 * @see AttributeContainer
		 */
		void AttributeUpdated(const AttributeKey& key);

	private:
		/**
		 * Attaches the given component instance pointer to this entity.
		 */
		void AttachComponentInstance(Component* ptr);
		
	private:
		/**
		* Flag to indicate whether to notify attributes in case they changed. 
		*/
		bool updateFlag;

		/**
		 * Message handler flag.
		 */
		bool messageFlag;

		/**
		 * An unique id (GUID) for this instance.
		 */
		UUID uniqueID;

		/**
		 * Pointer to the descriptor that was used to instantiate this entity
		 */
		const EntityDescriptor* descriptor;

		/**
		 * The non-unique name of this entity.
		 */
		EntityID entityID;
		
		/**
		 * Components attached to this instance.
		 */
		Components components;
	};
}

#endif // Entity_h__
