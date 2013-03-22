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
#include <cstring>
#include <climits>

#include <Aeon/Memory/MemoryPool.h>

#define BIT_SET(x, bit)				((x) |= (1UL << (bit)))				// Sets the specified bit in the bitset.
#define BIT_ISSET(x, bit)			((x) & (1UL << (bit)))				// Checks if the specified bit is set in the bitset.
#define BIT_CLEAR(x, bit)			((x) &= ~(1UL << (bit)))			// Clears the specified bit in the bitset.
#define MAX_INTEGER_BITSIZE			(sizeof(size_t) * 8)

namespace Aeon {
	// -------------------------------------------------------------------------
	MemoryPool::MemoryPool(size_t chunkSize, size_t chunkCount, size_t alignment):
		tryAgain(true),
		chunkCount(chunkCount),
		chunkSize(chunkSize),
		alignment(alignment),
		lastBitmap(0)
	{
		// Expect at least the five chunks to fit in the pool (to prevent obscenely high values).
		assert(alignment > 0);
		assert(chunkSize > 0);
		assert(chunkSize < (UINTMAX_MAX / 5));

		// Uphold this condition, since when using alignment, we'll need to allocate one extra chunk so we can shift the first block around
		// a bit (operator new or malloc don't support alignment). If it's not upheld, that extra chunk of memory might not be enough for
		// all the chunks to fit.
		assert(alignment <= chunkSize);

		if((chunkSize % alignment) > 0)
			alignedChunkSize = alignment * ((chunkSize / alignment) + 1);
		
		else
			alignedChunkSize = chunkSize;

		// See how large our bitmap needs to be.
		size_t extraBitsNeeded = (chunkCount % MAX_INTEGER_BITSIZE);

		bitmapCount = (chunkCount / MAX_INTEGER_BITSIZE);
		
		if(extraBitsNeeded != 0)
			++bitmapCount; // We need an extra bitmap for a couple of chunks.

		// Add one chunk of extra memory so we can shift the first block forward when aligning.
		totalPoolSize = (alignedChunkSize * chunkCount + (alignment > 1? alignedChunkSize : 0));
		
		pool = new uint8_t[totalPoolSize];
		bitmaps = new size_t[bitmapCount];

		memset(pool, 0, sizeof(uint8_t) * totalPoolSize);
		memset(bitmaps, 0, sizeof(size_t) * bitmapCount);

		// Calculate the starting point of the first chunk (with alignment taken into account).
		if(alignment > 1)
			alignedPoolStart = reinterpret_cast<uint8_t*>(reinterpret_cast<size_t>(pool) + (alignment - (reinterpret_cast<size_t>(pool) % alignment)));

		else
			alignedPoolStart = pool;

		if(extraBitsNeeded)
		{
			// If we need a couple of extra bits (for example 5), the remaining bits (on 32-bits in this case 32 - 5 = 27) need to be set so
			// the pool allocator will ignore them and see them as 'set' (otherwise it will return chunks outside the memory block range).
			for(size_t i(extraBitsNeeded); i < MAX_INTEGER_BITSIZE; ++i)
				BIT_SET(bitmaps[bitmapCount - 1], i);
		}
	}

	// -------------------------------------------------------------------------
	MemoryPool::~MemoryPool()
	{
		delete[] bitmaps;
		delete[] pool;
	}

	// -------------------------------------------------------------------------
	const char* MemoryPool::GetName() const
	{
		return "Memory Pool";
	}

	// -------------------------------------------------------------------------
	String MemoryPool::GetStatistics() const
	{
		std::ostringstream s;

#if _DEBUG == 1
		s << "Statistics for " << GetName() << '\n'
		  << "----------------------------------------\n"
		  << "Each number (0 = free, 1 = occupied) symbolizes one chunk.\n"
		  << "Start address | Bitmap of chunk availability (1 = occupied)\n"
		  << "----------------------------------------\n";

		size_t allocatedBlocks(0);

		for(size_t i(0), j; i < bitmapCount; ++i)
		{
			s << std::hex << "0x" << reinterpret_cast<size_t>(&alignedPoolStart[(i * MAX_INTEGER_BITSIZE) * alignedChunkSize]) << ' ' << std::dec;

			size_t extraBitsNeeded = (chunkCount % MAX_INTEGER_BITSIZE);

			for(j = 0; j < MAX_INTEGER_BITSIZE; ++j)
			{
				if(i == (bitmapCount - 1) && extraBitsNeeded && j >= extraBitsNeeded)
					break;

				if(BIT_ISSET(bitmaps[i], j))
				{
					++allocatedBlocks;
					s << '1';
				}

				else
					s << '0';
			}

			s << '\n';
		}
		
		s << "----------------------------------------\n"
		  << "Chunk size (aligned): " << chunkSize << " (" << alignedChunkSize << ")\n"
		  << "Chunk count: " << chunkCount << '\n'
		  << "Alignment: " << alignment << '\n'
		  << "Amount of allocated blocks: " << allocatedBlocks << '\n'
		  << "Amount of allocated bytes: " << (allocatedBlocks * chunkSize) << '\n'
		  << "Memory pool size (usable): " << totalPoolSize << " (" << (chunkCount * chunkSize) << ")\n"
		  << "----------------------------------------\n";
#else
		s << "Memory tracking is deactivated in non-debug builds for performance reasons";
#endif // _DEBUG

		return s.str();
	}

