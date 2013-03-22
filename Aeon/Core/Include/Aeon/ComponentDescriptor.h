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
#ifndef ComponentDescriptor_h__
#define ComponentDescriptor_h__

#include <map>

#include <Aeon/Attribute.h>
#include <Aeon/AeonForward.h>
#include <Aeon/Component.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief A component descriptor, serving as a template for components, so they can be 
	 * designed and attached to entities without requiring their native implementations.
	 * 
	 * A descriptor holds a name, a semi-unique ID and a set of parameters (Key-Attribute Type) pairs.
	 * 
	 * Once you attach this descriptor to an entity descriptor, the entity inherits all parameters defined here.
	 * 
	 * @see EntityDescriptor
	 */
	class AEON_EXPORT ComponentDescriptor
	{
	public:
		typedef std::map<const String, IAttribute::Type> Parameters;

	private:
		/**
		 * Constructor.
		 */
		ComponentDescriptor(const String& componentName);

		/**
		 * Constructor.
		 */
		ComponentDescriptor(const String& componentName, const UUID& uuid);

	public:
		/**
		 * Create a new descriptor.
		 */
		static ComponentDescriptor* Create(const String& name);

		/**
		 * Create a new descriptor from serialized data.
		 */
		static ComponentDescriptor* Create(const Serialize::ComponentDescriptor& serializedData);
		
		/**
		 * Destroy the given descriptor. Be careful with destroying descriptors attached to the database.
		 * @see EntityDatabase::AddComponentDescriptor
		 */
		static void Destroy(ComponentDescriptor* ptr);

	public:
		/**
		 *  Sets the name of this descriptor
		 */
		void SetName(const String& name);
		
		/**
		 * Returns the id of this component.
		 */
		const ComponentID& GetComponentID();

		/**
		 * Get the UUID. 
		 */
		const UUID& GetUUID();
	public:

		/**
		 * Assigns a new parameter of type T (has to be a valid attribute type) to this descriptor.
		 * 
		 * @see Attribute
		 */
		template <typename T>
		void AssignParameter(const String& name)
		{
			if(Attribute<T>::GetAttributeType() != IAttribute::INVALID)
			{
				parameters[name] = (IAttribute::Type)Attribute<T>::GetAttributeType();
			}
		}

		/**
		 * Removes the given paramater.
		 */
		void RemoveParameter(const String& name);
		
		/**
		 * Sets the given paramater to the given typ.e
		 */
		void SetParameter(const String& name, IAttribute::Type type);
		
		/**
		 * Get a list of all paramaters.
		 */
		const ComponentDescriptor::Parameters& GetParameters() const;

		/**
		 * Get the type of the given parameter.
		 */
		IAttribute::Type GetParameter(const String& key);
		
		/**
		 * Check whether the descriptor has a parameter with the given name.
		 */
		bool HasParamater(const String& key);

		/**
		 * Removes all paramaters.
		 */
		void ClearParameters();

		/**
		 * Get the description.
		 */
		 const String& GetDescription();

		 /**
		  * Set the description.
		  */
		 void SetDescription(const String& description);

	private:
		/**
		 * The id (key-hash pair)
		 */
		ComponentID componentID;
		
		/**
		 * The unique id.
		 */
		UUID uuid;
		/**
		 * Parameter map.
		 */
		Parameters parameters;

		/**
		 * The description.
		 */
		String description;
	};
}

#endif // ComponentDescriptor_h__
