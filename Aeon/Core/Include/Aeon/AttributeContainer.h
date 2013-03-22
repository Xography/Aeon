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
#ifndef AttributeContainer_h__
#define AttributeContainer_h__

#include <map>
#include <Aeon/AeonTypes.h>
#include <Aeon/Attribute.h>
#include <Aeon/Export.h>

namespace Aeon {

	/**
	* Map holding attributes.
	*/
	typedef std::map<const Aeon::AttributeKey, Aeon::IAttribute*> Attributes;

	/**
	 * @brief Abstract base class providing common functionality for classes containing a set of attributes.
	 */
	class AEON_EXPORT AttributeContainer 
	{
	public:
		/**
		 * Get an attribute from this entity instance. It is important to cast to the correct type.  If unsure check with
		 * Attribute::GetType()
		 * 
		 * @param key the attribute key to lookup.
		 *
		 * @return Read only reference to the data of the given type.
		 */
		template<typename T> const T& GetAttribute(const AttributeKey& key)
		{
			return static_cast<Attribute<T>* >(attributes[key])->Get();
		}

		template<typename T> const T& GetAttribute(const uint32_t key)
		{
			return static_cast<Attribute<T>* >(attributes[HashedKey(key)])->Get();
		}

		template<typename T> const T& GetAttribute(const char* key)
		{
			return GetAttribute<T>(ATTR(key));
		}
		/**
		 * Returns the pointer to the attribute container, allowing low level access to the data. Be careful when using this.
		 * 
		 * @param key the attribute key.
		 * 
		 * @return pointer the attribute wrapper
		 */
		template<typename T> Attribute<T>* GetAttributePointer(const AttributeKey& key)
		{
			return static_cast<Attribute<T>* >(attributes[key]);
		}

		template<typename T> const T& GetAttributePointer(const uint32_t key)
		{
			return static_cast<Attribute<T>* >(attributes[HashedKey(key)]);
		}
		/**
		 * Set the attribute of this entity instance. It is important to pass the correct type. If unsure check with
		 * Attribute::GetType()
		 * 
		 * @param key the key of the attribute.
		 * @param value the value of the attribute.
		 */
		template<typename T> void SetAttribute(const AttributeKey& key, T value)
		{
			if(attributes[key])
			{
				reinterpret_cast<Attribute<T>*>(attributes[key])->Set(value);
				return;
			}

			Attribute<T>* att = Attribute<T>::Create(key, value, this);
			attributes[key] = att;
		}

		template<typename T> void SetAttribute(const char* key, T value)
		{
			SetAttribute(ATTR(key), value);
		}

		/**
		 * Adds an attribute. If it already exists, but is not of the matching type - it will be destroyed and recreated.
		 * 
		 * @param key the key of the attribute.
		 * @param value the value of the attribute.
		 */
		template<typename T> void AddAttribute(const AttributeKey& key, T value)
		{
			if(attributes[key])
				return;			

			Attribute<T>* att = Attribute<T>::Create(key, value, this);
			attributes[key] = att;
		}

		template<typename T> void AddAttribute(const AttributeKey& key)
		{
			AddAttribute(key, Attribute<T>::DefaultValue());
		}


		/**
		 * Get the map of attributes.
		 */
		const Attributes& GetAttributes() const
		{
			return attributes;
		}

		/**
		 * Clear all attributes (and free memory they allocated).
		 */
		void ClearAttributes();

		/**
		 * Assigns the attribute to this container by creating a copy of the pointer's data.
		 */
		void AssignCopy(IAttribute* ptr);
		
		/**
		 * Adds an attribute to this container. No copying involved.
		 */
		void AssignAttribute(IAttribute* ptr);
	public:
		/**
		 * Checks whether the entity has an attribute with the given key, if so it returns the type of the given attribute - if not, it
		 * will return IAttribute::Type::INVALID;
		 */
		uint32_t CheckAttribute(const AttributeKey& key);

		/**
		 * Callback for attributes in case they got changed.
		 */
		virtual void AttributeUpdated(const AttributeKey& key) = 0;

	protected:
		/**
		 * We need to keep attributes in an ordered map in order to serialize and deserialize values properly.
		 */
		Attributes attributes;

		/**
		 * Internal function to help deallocate attributes.
		 */
		void DestroyAttribute(IAttribute*  attr);
	};
}

#endif // AttributeContainer_h_