	// -------------------------------------------------------------------------
	void* MemoryPool::Allocate()
	{
		static_assert(sizeof(unsigned long) == sizeof(size_t), "This code expects the size of unsigned long and size_t must be the same because it makes use of ULONG_MAX to get the maximum value of a size_t!");

		while(lastBitmap < bitmapCount)
		{
			//std::cout << "Bitmap " << lastBitmap << ": " << std::hex << " 0x" << bitmaps[lastBitmap] << std::dec;
			
			// Do a quick test to see if all bits are set.
			if(bitmaps[lastBitmap] != ULONG_MAX)
			{
				//std::cout << " [Checking] ";

				for(size_t j(0); j < MAX_INTEGER_BITSIZE; ++j)
				{				
					if(!BIT_ISSET(bitmaps[lastBitmap], j))
					{
						//std::cout << "[Bit " << j << "] [uint8_t " << (((lastBitmap * bitSize) + j) * alignedChunkSize) << "] " << '\n';

						tryAgain = true;

						BIT_SET(bitmaps[lastBitmap], j);
						return &alignedPoolStart[((lastBitmap * MAX_INTEGER_BITSIZE) + j) * alignedChunkSize];
					}
				}
			}

			//std::cout << '\n';

			++lastBitmap;
		}

		// Still here? Then there must have been no chunks left.
		if(tryAgain)
		{
			lastBitmap = 0;
			tryAgain = false;

			if(void* p = Allocate())
				return p;
		}
		
		// This should only happen when there are no more free chunks.
		lastBitmap = 0;
		return nullptr;
	}

	// -------------------------------------------------------------------------
	void* MemoryPool::Allocate(size_t size)
	{
		if(size > chunkSize)
		{
			LOG_DEBUG << "MemoryPool::Allocate requested with too large chunk size, returning null!";
			return nullptr;
		}

		return Allocate();
	}

	// -------------------------------------------------------------------------
	void* MemoryPool::Reallocate(void* mem, size_t size)
	{
		LOG_DEBUG << "Tried to reallocate a block from the memory pool, this can't happen!";
		return nullptr;
	}

	// -------------------------------------------------------------------------
	void MemoryPool::Deallocate(void* mem)
	{
		bool valid(false);

		if(!ContainsBlock(mem, &valid))
			LOG_DEBUG << "Tried to deallocate block that is not in the memory pool!";

		else if(!valid)
			LOG_DEBUG << "Tried to deallocate block that is not a valid chunk in the memory pool (wrong alignment)!";

		else
		{
			size_t offset = (reinterpret_cast<size_t>(mem) - reinterpret_cast<size_t>(alignedPoolStart));

			// Translate the offset into the bit that is used to describe its availability.
			size_t absoluteBit = (offset / alignedChunkSize);
			size_t relativeBit = (absoluteBit % MAX_INTEGER_BITSIZE);
			size_t bitmapIndex = (absoluteBit / MAX_INTEGER_BITSIZE);

			BIT_CLEAR(bitmaps[bitmapIndex], relativeBit);
		}
	}

	// -------------------------------------------------------------------------
	size_t MemoryPool::RetrieveMemorySize(void* mem) const
	{
		return chunkSize;
	}

	// -------------------------------------------------------------------------
	bool MemoryPool::ContainsBlock(void* block, bool* isValid) const
	{
		if(isValid)
			(*isValid) = false;

		size_t offset = (reinterpret_cast<size_t>(block) - reinterpret_cast<size_t>(alignedPoolStart));

		if(reinterpret_cast<size_t>(block) < reinterpret_cast<size_t>(alignedPoolStart) || offset > (reinterpret_cast<size_t>(alignedPoolStart) + (chunkCount * alignedChunkSize)))
			return false;

		if(isValid)
			(*isValid) = ((offset % alignedChunkSize) == 0);

		return true;
	}
}
