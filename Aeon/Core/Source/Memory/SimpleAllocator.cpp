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
#include <sstream>

#include <Aeon/Memory/SimpleAllocator.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	SimpleAllocator::SimpleAllocator():
		allocatedBlocks(0),
		allocatedMemory(0)
	{

	}

	// -------------------------------------------------------------------------
	SimpleAllocator::~SimpleAllocator()
	{

	}

	// -------------------------------------------------------------------------
	const char* SimpleAllocator::GetName() const
	{
		return "Simple Allocator";
	}

	// -------------------------------------------------------------------------
	String SimpleAllocator::GetStatistics() const
	{
		std::ostringstream s;

#if _DEBUG == 1
		s << "Statistics for " << GetName() << '\n'
		  << "----------------------------------------\n"
		  << "Amount of allocated blocks: " << allocatedBlocks << '\n'
		  << "Amount of allocated bytes: " << allocatedMemory << '\n'
		  << "----------------------------------------\n";
#else
		s << "Memory tracking is deactivated in non-debug builds for performance reasons";
#endif // _DEBUG

		return s.str();
	}

	// -------------------------------------------------------------------------
	void* SimpleAllocator::Allocate(size_t size)
	{
#if _DEBUG == 1
		++allocatedBlocks;
		allocatedMemory += size;
#endif // _DEBUG

		return malloc(size);
	}

	// -------------------------------------------------------------------------
	void* SimpleAllocator::Reallocate(void* mem, size_t size)
	{
#if _DEBUG == 1
		if(mem)
			allocatedMemory -= RetrieveMemorySize(mem);
		
		++allocatedBlocks;
		allocatedMemory += size;
#endif // _DEBUG

		return realloc(mem, size);
	}

	// -------------------------------------------------------------------------
	void SimpleAllocator::Deallocate(void* mem)
	{
#if _DEBUG == 1
		--allocatedBlocks;
		allocatedMemory -= RetrieveMemorySize(mem);
#endif // _DEBUG

		free(mem);
	}

	// -------------------------------------------------------------------------
	size_t SimpleAllocator::RetrieveMemorySize(void* mem) const
	{
		#ifdef _MSC_VER
			return _msize(mem);
		#else
			return 0;
		#endif // AEON_WINDOWS
	}
}
