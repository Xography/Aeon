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
#include <Aeon/ProcessManager.h>
#include <Aeon/Logging/Logging.h>

#include "AeonTestUtil.h"

#include <igloo/igloo.h>

using namespace igloo;
using namespace Aeon;

// -------------------------------------------------------------------------
void PhysicsProcess()
{
	THREAD_BEGIN_LOOP
	{
		//LOG_DEBUG << "Physics says hello!";
	}
	THREAD_END_LOOP
}

// -------------------------------------------------------------------------
void GraphicsProcess()
{
	THREAD_BEGIN_LOOP
	{
		//LOG_DEBUG << "Graphics says hello!";
	}
	THREAD_END_LOOP
}

/**
 * Test cases for the Aeon Threading facilities
 */
Context(Test_Aeon_Process)
{

	// -------------------------------------------------------------------------
	Spec(Test_Process_Creation)
	{
		Aeon::ProcessManager& man = Aeon::ProcessManager::Instance();

		Aeon::ProcessManager::ThreadID id1 = man.Create("PhysicsProcess", BIND(&PhysicsProcess));
		Aeon::ProcessManager::ThreadID id2 = man.Create("GraphicsProcess", BIND(&GraphicsProcess));

		Aeon::ProcessManager::ThreadID id3 = man.Create("PhysicsProcess", BIND(&PhysicsProcess));

		// TODO: change Sleep to something more crossplatform
		AeonTestUtil::Sleep(3000);	man.PauseAll();
		AeonTestUtil::Sleep(1000);	man.ResumeAll();
		AeonTestUtil::Sleep(1000);	man.Pause(id1);
		AeonTestUtil::Sleep(1000);	man.Pause(id2);		man.Resume(id1);
		AeonTestUtil::Sleep(1000);	man.Resume(id2);

		AssertThat(id3.IsNull(), Is().EqualTo(true));
		AssertThat(id1.IsNull(), Is().EqualTo(false));
		AssertThat(id2.IsNull(), Is().EqualTo(false));

		man.StopAll();
	}
};
