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
#include <Aeon/Component.h>
#include <Aeon/ComponentRegistry.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/Message.h>
#include <Aeon/MessageChannel.h>
#include <Aeon/MessageDispatcher.h>

#include "AeonTestUtil.h"

#include <igloo/igloo.h>


using namespace igloo;
using namespace Aeon;


// -------------------------------------------------------------------------
BEGIN_DECLARE_MESSAGE(MessageAsyncTest)
{
	DECLARE_ATTRIBUTE(int32_t, "test")
}
END_DECLARE_MESSAGE()

// -------------------------------------------------------------------------
class AsyncMessageTestComponent : public Aeon::Component
{
	AEON_COMPONENT_DECLARE(AsyncMessageTestComponent, "AsyncMessageTestComponent")

public:

	void OnMessage(Aeon::Message* m) override
	{
		if(m->GetType() == Aeon::MessageType<MessageAsyncTest>::id())
		{
			MessageAsyncTest* msg = static_cast<MessageAsyncTest*>(m);
			LOG_DEBUG << "Received message with testVar equal to " << msg->GetAttribute<int32_t>(ATTR("test")) << "!";
		} 
	}

	virtual void Destroy() 
	{

	}

	virtual void OnCreateComponentInstance() 
	{

	}

	virtual void OnInitialise(Aeon::Entity* parent) 
	{
		Subscribe("AsyncTestChannel");
	}

};

/**
 * Test cases for the messaging system.
 */
Context(Test_Messaging)
{
	// -------------------------------------------------------------------------
	void SetUp()
	{
		Aeon::EntityDatabase& db = Aeon::EntityDatabase::Instance();
		db.InitNew("Messaging Test", "Test");
		db.AddComponentDescriptor(ComponentDescriptor::Create("AsyncMessageTestComponent"));	
	}

	// -------------------------------------------------------------------------
	Spec(AsyncMessages_Test)
	{
		Aeon::MessageDispatcher& dispatcher = Aeon::MessageDispatcher::Instance();
		dispatcher.CreateChannel("AsyncTestChannel");

		Aeon::ComponentRegistry* registry   = Aeon::EntityDatabase::Instance().GetComponentRegistry();
		registry->RegisterComponent(new AsyncMessageTestComponent());

		AsyncMessageTestComponent* com = static_cast<AsyncMessageTestComponent*>(registry->CreateComponent("AsyncMessageTestComponent", nullptr));

#undef SendMessage // STOP RESOLVING THIS MACRO!

		Message* msg = Aeon::Message::Create<MessageAsyncTest>();
		msg->SetAttribute<int32_t>(ATTR("Test"), 5);

		dispatcher.SendMessageAsync("AsyncTestChannel", msg);

		//com->Destroy();
		//delete com;
		AeonTestUtil::Sleep(1000);
	}
};
