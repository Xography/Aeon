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
#include <Aeon/AttributeContainer.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	#define ASSIGN_IF(EnumType,RealType) \
	case IAttribute::EnumType: \
	{ \
		AddAttribute<RealType>(attName,static_cast<Attribute<RealType>* >(attrib)->GetCopy()); \
		break; \
	} \

	// -------------------------------------------------------------------------
	uint32_t AttributeContainer::CheckAttribute(const AttributeKey& key)
	{
		Attributes::iterator it = attributes.find(key);

		if(it != attributes.end())
			return it->second->GetType();

		return IAttribute::INVALID;
	}

	// -------------------------------------------------------------------------
	void AttributeContainer::DestroyAttribute(IAttribute* attr)
	{
		if(!attr)
			return;

		if(attr->GetType() == IAttribute::BLOB)
		{
			Attribute<Blob>* blob = static_cast<Attribute<Blob>*>(attr);
			blob->GetMutable()->Free();
		}

		delete attr;
	}
	
	// -------------------------------------------------------------------------
	void AttributeContainer::ClearAttributes()
	{
		auto attributesIterator = attributes.begin();

		while(attributesIterator != attributes.end())
		{
			IAttribute* ptr = attributesIterator->second;
			attributesIterator = attributes.erase(attributesIterator);
			DestroyAttribute(ptr);
		}

		attributes.clear();
	}

	// -------------------------------------------------------------------------
	void AttributeContainer::AssignCopy(IAttribute* attrib)
	{
		if(!attrib)
			return;

		const AttributeKey& attName = attrib->GetKey();

		switch(attrib->GetType())
		{
			ASSIGN_IF(BLOB, Blob);
			ASSIGN_IF(BOOL, bool);
			ASSIGN_IF(DOUBLE, double);
			ASSIGN_IF(FLOAT, float);
			ASSIGN_IF(INT32, int32_t);
			ASSIGN_IF(INT64, int64_t);
			ASSIGN_IF(MATRIX4X4, Matrix4x4);
			ASSIGN_IF(QUATERNION, Quaternion);
			ASSIGN_IF(RANGE, Range);
			ASSIGN_IF(STRING, String);
			ASSIGN_IF(VECTOR2, Vector2);
			ASSIGN_IF(VECTOR3, Vector3);
			ASSIGN_IF(VECTOR4, Vector4);

		default:
			LOG_WARNING << "Unable to assign copy of attribute";
			break;
		}
	}

	// -------------------------------------------------------------------------
	void AttributeContainer::AssignAttribute(IAttribute* ptr)
	{
		if(!ptr) return;

		this->attributes[ptr->GetKey()] = ptr;
	}

}
