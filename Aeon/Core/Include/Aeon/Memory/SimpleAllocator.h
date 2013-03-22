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
#ifndef SimpleAllocator_h__
#define SimpleAllocator_h__

#include <Aeon/Memory/IAllocator.h>

namespace Aeon {
	/**
	 * @brief A simple memory allocator based on malloc, realloc and free from the standard library.
	 */
	class AEON_EXPORT SimpleAllocator : public IAllocator
	{
	public:
		/**
		 * Constructor.
		 */
		SimpleAllocator();

		/**
		 * Destructor.
		 */
		~SimpleAllocator();

	public:
		/**
		 * Retrieves the name of this allocator.
		 */
		const char* GetName() const override;

		/**
		 * Retrieves a memory dump from this allocator.
		 */
		String GetStatistics() const override;

	public:
		/**
		 * Allocates a new block of memory of the specified size.
		 */
		void* Allocate(size_t size) override;

		/**
		 * Reallocates the specified block of memory to the specified new size. Works like realloc from the C standard library.
		 */
		void* Reallocate(void* mem, size_t size) override;

		/**
		 * Deallocates or frees the specified block.
		 */
		void Deallocate(void* mem) override;

		/**
		 * Retrieves the size of the specified block of memory. This is only supported on Visual C++ compilers, it returns 0 on others.
		 */
		size_t RetrieveMemorySize(void* mem) const override;

	private:
		/**
		 * Used to track this allocator object and dumping the statistics.
		 */
		size_t allocatedBlocks, allocatedMemory;
	};
}

#endif // SimpleAllocator_h__
