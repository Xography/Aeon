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
#include <Aeon/Types/UUID.h>
#include <Aeon/Logging/Logging.h>
#include <Aeon/ProcessManager.h>

#include "AeonTestUtil.h"

#include <igloo/igloo.h>

using namespace igloo;

bool aFailed = false;
bool bFailed = false;

// -------------------------------------------------------------------------
void UUIDProcessA()
{
	Aeon::UUID prev("Test");
	THREAD_BEGIN_LOOP
	{
		 Aeon::UUID a("Test");

		if(prev == a)
		{
			aFailed = true;
		}
	}
	THREAD_END_LOOP
}

// -------------------------------------------------------------------------
void UUIDProcessB()
{

	Aeon::UUID prev("Test");
	THREAD_BEGIN_LOOP
	{
		Aeon::UUID b("Test");

		if(prev == b)
		{
			bFailed = true;
		}
	}
	THREAD_END_LOOP
}

/**
 * Test cases for the Aeon UUID system.
 */
Context(Test_Aeon_UUID)
{
	// -------------------------------------------------------------------------
	Spec(Test_Uniqueness)
	{
		Aeon::UUID a("SomeText");
		Aeon::UUID b("SomeText");
		
		LOG_DEBUG << a << " <-> " << b;

		Assert::That(a, Is().Not().EqualTo(b));
		Assert::That(a.GetHash(), Is().EqualTo(b.GetHash()));
	}

	// -------------------------------------------------------------------------
	Spec(Test_Performance)
	{
		Aeon::UUID prev("Test");
		LOG_DEBUG << "Generating 1 million unique ids (testing with assertions enabled, this will take some time)";

		for(int i = 0; i < 1000000; i++)
		{
			Aeon::UUID uuid("Test");

			Assert::That(uuid, Is().Not().EqualTo(prev));
			prev = uuid;
		}

		LOG_DEBUG << "Done.";
	}

	// -------------------------------------------------------------------------
	Spec(Test_ThreadSafety)
	{
		LOG_DEBUG << "Testing unique id thread safety";
		Aeon::ProcessManager& man = Aeon::ProcessManager::Instance();

		Aeon::ProcessManager::ThreadID id1 = man.Create("TestProcessA", BIND(&UUIDProcessA));
		Aeon::ProcessManager::ThreadID id2 = man.Create("TestProcessB", BIND(&UUIDProcessB));
		AeonTestUtil::Sleep(3000);
		man.StopAll();

		Assert::That(aFailed, Is().Not().EqualTo(true));
		Assert::That(bFailed, Is().Not().EqualTo(true));
	}
};
