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
#include <Aeon/Serialization/World.pb.h>

#include <Aeon/WorldBase.h>
#include <Aeon/Entity.h>
#include <Aeon/Attribute.h>
#include <Aeon/AttributeContainer.h>
#include <Aeon/AeonTypes.h>

 #include <igloo/igloo.h>

using namespace igloo;
using namespace Aeon;

/**
 * Implementation of the abstract World base class.
 */
class WorldTest : public WorldBase
{
public:
	WorldTest():
		WorldBase(),
		loadCallbackCalled(false),
		saveCallbackCalled(false)
	{

	}
public:
	bool loadCallbackCalled, saveCallbackCalled;
	Vector<Entity*>::type entities;

public:
	virtual void OnSerializeEntities(Serialize::World* target) 
	{
		saveCallbackCalled = true;
		for(uint32_t i = 0; i < entities.size(); i++)
		{
			entities[i]->OnSerialize(target->add_entity_table());
		}	
	}

	virtual void OnLoadEntity(const Serialize::EntityInstance& data) 
	{
		loadCallbackCalled = true;
		entities.push_back(EntityDatabase::Instance().GetFactory()->CreateEntity(data));
	}

	virtual void OnUpdate(float delta)
	{
		
	}
public:
	bool VerifySettings(const UUID& id)
	{
		if(!GetSettings()) return false;
		if(GetSettings()->GetUniqueID() != id) return false;
	}
};

static UUID testDescriptorUUID(UUID::Null());
static UUID dummyDescriptorUUID(UUID::Null());

/**
 * Sets up our test data.
 */
void SetupTestWorld()
{
	EntityDatabase::Instance().InitNew("World Test", "Testing the abstract World class");
	EntityDescriptor* descriptor = EntityDescriptor::Create("WorldSettings");
	descriptor->SetAttribute<bool>(AttributeKey("Test"), true);
	EntityDatabase::Instance().AddEntityDescriptor(descriptor);

	EntityDescriptor* dummyDescriptor = EntityDescriptor::Create("DummyData");
	dummyDescriptor->SetAttribute<String>(AttributeKey("TestString"), "Test");
	EntityDatabase::Instance().AddEntityDescriptor(dummyDescriptor);

	EntityDatabase::Instance().Save("TestWorld.db");
	testDescriptorUUID = descriptor->GetUUID();
	dummyDescriptorUUID = dummyDescriptor->GetUUID();
	EntityDatabase::Instance().ClearAll();
}

/**
 * Test cases for the abstract World class.
 */
Context(Test_AeonWorld)
{
	// -------------------------------------------------------------------------
	Spec(Test_World_0_Saving)
	{
		SetupTestWorld();
		EntityDatabase::Instance().Load("TestWorld.db");

		WorldTest* testworld = new WorldTest();
		testworld->SetWorldName("Test World");
		AssertThat(EntityDatabase::Instance().HasEntityDescriptor(testDescriptorUUID), Is().True());
		AssertThat(EntityDatabase::Instance().HasEntityDescriptor(dummyDescriptorUUID), Is().True());
		testworld->entities.push_back(EntityDatabase::Instance().GetFactory()->CreateEntity(dummyDescriptorUUID));
		
		testworld->SetSettings(EntityDatabase::Instance().GetFactory()->CreateEntity(testDescriptorUUID));
		AssertThat(testworld->Save(String("TestWorld.wdb")), Is().True());
		AssertThat(testworld->saveCallbackCalled, Is().True());
		AssertThat(testworld->SerializedEntitiesCount(), Is().EqualTo(1));

		EntityDatabase::Instance().ClearAll();
	}

	// -------------------------------------------------------------------------
	Spec(Test_World_1_Loading)
	{
		EntityDatabase::Instance().Load("TestWorld.db");

		WorldTest* testworld = new WorldTest();
		AssertThat(EntityDatabase::Instance().HasEntityDescriptor(testDescriptorUUID), Is().True());

		AssertThat(testworld->Load(String("TestWorld.wdb")), Is().True());
		
		testworld->OnSetupScene();

		AssertThat(testworld->loadCallbackCalled, Is().True());
		AssertThat(testworld->GetID().hash, Is().EqualTo(WorldID("Test World").hash));
		Assert::That(testworld->GetSettings() != 0);
		AssertThat(testworld->GetSettings()->GetAttribute<bool>("Test"), Is().True());
		AssertThat(testworld->SerializedEntitiesCount(), Is().EqualTo(1));
		AssertThat(testworld->entities.size(), Is().EqualTo(1));

		EntityDatabase::Instance().ClearAll();
	}
};