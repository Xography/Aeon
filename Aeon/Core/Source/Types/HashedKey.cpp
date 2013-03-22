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
 #include <Aeon/Types/HashedKey.h>

 #include <Aeon/Util/MurmurHash3.h>

 namespace Aeon {
	// -------------------------------------------------------------------------
	 HashedKey::HashedKey() :
		hash(0)
	{
	
	}

	// -------------------------------------------------------------------------
	HashedKey::HashedKey(const String &s) :
		key(s)
	{
		MurmurHash3_x86_32(s.data(), s.length(), 0x9747b28c, &hash);
	}


	// -------------------------------------------------------------------------
	HashedKey::HashedKey(uint32_t hash) :
		hash(hash)
	{

	}

	// -------------------------------------------------------------------------
	HashedKey::HashedKey(const String& s, uint32_t hash) :
		key(s),
		hash(hash)
	{

	}

	// -------------------------------------------------------------------------
	HashedKey::HashedKey(const HashedKey &rhs) : 
		key(rhs.key),
		hash(rhs.hash)
	{

	}

	// -------------------------------------------------------------------------
	bool HashedKey::operator==(const HashedKey& b) const
	{
		return b.hash == hash;
	}

	// -------------------------------------------------------------------------
	bool HashedKey::operator!=(const HashedKey& b) const
	{
		return b.hash != hash;
	}

	// -------------------------------------------------------------------------
	bool HashedKey::operator<(const HashedKey& b) const
	{
		return hash < b.hash;
	}

	// -------------------------------------------------------------------------
	bool HashedKey::IsNull() const
	{
		return (hash == 0);
	}

	// -------------------------------------------------------------------------
	HashedKey HashedKey::Null()
	{
		HashedKey null;
		null.hash = 0;
		return null;
	}

 }