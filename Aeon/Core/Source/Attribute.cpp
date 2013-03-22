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
#include <Aeon/Serialization/Attribute.pb.h>
#include <Aeon/Serialization/Value.pb.h>
#include <Aeon/Attribute.h>
#include <Aeon/AttributeContainer.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	// Macro that helps us to copy a value into a descriptor
	// Parameters: 
	// aeon_type: Return of IAttribute::GetType()
	// funct_name: name of the function (type) in protobuf
	// val_type: native c++ type
#define COPY_VALUE_TO_DESCRIPTOR(aeon_type, funct_name, val_type) \
		case IAttribute::aeon_type: \
	{ \
	Attribute<val_type>* attribute = reinterpret_cast<Attribute<val_type>* >(ptr); \
	val->set_##funct_name##_value(attribute->Get()); \
	break; \
} \

	// -------------------------------------------------------------------------
	// Assign Matrix row
#define ASSIGN_ROW(row_id) \
	Aeon::Serialize::Vector4* vec##row_id = dest->mutable_x##row_id(); \
	vec##row_id->set_x0(src.GetX##row_id().GetX()); \
	vec##row_id->set_x1(src.GetX##row_id().GetY()); \
	vec##row_id->set_x2(src.GetX##row_id().GetZ()); \
	vec##row_id->set_x3(src.GetX##row_id().GetW()); \

	// -------------------------------------------------------------------------
	//  Macro to ssign all 4 matrix rows
#define ASSIGN_MATRIX_ROWS \
	ASSIGN_ROW(0) \
	ASSIGN_ROW(1) \
	ASSIGN_ROW(2) \
	ASSIGN_ROW(3) \

	// -------------------------------------------------------------------------
	// Macro to create the variables to store a complex type into an descriptor
