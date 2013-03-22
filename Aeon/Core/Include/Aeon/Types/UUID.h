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
#ifndef UUID_h__
#define UUID_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/AeonForward.h>
#include <Aeon/Multicore/Types.h>

namespace Aeon {
	/**
	 * @brief UUID class providing unique object identifiers for the Aeon .
	 * 
	 * This class is not RFC 4122 (UUID v5) compliant! It is only intended for usage withing the scope of the framework 
	 * and its applications.
	 */
	class AEON_EXPORT UUID
	{
		struct Impl;
	public:
		/**
		 * Constructor, creates a namespace-based UUID (Aeon namespace) for the given input name.
		 */
		UUID(const String& name);

		/**
		 * Constructor, uses previously generated data.
		 */
		UUID(const Serialize::UUID& data);

		/**
		 * Dtor.
		 */
		virtual ~UUID();

	public:
		/**
		 * Copy constructor
		 */
		UUID(const UUID& rhs);

	public:
		/**
		 * Constructor, setting internal data directly.
		 */
		UUID(uint64_t a, uint64_t b);

	public:
		/**
		 * A "null" UUID to check against (if no null pointer is available).
		 */
		static const UUID& Null();

	public:

		/**
		 * Get the source hash used for this uuid.
		 * 
		 * Note that this hash never ensures uniqueness or validity. If the source hash has changed 
		 * after an uuid has been generated/persisted (i.e. a descriptor was renamed afterwards), this hash remains unchanged and thus will 
		 * never match or against the wrong descriptor. The proper usage of this hash would be quick runtime reverse-lookups where 
		 * you can be sure the parameters (i.e. hashes) do not change.
		 */
		uint32_t GetHash() const;
			
		/**
		 * Used for hashing functions, this does _not_ return the reverse-lookup hash.
		 */
		uint32_t GetKeyHash() const;

		/**
		 * Returns the internal UUID data structure.
		 */
		const uint64_t* GetData() const;

	public:
		/**
		 * Equality operator.
		 */
		bool operator==(const UUID& rhs) const;

		/**
		 * Inequality operator.
		 */
		bool operator!=(const UUID& rhs) const;

		/**
		 * Operator overload to help us keep keys in an order-preserving data structure.
		 */
		bool operator<(const UUID& rhs) const;

	public:
		/**
		 * Serializes thie given uuid into the given serialization target.
		 */
		static void Serialize(const UUID& src, Serialize::UUID* target);

	private:
		/**
		 * UUID Internal data
		 */
		uint64_t data[2];
	};
	
	/**
	 * Handles writing UUIDs to ostreams.
	 */
	inline std::ostream &operator<<(std::ostream& stream, const UUID& object)
	{		
		const uint64_t* data = object.GetData();
		stream	<< std::hex << ((data[0] >> 32) & 0xFFFFFFFF) << '-' << (data[0] & 0xFFFFFFFF) 
				<< '-'		<< ((data[1] >> 32) & 0xFFFFFFFF) << '-' << (data[1] & 0xFFFFFFFF); 
		return stream;
	}
	
	/**
	 * @brief UUID Hashing function
	 */
	struct UUIDKeyHashFunction
	{
		/**
		 * Operator overload, used by the C++ stdlib.
		 */
		uint32_t operator()(const UUID& uuid) const
		{
			return uuid.GetKeyHash();
		}
	};
	
	/**
	 * @brief Used for equality checks of an UUID instance.
	 */
	struct UUIDKeyEquality
	{	
		/**
		 * Operator overload, used by the C++ stdlib.
		 */
		bool operator()(const UUID& lhs, const UUID& rhs) const
		{
			return lhs == rhs;
		}
	};
}

#endif // UUID_h__
