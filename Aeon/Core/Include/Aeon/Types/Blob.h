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
#ifndef Blob_h__
#define Blob_h__

#include <cstdint>
#include <cstring>

#include <Aeon/Memory/DynamicPool.h>

namespace Aeon {
	/**
	 * @brief A blob class, wrapping an arbitary sequence of bytes.
	 */
	class AEON_EXPORT Blob
	{
	public:
		/**
		 * 
		 */
		struct MemoryBlock
		{
			uint8_t* data;
			size_t memSize;
		};

		/**
		 * 
		 */
		Blob():
			hasData(false)
		{
			mem.data = 0;
			mem.memSize = 0;
		}

		/**
		 * 
		 */
		Blob(size_t reservedSize):
			hasData(false)
		{
			mem.data = new uint8_t[reservedSize];
			mem.memSize = reservedSize;
		}
			
		/**
		 * 
		 */
		Blob(uint8_t* data, size_t size):
			hasData(false)
		{
			Init(data, size);
		}

		/**
		 *
		 */
		void Free()
		{
			delete[] mem.data;
			hasData = false;
		}

		/**
		 * 
		 */
		const MemoryBlock& GetBlob() const
		{
			return mem;
		}

		/**
		 * Copies the given data into this blob instance.
		 */
		void AssignCopy(const uint8_t* data, size_t size, bool free = true)
		{
			if(free && hasData) Free();
			Init(data, size);
		}

		/**
		 * Assign the data (shall not exceed the previously reserved size.)
		 */
		void Assign(uint8_t* data)
		{
			mem.data = data;
		}

		/**
		 * 
		 */
		size_t Size() const
		{
			return mem.data ? mem.memSize : 0;
		}

		/**
		 * 
		 */
		Blob(const Blob& rhs):
			hasData(rhs.hasData)
		{
			if(hasData)
			{
				Init(rhs.GetBlob().data, rhs.GetBlob().memSize);
			}
			else
			{
				mem.data = 0;
				mem.memSize = 0;
			}
		}
	private:
		/**
		 * 
		 */
		void Init(const uint8_t* data, size_t memSize)
		{
			if (!data || !memSize) return;

			mem.data = new uint8_t[memSize];
			mem.memSize = memSize;
			memcpy(mem.data, data,memSize);
			hasData = true;
		}

		/**
		 * 
		 */
		MemoryBlock mem;

		/**
		 * 
		 */
		bool hasData;

	};
}

#endif // Blob_h__