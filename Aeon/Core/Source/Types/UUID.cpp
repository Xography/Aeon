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
#include <Aeon/Serialization/UUID.pb.h>

#include <Aeon/Types/UUID.h>

#include <Aeon/Platform/Platform.h>
#include <Aeon/Logging/Logging.h>
#include <Aeon/Util/MurmurHash3.h>

#ifdef AEON_WINDOWS
#if _MSC_VER < 1700
#	include <Windows.h>
#else
#	include <chrono>
#endif
#elif defined(AEON_LINUX)
#	include <time.h>
#endif // AEON_WINDOWS


namespace Aeon {

	// -------------------------------------------------------------------------
	UUID::UUID(const String& name)
	{
		// Note: This does not strictly follow RFC 4122
		// 
		// Layout  [            128  Bits            ]
		// Data    [ 32 ][            96 Bits        ]
		// Input:  [hash][namespace counter timestamp]
		// Output: [hash][        hashed data        ]

		// Make sure no two threads are getting a good timing value at the same time (in rare conditions two entities with exactly
		// the same key could be generating an ID at the exact same moment).
		uint64_t clock = 0;

#ifdef AEON_WINDOWS
#if _MSC_VER < 1700
		// Use the Windows performance counter as clock value.
		static LARGE_INTEGER counter;
		BOOL b = QueryPerformanceCounter(&counter);
		assert(b);

		clock = counter.QuadPart;
#else
		std::chrono::nanoseconds ns = std::chrono::high_resolution_clock::now().time_since_epoch();
		std::chrono::duration_cast<std::chrono::duration<long long, std::nano> >(ns);
		clock = ns.count();
#endif

#elif defined(AEON_LINUX)
		// Use nanoseconds as clock value.
		timespec spec;
		clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &spec);
		clock = spec.tv_nsec;
#endif // AEON_WINDOWS
	

		// Seed our data
		static uint32_t counterFeed = 0;
		static uint32_t hash;
		MurmurHash3_x86_32(name.data(),name.length(), 0x9747b28c, &hash);

		data[0] = (uint64_t)(1095061326 + (++counterFeed)) << 32; // AEON (41 45 4f 4e) + counter
		data[1] = clock;

		MurmurHash3_x86_128(data, sizeof(uint32_t)* 4, 0x9747b28c, data);
		data[0] = (static_cast<uint64_t>(hash)) << 32 | ((data[0] >> 32) & 0xFFFFFFFF);
	}

	// -------------------------------------------------------------------------
	UUID::UUID(const Serialize::UUID& data)
	{
		this->data[0] = data.hi();
		this->data[1] = data.lo();
	}

	// -------------------------------------------------------------------------
	UUID::UUID(const UUID& rhs)
	{
		data[0] = rhs.data[0];
		data[1] = rhs.data[1];
	}

	// -------------------------------------------------------------------------
	UUID::UUID(uint64_t a, uint64_t b)
	{
		data[0] = a;
		data[1] = b;
	}

	// -------------------------------------------------------------------------
	UUID::~UUID()
	{

	}
	// -------------------------------------------------------------------------
	const uint64_t* UUID::GetData() const
	{
		return data;
	}

	// -------------------------------------------------------------------------
	void UUID::Serialize(const UUID& src, Serialize::UUID* target)
	{
		target->set_hi(src.GetData()[0]);
		target->set_lo(src.GetData()[1]);
	}

	// -------------------------------------------------------------------------
	uint32_t UUID::GetHash() const
	{
		return (data[0] >> 32) & 0xFFFFFFFF;
	}

	// -------------------------------------------------------------------------
	bool UUID::operator!=(const UUID& rhs) const
	{
		return	data[0] != rhs.data[0] &&
				data[1] != rhs.data[1];			
	}

	// -------------------------------------------------------------------------
	bool UUID::operator==(const UUID& rhs) const
	{
		return	data[0] == rhs.data[0] &&
				data[1] == rhs.data[1];			
	}

	// -------------------------------------------------------------------------
	bool UUID::operator<(const UUID& rhs) const
	{
		return	GetKeyHash() < rhs.GetKeyHash();			
	}
	
	// -------------------------------------------------------------------------
	uint32_t UUID::GetKeyHash() const
	{
		uint32_t hash = 0;
		MurmurHash3_x86_32(data, 2*sizeof(uint64_t), 0x9747b28c, &hash);
		return hash;
	}

	// -------------------------------------------------------------------------
	const UUID& UUID::Null()
	{
		static UUID nullID(0,0);
		return nullID;
	}


}
