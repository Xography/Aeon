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
#ifndef Attribute_h__
#define Attribute_h__

#include <unordered_map>

#include <Aeon/AeonTypes.h>
#include <Aeon/AeonForward.h>
#include <Aeon/Types/Blob.h>
#include <Aeon/Types/HashedKey.h>
#include <Aeon/Types/Matrix4x4.h>
#include <Aeon/Types/Quaternion.h>
#include <Aeon/Types/Range.h>
#include <Aeon/Types/Vector2.h>
#include <Aeon/Types/Vector3.h>
#include <Aeon/Types/Vector4.h>
#include <Aeon/Types/UUID.h>
#include <Aeon/Export.h>
#include <Aeon/Platform/Platform.h>

namespace Aeon {
	class AttributeContainer;
	typedef HashedKey AttributeKey;

	/**
	 *  @brief Basic Attribute interface.
	 */
	class AEON_EXPORT IAttribute
	{
	public:
		/**
		 * Destructor.
		 */
		virtual ~IAttribute() {}
		
	public:
		/**
		 * Attribute type, retrieved via GetType()
		 */
		enum Type 
		{
			INVALID = 0, // Can be used to check whether the set type is a valid type, i.e. if(GetType()){ ..
			BLOB,
			BOOL,
			DOUBLE,
			FLOAT,
			INT32,
			INT64,
			MATRIX4X4,
			QUATERNION,
			RANGE,
			STRING,
			VECTOR2,
			VECTOR3,
			VECTOR4/*,
			UINT32,
			UINT64,
			UUID,
			CHAR
			*/
		};

		/**
		 * Get the type of this attribute.
		 * 
		 * @return the AttributeType
		 */
		virtual unsigned int GetType() = 0;

		/**
		 * Get the attribute Key.
		 * 
		 * @return the key.
		 */
		virtual const AttributeKey& GetKey() const = 0;

		/**
		 * Unpacks the given data into memory, creating a new attribute instance.
		 */
		static IAttribute* DeserializeAttribute(const AttributeKey& key, const Serialize::Value& val, AttributeContainer* parent);

		/**
		 * Serializes the given Attribute pointer into the given container so the attribute can be safely destroyed.
		 */
		static void SerializeAttribute(IAttribute* ptr, Serialize::Attribute* container);

		/**
		 * Serializes only the value of the given attribute.
		 */
		static void SerializeValue(IAttribute* ptr, Serialize::Value* val);
	};

	/**
	 * @brief Attribute Container template class, a generic template to wrap attributes (and parameters) for entities.
	 */
	template<typename T> class Attribute : public IAttribute 
	{
	private:
		/**
		 * Constructor. 
		 */
		Attribute(AttributeKey key, T val, AttributeContainer* parent): 
			key(key), 
			parent(parent), 
			value(val)
		{
					
		}

	public:
		/**
		 * Set the value.
		 */
		void Set(T val)
		{
			this->value = val;
			NotifyParent();
		}

		/**
		 * Set the value by pointer.
		 */
		void Set(T* val)
		{
			this->value = (*val);
			NotifyParent();
		}

		/**
		 * Get a reference to the value.
		 * @return reference.
		 */
		const T& Get()
		{
			return value;
		}

		/**
		 * Get a copy of the value.
		 * 
		 * @return the value, as a copy.
		 */
		T GetCopy()
		{
			return value;
		}

		/**
		 * Get a pointer to the internal data. Use carefully.
		 * 
		 * @param the pointer to the data.
		 */
		T* GetMutable() 
		{
			return &value;
		}

		/**
		 * Get the attribute key (identifier).
		 * @return the key.
		 */
		const AttributeKey& GetKey() const
		{
			return key;
		}

		/**
		 * Factory method for the Attribute.
		 * 
		 * @param key the identifier for the attribute.
		 * @param val value for the attribute.
		 * @param parent the parent entity of the attribute.
		 * @return a new instance.
		 */
		static Attribute* Create(AttributeKey key, T val, AttributeContainer* parent)
		{
			Attribute<T>* ptr = new Attribute(key, val, parent);
			ptr->type = Attribute<T>::GetAttributeType();
			return ptr;
		}

		/**
		 * Get the Attribute type to safely check the attribute type before casting.
		 * 
		 * @see IAttribute::Type
		 * @return the attribute type, or INVALID if no template specialization for this type exists..
		 */
		unsigned GetType()
		{
			return type;
		}

		/**
		 * Place-holder for template specializations.
		 */
		static unsigned GetAttributeType() 
		{
			return INVALID;
		}

		static T DefaultValue();

	public:
		/**
		 * Equality operator.
		 */
		bool operator==(const Attribute<T>& rhs) const
		{
			return this->value == rhs.value;
		}

		/**
		 * Inequality operator.
		 */
		bool operator!=(const Attribute<T>& rhs) const
		{
			return this->value != rhs.value;
		}

	private:
		/**
		 * Notifies the parent, that this attribute has been changed. Causes the parent entity to emit a message to the attached components to inform them 
		 * of the change.
		 */
		void NotifyParent()
		{
			if(parent)
			#ifdef _MSC_VER
				parent->AttributeUpdated(key);
			#else
				NotifyParentWrapper(parent, this->key);
			#endif // _MSC_VER
		}

	private:
		/**
		 * The key to this attribute.
		 */
		AttributeKey key;

		/**
		 * The parent container holding this attribute
		 */
		AttributeContainer* parent;
		
		/**
		 * The value that is set for this attribute.
		 */
		T value;

		/**
		 * 
		 */
		uint32_t type;
	};

