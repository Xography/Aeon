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
#ifndef DynamicPool_h__
#define DynamicPool_h__

#include <Aeon/Memory/MemoryPool.h>

namespace Aeon {
	/**
	 * @brief Dynamic memory pool implementation with fixed chunk sizes and a fixed amount of chunks. This dynamic memory pool acts in exactly the same way as a standard memory pool, but it automatically
	 * creates new pools if the existing pool(s) it manages overflow(s). You should tune your pool based on your applications needs, the purpose of the dynamic pool is to only have to create a new
	 * pool in rare circumstances. Normally one pool should be enough, but in case it is ever to fill up entirely, the dynamic pool will make sure that the new chunks are accomodated.
	 *
	 * @see MemoryPool
	 */
	class AEON_EXPORT DynamicPool : public IAllocator
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param chunkSize The size of one chunk in the pool. A block of this size will be retrieved when you allocate a new chunk.
		 * @param chunkCount The amount of chunks that should be available in the first pool.
		 * @param alignment The amount of bytes to align each chunk on. A value of 1 is the default and means no special alignment. Any value higher than that will ensure that each chunk starts
		 * on a boundary where start_of_boundary % alignment == 0, e.g. an alignment of 4 can result in better performance on 32-bit systems, but will also take slightly more space (depending
		 * on the chunk size).
		 * @param ratio When the first fixed pool is completely filled up, this dynamic pool wil create a new fixed pool. The ratio determines how large this pool should be compared
		 * to the original pool (i.e. the new pool's chunk count will be chunkCount * ratio).
		 */
		DynamicPool(size_t chunkSize, size_t chunkCount, size_t alignment = 1, float ratio = 1);

		/**
		 * Destructor.
		 */
		~DynamicPool();

	public:
		/**
		 * Returns the name of the allocator.
		 */
		const char* GetName() const override;

		/**
		 * Returns a dump of the statistics of the pool.
		 */
		String GetStatistics() const override;

		/**
		 * Returns the amount of pools managed by the dynamic pool.
		 */
		size_t GetPoolCount() const;

		/**
		 * Returns the pool with the specified number.
		 *
		 * @param id The pool id, ranging from 0 to (GetPoolCount() - 1).
		 */
		const MemoryPool *GetFixedPool(size_t pool) const;

	public:
		/**
		 * Allocates a new block of memory in the pool. This block will be of the same size set in the constructor.
		 */
		void* Allocate();
		
		/**
		 * Allocates a new chunk of memory in the memory pool.
		 *
		 * @param size Chunks in the memory pool are always of the same size, but if the requested size is larger than the chunk size in the pool, null is returned.
		 *
		 * @return A new block of memory with the size of a chunk in the pool (set in the constructor) or a nullptr if the specified size is larger than the chunk size.
		 */
		void* Allocate(size_t size) override;

		/**
		 * Reallocates a memory block to the specified new size. This operation is not supported for pools and always returns null.
		 */
		void* Reallocate(void* mem, size_t size) override;

		/**
		 * Frees the specified block of memory from the pool. If the specified block is outside the pool or not at the start of a chunk, this function does nothing.
		 */
		void Deallocate(void* mem) override;

		/**
		 * Returns the total size occupied by the specified memory block. Returns 0 if the specified block is not a valid pool chunk.
		 */
		size_t RetrieveMemorySize(void* mem) const override;

	private:
		/**
		 * The memory pools managed by the dynamic pool.
		 */
		std::vector<MemoryPool *> pools;

		/**
		 * The size of one chunk.
		 */
		size_t chunkSize;

		/**
		 * The amount of chunks in the first fixed pool.
		 */
		size_t chunkCount;

		/**
		 * The alignment to use.
		 */
		size_t alignment;

		/**
		 * The ratio that determines the size of new pools with regards to the first fixed pool.
		 */
		float ratio;
	};

	/**
	 * Executes a test of the DynamicPool class, uses assertions for checking.
	 */
	void DynamicPoolTest();
}

#endif // DynamicPool_h__
