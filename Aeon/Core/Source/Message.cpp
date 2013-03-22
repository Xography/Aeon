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
#include <string>
#include <cassert>
#include <unordered_map>

#include <Aeon/Message.h>
#include <Aeon/Util/MurmurHash3.h>

#define MAGIC_SEED								0x9747B28C
#define MAX_MESSAGEID_CACHE_SIZE				64

namespace Aeon { 
	/*
	 * Message Type ID Code:
	 * Copyright 2010 Mohammad Abdulfatah. All rights reserved.
	 * 
	 * Licensed under the MIT license:
	 *
	 *     http://www.opensource.org/licenses/mit-license.php
	 *
	 */
	// -------------------------------------------------------------------------
	void EnsureUniqueIdOrDie(const char* typeName, unsigned int id)
	{
	#ifdef _DEBUG
		typedef std::unordered_map<unsigned int, const char*> id_to_type_map_t;

		static id_to_type_map_t idToTypeMap;
		
		id_to_type_map_t::const_iterator i = idToTypeMap.find(id);

		if (i != idToTypeMap.end())
		{
			std::string storedTypeName(i->second);
			assert(storedTypeName == std::string(typeName));
		}   
	#endif // _DEBUG
	}

	// -------------------------------------------------------------------------
	uint32_t GenerateMessageID(const char* typeName)
	{
		if(size_t len = strlen(typeName))
		{
			static std::unordered_map<String, uint32_t> idCache;

			// See if we cached this typename.
			auto i = idCache.find(typeName);

			if(i != idCache.end())
				return i->second;

			// We didn't, generate a new hash using MurmurHash.
			uint32_t id = 0;

			MurmurHash3_x86_32(typeName, len, 0xDEADBEEF, &id);

			if(id > 0)
			{
				// This is a valid hash, add it to the cache and ensure it is unique.
				EnsureUniqueIdOrDie(typeName, id);
				idCache[typeName] = id;
			}

			return id;
		}

		return 0;
	}

	// -------------------------------------------------------------------------
	Message::Message(uint32_t type):
		type(type)
	{

	}

	// -------------------------------------------------------------------------
	void Message::Destroy(Message* ptr)
	{
		delete ptr;
	}
}