	/**
	 * Wrapper needed because Attribute requires the definition of AttributeContainer and the other way around and the definition of Attribute::NotifyParent can't
	 * be moved to the source (it is a template class method).
	 */

#ifdef _MSC_VER
	// See above
#else
	void NotifyParentWrapper(AttributeContainer* parent, const AttributeKey& key);
#endif
	/**
	 * Template specializations defining attribute types.
	 */
#ifdef _MSC_VER
	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Blob>::GetAttributeType()
	{
		return IAttribute::BLOB;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<bool>::GetAttributeType()
	{
		return IAttribute::BOOL;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<double>::GetAttributeType()
	{
		return IAttribute::DOUBLE;
	}
	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<float>::GetAttributeType()
	{
		return IAttribute::FLOAT;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<int32_t>::GetAttributeType()
	{
		return IAttribute::INT32;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<int64_t>::GetAttributeType()
	{
		return IAttribute::INT64;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Matrix4x4>::GetAttributeType()
	{
		return IAttribute::MATRIX4X4;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Quaternion>::GetAttributeType()
	{
		return IAttribute::QUATERNION;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Range>::GetAttributeType()
	{
		return IAttribute::RANGE;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<String>::GetAttributeType()
	{
		return IAttribute::STRING;
	}
	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Vector2>::GetAttributeType()
	{
		return IAttribute::VECTOR2;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Vector3>::GetAttributeType()
	{
		return IAttribute::VECTOR3;
	}

	// -------------------------------------------------------------------------
	template <> unsigned int Attribute<Vector4>::GetAttributeType()
	{
		return IAttribute::VECTOR4;
	}
#else
	template <> unsigned Attribute<Blob>::GetAttributeType();
	template <> unsigned Attribute<bool>::GetAttributeType();
	template <> unsigned Attribute<double>::GetAttributeType();
	template <> unsigned Attribute<float>::GetAttributeType();
	template <> unsigned Attribute<int32_t>::GetAttributeType();
	template <> unsigned Attribute<int64_t>::GetAttributeType();
	template <> unsigned Attribute<Matrix4x4>::GetAttributeType();
	template <> unsigned Attribute<Quaternion>::GetAttributeType();
	template <> unsigned Attribute<Range>::GetAttributeType();
	template <> unsigned Attribute<String>::GetAttributeType();
	template <> unsigned Attribute<Vector2>::GetAttributeType();
	template <> unsigned Attribute<Vector3>::GetAttributeType();
	template <> unsigned Attribute<Vector4>::GetAttributeType();
#endif // _MSC_VER


	template <> Blob Attribute<Blob>::DefaultValue();
	template <> bool Attribute<bool>::DefaultValue();
	template <> double Attribute<double>::DefaultValue();
	template <> float Attribute<float>::DefaultValue();
	template <> int32_t Attribute<int32_t>::DefaultValue();
	template <> int64_t Attribute<int64_t>::DefaultValue();
	template <> Matrix4x4 Attribute<Matrix4x4>::DefaultValue();
	template <> Quaternion Attribute<Quaternion>::DefaultValue();
	template <> Range Attribute<Range>::DefaultValue();
	template <> String Attribute<String>::DefaultValue();
	template <> Vector2 Attribute<Vector2>::DefaultValue();
	template <> Vector3 Attribute<Vector3>::DefaultValue();
	template <> Vector4 Attribute<Vector4>::DefaultValue();
}

#endif // Attribute_h__
