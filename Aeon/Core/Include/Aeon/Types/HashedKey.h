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
#ifndef HashedKey_h__
#define HashedKey_h__

#include <iostream>
#include <Aeon/AeonTypes.h>

namespace Aeon {
	/**
	 * @brief Pre-hashed key pair for hash maps.
	 */
	struct AEON_EXPORT HashedKey 
	{
	public:
		/**
		 * Constructor.
		 */
		HashedKey();
		
		/**
		 * Constructor, generates and stores a hash from the string passed as key.
		 */
		explicit HashedKey(const String &s);
			
		/**
		 * Constructor, just holds an empty string and the hash.
		 */
		HashedKey(uint32_t hash);

		/**
		 * Constructor, sets both hash and key.
		 */
		HashedKey(const String& s, uint32_t hash);

		/**
		 * Copy constructor.
		 */
		HashedKey(const HashedKey &rhs);

	public:
		/**
		 * Equality operator.
		 */
		bool operator==(const HashedKey& b) const;

		/**
		 * Inequality operator.
		 */
		bool operator!=(const HashedKey& b) const;

		/**
		 * Operator overload to help us keep keys in an order-preserving data structure.
		 */
		bool operator<(const HashedKey& b) const;
		
	public:
		/**
		 * Returns a boolean indicating of this hashed key is 'null' or invalid.
		 * 
		 */
		bool IsNull() const;

		static HashedKey Null();
	public:
		/**
		 * 
		 */
		String key;
		
		/**
		 * 
		 */
		uint32_t hash;
	};

	/**
	 * Handles writing hashed keys to ostreams.
	 */
	inline std::ostream& operator<<(std::ostream &f, const HashedKey &obj)
	{
		f <<  obj.key << "(" << std::hex << obj.hash << ")";
		return f;
	}

	/**
	 * @brief Hash key "function", used for hash maps.
	 */
	struct AggregateKeyHash
	{
		/**
		 * Operator overload, used by the C++ stdlib.
		 */
		long operator()(const HashedKey &k) const
		{
			return k.hash;
		}
	};
	
	/**
	 * @brief Used for equality checks, used for hash maps.
	 */
	struct AggregateKeyEquality
	{	
		/**
		 * Operator overload, used by the C++ stdlib.
		 */
		bool operator()(const HashedKey &lhs, const HashedKey &rhs) const
		{
			return lhs.hash == rhs.hash;	
		}
	};
}

#endif // HashedKey_h__