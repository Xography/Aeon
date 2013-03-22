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
#ifndef MemoryPool_h__
#define MemoryPool_h__

#include <Aeon/Memory/IAllocator.h>

namespace Aeon {
	/**
	 * @brief Memory pool implementation with fixed chunk sizes and a fixed amount of chunks.
	 *
	 * @important Possible future optimization: automatically align chunks on dword or qword boundaries (depending on the architecture and size_t). Allow a boolean in the constructor to specify this.
	 * 
	 */
	class AEON_EXPORT MemoryPool : public IAllocator
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param chunkSize The size of one chunk in the pool. A block of this size will be retrieved when you allocate a new chunk.
		 * @param chunkCount The amount of chunks that should be available in the pool.
		 * @param alignment The amount of bytes to align each chunk on. A value of 1 is the default and means no special alignment. Any value higher than that will ensure that each chunk starts
		 * on a boundary where start_of_boundary % alignment == 0, e.g. an alignment of 4 can result in better performance on 32-bit systems, but will also take slightly more space (depending
		 * on the chunk size).
		 */
		MemoryPool(size_t chunkSize, size_t chunkCount, size_t alignment = 1);

		/**
		 * Destructor.
		 */
		~MemoryPool();

	public:
		/**
		 * Returns the name of the allocator.
		 */
		const char* GetName() const override;

		/**
		 * Returns a dump of the statistics of the pool.
		 */
		String GetStatistics() const override;

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

		/**
		 * Indicates if the specified block is in this pool.
		 *
		 * @param block The memory block to examine.
		 * @param isValid If this pointer is set, the boolean inside it will be set to indicate whether the specified block is a valid chunk inside the pool (i.e. it is a chunk that was returned by Allocate).
		 */
		bool ContainsBlock(void* block, bool* isValid = nullptr) const;

	private:
		/**
		 * Flag that indicates if the bitmaps should be checked again.
		 */
		bool tryAgain;

		/**
		 * The actual pool memory and the start of the pool memory after it has been aligned.
		 */
		uint8_t *pool, *alignedPoolStart;
		
		/**
		 * Bitmap used to check for free chunks.
		 */
		size_t *bitmaps;

		/**
		 * The amount of chunks the pool has room for (not the amount of chunks necessarily in use).
		 */
		size_t chunkCount;

		/**
		 * The amount of bitmaps in the bitmap.
		 */
		size_t bitmapCount;

		/**
		 * The alignment to use.
		 */
		size_t alignment;

		/**
		 * The size of one chunk and the size of a chunk if alignment has been taken into account (alignedChunkSize >= chunkSize).
		 */
		size_t chunkSize, alignedChunkSize;
		
		/**
		 * Total pool size (including extra memory that is unavailable due to alignment requirements).
		 */
		size_t totalPoolSize;

		/**
		 * The last checked bitmap.
		 */
		size_t lastBitmap;
	};
}

#endif // MemoryPool_h__
