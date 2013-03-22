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
#ifndef Component_h__
#define Component_h__

#include <Aeon/Entity.h>
#include <Aeon/Message.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/AeonForward.h>
#include <Aeon/IMessageHandler.h>
#include <Aeon/MessageChannel.h>
#include <Aeon/Types/HashedKey.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * Component Identifier
	 */
	typedef HashedKey ComponentID;
	class AEON_EXPORT ComponentDescriptor;


#define AEON_COMPONENT_DECLARE(type_class_impl_, typename_string_) \
	public: type_class_impl_() : Component(Aeon::ComponentID(typename_string_)) { } \
	~type_class_impl_() {} \
	type_class_impl_(const type_class_impl_& rhs, Aeon::Entity* parent): Component(rhs.id, parent) { this->OnCreateComponentInstance(); } \
	Component* Create(Aeon::Entity* parent) { type_class_impl_* ptr = new type_class_impl_(*this, parent);  ptr->OnInitialise(parent); return ptr; } \

	/**
	 * @brief Abstract base component class that can be used to implement behaviors for the Aeon Entity system.
	 */
	class AEON_EXPORT Component : public IMessageHandler
	{
	protected:
		/**
		 * Constructor.
		 **/
		Component(const ComponentID& id);

		/**
		 * Constructor, allows to set a parent.
		 */
		Component(const ComponentID& id, Entity* parent);
		
		/**
		 * Constructor, allows to set a parent and uuid.
		 */
		Component(const ComponentID& id, Entity* parent, const UUID& uuid);

	public:
		/**
		 * Destructor.
		 */
		virtual ~Component();

	public:
		/**
		 * Returns the non-unique identifier (name) of this component.
		 * 
		 * @return the id.
		 */
		const ComponentID& GetID() const;

		/**
		 * Change the ID of the component during runtime.
		 * 
		 * @note this affects only the current instance. If you want to change the template, you should ensure that
		 * you use ComponentRegistry::GetComponentTemplate to get the correct instance.
		 * 
		 * @param id the new id.
		 */
		void SetComponentID(const Aeon::ComponentID& id);

		/**
		 * Get the unique identifier of this component.
		 * 
		 * @return UUID
		 */
		const UUID& GetUUID() const;

		/**
		 * Get the parent entity this component is attached to, if any.
		 * 
		 * @return UUID
		 */
		Entity* GetParent() const;

		/**
		 * Set the parent Entity of this component.
		 *
		 * @param ent the parent
		 */
		void SetParent(Aeon::Entity* ent);

		/**
		 * Returns the descriptor uuid that was used to instantiate this component.
		 */
		const UUID& GetDescriptorUUID();

		/**
		 * Set the parent UUID for this component instance.
		 */
		void SetDescriptorUUID(const Aeon::UUID& parent);

		/**
		 * Changes the components UUID to the given one. 
		 */
		void SetUUID(const Aeon::UUID& thisUUID);

	public:
		/**
		 * Makes sure the component unsubscribes from all its channels. Must be called before deleting.
		 */
		void CleanupChannels();

	public:
		/**
		 * Creates a new component.
		 * 
		 * @note When implementing a component, the expected behavior of this function is to return a new instance of
		 * the component constructed using the copy constructor (to copy default values). This may avoid allocation overhead.
		 * 
		 * @param parent the parent entity.
		 * @return new component instance.
		 */
		virtual Component* Create(Aeon::Entity* parent) = 0;

		/**
		 * Called when a component is being first allocated and registered as template in the component registry. 
		 * 
		 * @This function is only called ONCE during an entity database life cycle.
		 */
		virtual void OnRegisterAsTemplate() { };
		
		/**
		 * Called when a component is being removed and unregistered as template from the component registry. 
		 * 
		 * @This function is only called ONCE during an entity database life cycle.
		 */
		virtual void OnUnregisterTemplate() { };
		/**
		 * Called a new component is being instanced. Reset your private fields here.
		 */
		virtual void OnCreateComponentInstance() = 0;


		/**
		 * Called a new component is being created. Put your initialization code  here.
		 */
		virtual void OnInitialise(Aeon::Entity* parent) = 0;
		
		/**
		 * Called when the component is destroyed, you MUST do your deinitilization in here. The destructor is also used when
		 * unregistering the descriptors!
		 */
		virtual void Destroy() = 0;

		/**
		 * Called by the responsible bucket (batch updater), if the component is assigned to a batch bucket. Otherwise
		 * this function will not be called.
		 * 
		 * @param delta time elapsed since last update.
		 */
		virtual void OnUpdate(float delta) { };

		/**
		 * Called by the message dispatcher if a message meant for this component (or it has subscribed to) arrives.
		 * 
		 * @param m the message.
		 */
		virtual void OnMessage(Message* m) { };

	public:
		/**
		 * Subscribes to the specified channel.
		 */
		void Subscribe(const Aeon::MessageChannel::ChannelID& id);

		/**
		 * Unsubscribes from the specified channel. You don't have to call this for every subscribed channel at shutdown, call Destroy()
		 * instead.
		 */
		void Unsubscribe(const Aeon::MessageChannel::ChannelID& id);

	protected:
		/**
		 * The entity this component is attached to (can be null).
		 */
		Aeon::Entity* parent;

		/**
		 * The ID of this component (unique across components, but not unique across instances).
		 */
		Aeon::ComponentID id;

		/**
		 * The UUID of this component, unique across all instances of this component, as well as other components.
		 */
		Aeon::UUID uuid;

		/**
		 * List of channels this component is subscribed to.
		 */
		std::vector<MessageChannel::ChannelID> subscriptions;
			
		/**
		 *
		 */
		Aeon::UUID parentUUID;
	};
}
#endif // Component_h__