#define COMPLEX_TYPE_TO_DESCRIPTOR(aeon_type, funct_name) \
	Attribute<aeon_type>* attribute = reinterpret_cast<Attribute<aeon_type>* >(ptr);  \
	const aeon_type& src = attribute->Get(); \
	Aeon::Serialize::aeon_type* dest = val->mutable_##funct_name##_value(); \

	// -------------------------------------------------------------------------
	IAttribute* IAttribute::DeserializeAttribute(const AttributeKey& key, const Serialize::Value& val, AttributeContainer* parent)
	{
		if(key.key.empty()) return 0;
		
		if(val.has_blob_value())
		{
			Attribute<Blob>* blob = Attribute<Blob>::Create(key, Blob(), parent);
			blob->GetMutable()->AssignCopy(reinterpret_cast<const uint8_t*>(val.blob_value().data()), val.blob_value().size());
			return blob;
		}
		
		else if(val.has_boolean_value()) 		return Attribute<bool>::Create(key, val.boolean_value(), parent);
		else if(val.has_double_value()) 		return Attribute<double>::Create(key, val.double_value(), parent);
		else if(val.has_float_value()) 			return Attribute<float>::Create(key, val.float_value(), parent);
		else if(val.has_int_value()) 			return Attribute<int32_t>::Create(key, val.int_value(), parent);
		else if(val.has_long_value()) 			return Attribute<int64_t>::Create(key, val.long_value(), parent);
		else if(val.has_string_value()) 		return Attribute<String>::Create(key, val.string_value(), parent);

		if(val.has_matrix4x4_value())
		{
			const Aeon::Serialize::Matrix4x4& m = val.matrix4x4_value();
			Attribute<Matrix4x4>* attribute = Attribute<Matrix4x4>::Create(key, 
				Matrix4x4(m.x0().x0(), m.x0().x1(), m.x0().x2(), m.x0().x3(), 
				m.x1().x0(), m.x1().x1(), m.x1().x2(), m.x1().x3(), 
				m.x2().x0(), m.x2().x1(), m.x2().x2(), m.x2().x3(), 
				m.x3().x0(), m.x3().x1(), m.x3().x2(), m.x3().x3()), parent);
			return attribute;
		}
		if(val.has_quaternion_value())
		{
			const Aeon::Serialize::Quaternion& q = val.quaternion_value();
			return Attribute<Quaternion>::Create(key,Quaternion(q.x(),q.y(),q.z(),q.w()),parent);
		}
		if(val.has_range_value())
		{
			const Aeon::Serialize::Range& r = val.range_value();
			return Attribute<Range>::Create(key,Range(r.start(),r.end(), r.value()),parent);
		}
		if(val.has_vector2_value())
		{
			const Aeon::Serialize::Vector2& v = val.vector2_value();
			return Attribute<Vector2>::Create(key,Vector2(v.x(), v.y()),parent);
		}
		if(val.has_vector3_value())
		{
			const Aeon::Serialize::Vector3& v = val.vector3_value();
			return Attribute<Vector3>::Create(key,Vector3(v.x(), v.y(), v.z()),parent);
		}
		if(val.has_vector4_value())
		{
			const Aeon::Serialize::Vector4& v = val.vector4_value();
			return Attribute<Vector4>::Create(key,Vector4(v.x0(), v.x1(), v.x2(), v.x3()),parent);
		}

		LOG_WARNING << "Attribute has no valid data, returning 0";
		return 0;
	}

	// -------------------------------------------------------------------------
	void IAttribute::SerializeAttribute(IAttribute* ptr, Serialize::Attribute* target)
	{
		if(!ptr || !target)
		{
			LOG_WARNING << "Unable to serialize attribute: Null data";
			return;
		}

		target->mutable_key()->set_name(ptr->GetKey().key);
		SerializeValue(ptr, target->mutable_value());
	}

	// -------------------------------------------------------------------------
	void IAttribute::SerializeValue(IAttribute* ptr, Serialize::Value* val)
	{
		if(ptr->GetType() == IAttribute::INVALID)
		{
			LOG_WARNING << "Cannot serialize attribute with key " << ptr->GetKey() << ". Attribute Value will be 0.";
			return;
		}

		switch(ptr->GetType())
		{
			case IAttribute::BLOB:
			{
				Attribute<Blob>* attribute = reinterpret_cast<Attribute<Blob>* >(ptr); 
				val->set_blob_value(attribute->GetCopy().GetBlob().data, attribute->Get().Size()); 
				break; 
			}
			
			COPY_VALUE_TO_DESCRIPTOR(FLOAT, float, float)
			COPY_VALUE_TO_DESCRIPTOR(BOOL, boolean, bool)
			COPY_VALUE_TO_DESCRIPTOR(DOUBLE, double, double)
			COPY_VALUE_TO_DESCRIPTOR(INT32, int, int32_t)
			COPY_VALUE_TO_DESCRIPTOR(INT64, long, int64_t)
			COPY_VALUE_TO_DESCRIPTOR(STRING, string, String)
			
			case IAttribute::MATRIX4X4:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Matrix4x4, matrix4x4)
					ASSIGN_MATRIX_ROWS
					break;
			}
			case IAttribute::QUATERNION:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Quaternion, quaternion)

				dest->set_x(src.GetVector4().GetX());
				dest->set_y(src.GetVector4().GetY());
				dest->set_z(src.GetVector4().GetZ());
				dest->set_w(src.GetVector4().GetW());
				break;
			}
			case IAttribute::RANGE:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Range, range)

				dest->set_start(src.GetLowerBound());
				dest->set_end(src.GetUpperBound());
				dest->set_value(src.GetValue());
				break;
			}
			case IAttribute::VECTOR2:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Vector2, vector2)

					dest->set_x(src.GetX());
				dest->set_y(src.GetY());
				break;
			}
			case IAttribute::VECTOR3:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Vector3, vector3)

				dest->set_x(src.GetX());
				dest->set_y(src.GetY());
				dest->set_z(src.GetZ());
				break;
			}
			case IAttribute::VECTOR4:
			{
				COMPLEX_TYPE_TO_DESCRIPTOR(Vector4, vector4)

				dest->set_x0(src.GetX());
				dest->set_x1(src.GetY());
				dest->set_x2(src.GetZ());
				dest->set_x3(src.GetW());
				break;
			}
			default:
				// Assign no attribute
				LOG_WARNING << "Unable to assign attribute!";
				break;
		}
	}

