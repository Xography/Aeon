/*******************************************************************************
 *
 * This file is part of the Aeon Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#include <Aeon/Memory/IAllocator.h>
#include <Aeon/Memory/DynamicPool.h>
#include <Aeon/Memory/MemoryPool.h>

#include <igloo/igloo.h>
using namespace igloo;
using namespace Aeon;

const void* nullPointer = nullptr; // for checking against nullpointers

/**
 * Test Cases for the Memory Pool classes.
 */
Context(Test_Aeon_MemoryPools)
{

	// -------------------------------------------------------------------------
	Spec(Test_DynamicPool)
	{
		const size_t chunkSize = 50;
		const size_t chunks = 120;
		const size_t alignment = 2;

		// Use a ratio that will always cause the newly allocated pool to have only one chunk.
		const float ratio = (1.0f / static_cast<float>(chunks));
		const float check = ratio * chunks;

		// Make sure this is a number that won't produce rounding errors.
		AssertThat(check, Is().EqualTo(1.0f));

		void* midP = nullptr;
		Aeon::DynamicPool pool(chunkSize, chunks, alignment, ratio);

		for(size_t i(0); i < chunks; ++i)
		{
			// Allocate all the chunks in the pool.
			void* p = pool.Allocate();
			AssertThat(p, Is().Not().EqualTo(nullPointer));

			if(i == (chunks / 2))
				midP = p;
		}

		// There should be no more chunks free now, but the dynamic pool should automatically create a new pool.
		AssertThat(pool.Allocate(), Is().Not().EqualTo(nullPointer));

		// Another pool should be created now (since the chunk size of the second pool should be one, see above).
		AssertThat(pool.Allocate(), Is().Not().EqualTo(nullPointer));

		// Free one chunk.
		size_t address = reinterpret_cast<size_t>(midP);
		pool.Deallocate(midP);

		// Allocating a new chunk should now use the previously allocated chunk.
		AssertThat(reinterpret_cast<size_t>(pool.Allocate()), Is().EqualTo(address));
	}

	// -------------------------------------------------------------------------
	Spec(Test_MemoryPool)
	{
		const size_t chunkSize = 60;
		const size_t chunks = 120;
		const size_t alignment = 16;

		bool isValid(false);
		void* midP = nullptr;
		Aeon::MemoryPool pool(chunkSize, chunks, alignment);

		for(size_t i(0); i < chunks; ++i)
		{
			// Allocate all the chunks in the pool.
			void* p = pool.Allocate();

			AssertThat(p, Is().Not().EqualTo(nullPointer));
			AssertThat(pool.ContainsBlock(p, &isValid), Is().Not().EqualTo(false));
			AssertThat(isValid, Is().EqualTo(true));

			if(i == (chunks / 2))
				midP = p;
		}

		// There should be no more chunks free now. The secondary pass should also yield no free chunks.
		AssertThat(pool.Allocate(), Is().EqualTo(nullPointer));
		AssertThat(pool.ContainsBlock(nullptr, &isValid), Is().EqualTo(false));
		AssertThat(isValid, Is().EqualTo(false));

		// Free one chunk.
		size_t address = reinterpret_cast<size_t>(midP);
		pool.Deallocate(midP);

		AssertThat((address % alignment), Is().EqualTo(0));

		// Allocating a new chunk should now use the previously allocated chunk.
		AssertThat(reinterpret_cast<size_t>(pool.Allocate()), Is().EqualTo(address));
	}
};