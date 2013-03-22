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
#include <google/protobuf/message.h>
#include <Aeon/Aeon.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/MessageDispatcher.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	void Shutdown()
	{
		MessageDispatcher::Instance().Shutdown();

		ProcessManager::Instance().StopAll();

		EntityDatabase::Instance().GetFactory()->DestroyAll();
		EntityDatabase::Instance().Shutdown();
				
		google::protobuf::ShutdownProtobufLibrary();
	}
}