#ifndef _MSC_VER
	// -------------------------------------------------------------------------
	extern void NotifyParentWrapper(AttributeContainer* parent, const AttributeKey& key)
	{
		parent->AttributeUpdated(key);
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Blob>::GetAttributeType()
	{
		return IAttribute::BLOB;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<bool>::GetAttributeType()
	{
		return IAttribute::BOOL;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<double>::GetAttributeType()
	{
		return IAttribute::DOUBLE;
	}
	// -------------------------------------------------------------------------
	template <> unsigned Attribute<float>::GetAttributeType()
	{
		return IAttribute::FLOAT;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<int32_t>::GetAttributeType()
	{
		return IAttribute::INT32;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<int64_t>::GetAttributeType()
	{
		return IAttribute::INT64;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Matrix4x4>::GetAttributeType()
	{
		return IAttribute::MATRIX4X4;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Quaternion>::GetAttributeType()
	{
		return IAttribute::QUATERNION;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Range>::GetAttributeType()
	{
		return IAttribute::RANGE;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<String>::GetAttributeType()
	{
		return IAttribute::STRING;
	}
	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Vector2>::GetAttributeType()
	{
		return IAttribute::VECTOR2;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Vector3>::GetAttributeType()
	{
		return IAttribute::VECTOR3;
	}

	// -------------------------------------------------------------------------
	template <> unsigned Attribute<Vector4>::GetAttributeType()
	{
		return IAttribute::VECTOR4;
	}
#endif // _MSC_VER
	// -------------------------------------------------------------------------
	extern void NotifyParentWrapper(AttributeContainer* parent, const AttributeKey& key)
	{
		parent->AttributeUpdated(key);
	}

	// -------------------------------------------------------------------------
	template <> Blob Attribute<Blob>::DefaultValue()
	{
		return Blob();
	}

	// -------------------------------------------------------------------------
	template <> bool Attribute<bool>::DefaultValue()
	{
		return false;
	}

	// -------------------------------------------------------------------------
	template <> double Attribute<double>::DefaultValue()
	{
		return 0.0;
	}
	// -------------------------------------------------------------------------
	template <> float Attribute<float>::DefaultValue()
	{
		return 0.f;
	}

	// -------------------------------------------------------------------------
	template <> int32_t Attribute<int32_t>::DefaultValue()
	{
		return 0;
	}

	// -------------------------------------------------------------------------
	template <> int64_t Attribute<int64_t>::DefaultValue()
	{
		return 0;
	}

	// -------------------------------------------------------------------------
	template <> Matrix4x4 Attribute<Matrix4x4>::DefaultValue()
	{
		return Matrix4x4::Zero();
	}

	// -------------------------------------------------------------------------
	template <> Quaternion Attribute<Quaternion>::DefaultValue()
	{
		return Quaternion::Zero();
	}

	// -------------------------------------------------------------------------
	template <> Range Attribute<Range>::DefaultValue()
	{
		return Range(0.f,1.f);
	}

	// -------------------------------------------------------------------------
	template <> String Attribute<String>::DefaultValue()
	{
		return String();
	}
	// -------------------------------------------------------------------------
	template <> Vector2 Attribute<Vector2>::DefaultValue()
	{
		return Vector2::Zero();
	}

	// -------------------------------------------------------------------------
	template <> Vector3 Attribute<Vector3>::DefaultValue()
	{
		return Vector3::Zero();
	}

	// -------------------------------------------------------------------------
	template <> Vector4 Attribute<Vector4>::DefaultValue()
	{
		return Vector4::Zero();
	}
}
