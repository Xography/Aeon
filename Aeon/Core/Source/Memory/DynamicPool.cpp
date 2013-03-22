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
#include <Aeon/Memory/DynamicPool.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	DynamicPool::DynamicPool(size_t chunkSize, size_t chunkCount, size_t alignment, float ratio):
		chunkSize(chunkSize),
		chunkCount(chunkCount),
		alignment(alignment),
		ratio(ratio)
	{
		assert(ratio > 0.0f);
		assert(static_cast<size_t>(static_cast<float>(chunkCount) * ratio) > 0); // New pools must at least have one chunk.

		MemoryPool *pool = new MemoryPool(chunkSize, chunkCount, alignment);
		pools.push_back(pool);
	}

	// -------------------------------------------------------------------------
	DynamicPool::~DynamicPool()
	{
		for(size_t i(0); i < pools.size(); ++i)
			delete pools[i];
	}

	// -------------------------------------------------------------------------
	const char* DynamicPool::GetName() const
	{
		return "Dynamic Memory Pool";
	}

	// -------------------------------------------------------------------------
	String DynamicPool::GetStatistics() const
	{
		std::ostringstream s;

#if _DEBUG == 1
		s << "Statistics for " << GetName() << '\n'
		  << "----------------------------------------\n"
		  << "Chunk size: " << chunkSize << '\n'
		  << "Chunk count: " << chunkCount << '\n'
		  << "Alignment: " << alignment << '\n'
		  << "Ratio: " << ratio << '\n'
		  << "Amount of pools: " << pools.size() << '\n'
		  << "----------------------------------------\n"
		  << "NOTE: Detailed information can be acquired per pool individually.\n"
		  << "----------------------------------------\n";
#else
		s << "Memory tracking is deactivated in non-debug builds for performance reasons";
#endif // _DEBUG

		return s.str();
	}

	// -------------------------------------------------------------------------
	size_t DynamicPool::GetPoolCount() const
	{
		return pools.size();
	}

	// -------------------------------------------------------------------------
	const MemoryPool *DynamicPool::GetFixedPool(size_t pool) const
	{
		assert(pool < pools.size());

		return pools[pool];
	}

	// -------------------------------------------------------------------------
	void* DynamicPool::Allocate()
	{
		for(size_t i(0); i < pools.size(); ++i)
			if(void* p = pools[i]->Allocate())
				return p;

		std::cout << "Allocating new pool...\n";

		// Still here? Then there were no pools with free blocks.
		MemoryPool *newPool = new MemoryPool(chunkSize, static_cast<size_t>(static_cast<float>(chunkCount) * ratio), alignment);
		pools.push_back(newPool);

		void* p = newPool->Allocate();
		assert(p);
		return p;
	}

	// -------------------------------------------------------------------------
	void* DynamicPool::Allocate(size_t size)
	{
		if(size > chunkSize)
		{
			LOG_DEBUG << "DynamicMemoryPool::Allocate requested with too large chunk size, returning null!";
			return nullptr;
		}

		return Allocate();
	}

	// -------------------------------------------------------------------------
	void* DynamicPool::Reallocate(void* mem, size_t size)
	{
		for(size_t i(0); i < pools.size(); ++i)
			if(pools[i]->ContainsBlock(mem))
				return pools[i]->Reallocate(mem, size);

		return nullptr;
	}

	// -------------------------------------------------------------------------
	void DynamicPool::Deallocate(void* mem)
	{
		for(size_t i(0); i < pools.size(); ++i)
		{
			if(pools[i]->ContainsBlock(mem))
			{
				pools[i]->Deallocate(mem);
				break;
			}
		}
	}

	// -------------------------------------------------------------------------
	size_t DynamicPool::RetrieveMemorySize(void* mem) const
	{
		for(size_t i(0); i < pools.size(); ++i)
			if(pools[i]->ContainsBlock(mem))
				return pools[i]->RetrieveMemorySize(mem);

		return 0;
	}
}
