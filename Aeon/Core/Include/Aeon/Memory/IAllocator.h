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
#ifndef IAllocator_h__
#define IAllocator_h__

#include <Aeon/Logging/Logging.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief Interface for memory allocators.
	 */
	class AEON_EXPORT IAllocator
	{
	public:
		/**
		 * Virtual destructor.
		 */
		virtual ~IAllocator()
		{

		};

	public:
		/**
		 * Returns the name of the allocator.
		 */
		virtual const char* GetName() const = 0;

		/**
		 * Returns a dump of the memory allocated by this allocator.
		 */
		virtual String GetStatistics() const = 0;

	public:
		/**
		 * Allocates a new block of the specified size. Some allocators may not work with sizes for this call (e.g. pools), they may just ignore the
		 * parameter.
		 *
		 * @param size The size of the memory block to allocate (in bytes).
		 */
		virtual void* Allocate(size_t size) = 0;

		/**
		 * Reallocates the specified block of memory to the specified new size. This function should behave in the same way as C's realloc if it is
		 * supported.
		 *
		 * @param mem The block of memory to reallocate.
		 * @param size The new size to give to the block.
		 */
		virtual void* Reallocate(void* mem, size_t size) = 0;

		/**
		 * Deallocates or frees the specified block of memory. The block has to be a block that was previously allocated using Allocate in the same
		 * allocator.
		 *
		 * @param mem The memory block to free.
		 */
		virtual void Deallocate(void* mem) = 0;

		/**
		 * Returns the size of the specified block of memory allocated by this allocator.
		 */
		virtual size_t RetrieveMemorySize(void* mem) const = 0;
	};
}

#endif // IAllocator_h__
