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
#include <Aeon/Multicore/Types.h>
#include <Aeon/EntityDatabase.h>
#include <Aeon/ComponentDescriptor.h>
#include <Aeon/Logging/Logging.h>
#include <Aeon/Entity.h>
#include <Aeon/Types/UUID.h>

#include <igloo/igloo.h>
using namespace Aeon;
using namespace igloo;

class DebugComponent : public Component {
public:
	DebugComponent() : Component(ComponentID("DebugComponent")) {}
	DebugComponent(const DebugComponent& rhs, Entity* parent) : Component(rhs.id, parent) { }

	void OnUpdate( float delta ) 
	{
	}

	void OnMessage( Message* m ) 
	{
	}

	Component* Create(Entity* parent) 
	{
		return new DebugComponent(*this, parent);
	}

	void Destroy() 
	{
	}

	virtual void OnCreateComponentInstance() 
	{

	}

	virtual void OnInitialise(Aeon::Entity* parent) 
	{

	}

};


/**
 * Test Cases for the Entity Database.
 */
Context (Test_Aeon_Entity_Database)
{

	// -------------------------------------------------------------------------
	void SetUp()
	{

		EntityDatabase& db = EntityDatabase::Instance();

		// Create a fresh, new database:
		db.InitNew("Aeon Entity Database","Entity Database for Sample Game: Aeon.");
	}

	// -------------------------------------------------------------------------
	Spec(ResetChangesEmptyDB)
	{
		LOG_INFO << "*** Testing: Resetting changes on empty database.";
		EntityDatabase& db = EntityDatabase::Instance();
		db.ResetChanges();
	}

	// -------------------------------------------------------------------------
	Spec(ResetChangesNonEmptyDB)
	{

		LOG_INFO << "*** Testing: Resetting changes on non-empty database.";
		EntityDatabase& db = EntityDatabase::Instance();

		// Add some data, one of each type
		db.AddComponentDescriptor(ComponentDescriptor::Create("Empty"));
		db.AddEntityDescriptor(EntityDescriptor::Create("Empty"));
		db.AddCategory("Empty");

		AssertThat(db.GetComponentDescriptors()->size(), Equals(1));

		db.ResetChanges();

		AssertThat(db.GetComponentDescriptors()->size(), Equals(0));
	}

	// -------------------------------------------------------------------------
	Spec(TestSaveEmptyToFile)
	{
		LOG_INFO << "*** Testing: Saving empty database.";

		EntityDatabase& db = EntityDatabase::Instance();
		db.InitNew("Test Database", "Test Desc.");
		AssertThat(db.Save("Test.db"), Equals(true));

		db.ClearAll();

		AssertThat(db.Load("Test.db", false), Equals(true));
		AssertThat(db.GetDatabaseName(), Equals("Test Database"));
		AssertThat(db.GetDatabaseDescription(), Equals("Test Desc."));
	}

	// -------------------------------------------------------------------------
	Spec(TestChangeDBDescriptor)
	{
		LOG_INFO << "*** Testing: Changing DB descriptors.";

		EntityDatabase& db = EntityDatabase::Instance();
		db.InitNew("Test Database", "Test Desc.");
		db.SetDatabaseName("Changed TestDatabase");
		db.SetDatabaseDescription("Changed Description");
		AssertThat(db.Save("Test.db"), Equals(true));

		db.ClearAll();

		AssertThat(db.Load("Test.db", false), Equals(true));
		AssertThat(db.GetDatabaseName(), Equals("Changed TestDatabase"));
		AssertThat(db.GetDatabaseDescription(), Equals("Changed Description"));
	}

	// -------------------------------------------------------------------------
	Spec(TestAddCategories)
	{
		LOG_INFO << "*** Testing: Changing DB categories.";

		EntityDatabase& db = EntityDatabase::Instance();
		db.InitNew("Test Database", "Test Desc.");

		db.AddCategory("A Category");
		db.AddCategory("Another Category");
		db.AddCategory("Yet another Category");
		AssertThat(db.Save("Test.db"), Equals(true));

		db.ClearAll();

		AssertThat(db.Load("Test.db", false), Equals(true));
		AssertThat(db.GetCategories().size(), Equals(3));
		AssertThat(db.GetCategories().at(0)->GetName(), Equals("A Category"));
		AssertThat(db.GetCategories().at(1)->GetName(), Equals("Another Category"));
		AssertThat(db.GetCategories().at(2)->GetName(), Equals("Yet another Category"));
	}

	// -------------------------------------------------------------------------
	Spec(TestAddComponentDescriptors)
	{
		LOG_INFO << "*** Testing: Changing Adding Descriptors.";

		EntityDatabase& db = EntityDatabase::Instance();
		db.InitNew("Test Database", "Test Desc.");

		ComponentDescriptor* comp = ComponentDescriptor::Create("Descriptor");
		comp->AssignParameter<float>("Float");
		comp->AssignParameter<int32_t>("Integer");
		const UUID& lookupID = comp->GetUUID();

		db.AddComponentDescriptor(comp);

		AssertThat(db.Save("Test.db"), Equals(true));

		db.ClearAll();

		AssertThat(db.Load("Test.db", false), Equals(true));
		AssertThat(db.GetComponentDescriptors()->size(), Equals(1));

		AssertThat(db.GetComponentDescriptor(lookupID)->GetParameters().size(), Equals(2));
		ComponentDescriptor* ptr = db.GetComponentDescriptor(lookupID);

		AssertThat(ptr->GetParameter("Float"), Equals(IAttribute::FLOAT));
		AssertThat(ptr->GetParameter("Integer"), Equals(IAttribute::INT32));
	}
	// -------------------------------------------------------------------------
	Spec(TestEntityDescriptors)
	{
		LOG_INFO << "*** Testing: Adding Entity Descriptors.";
		EntityDatabase& db = EntityDatabase::Instance();
		db.InitNew("Aeon Entity Database","Entity Database for Sample Game: Aeon.");

		ComponentDescriptor* comp = ComponentDescriptor::Create("DebugComponent");
		comp->AssignParameter<float>("Float");
		comp->AssignParameter<int32_t>("Integer");
		const UUID& componentID = comp->GetUUID();

		db.AddComponentDescriptor(comp);

		EntityDescriptor* descriptor = EntityDescriptor::Create("Test1");
		descriptor->AddAttribute<float>(AttributeKey("FloatVal"), 0.f);
		descriptor->AddComponent(comp);

		const UUID& entityID = descriptor->GetUUID();
		db.AddEntityDescriptor(descriptor);

		AssertThat(db.GetComponentDescriptors()->size(), Equals(1));
		AssertThat(db.GetEntityDescriptors()->size(), Equals(1));
		AssertThat(db.Save("Test1.db"), Equals(true));
		
		AssertThat(db.Load("Test1.db", false),  Is().True());

		AssertThat(db.HasComponentDescriptor(componentID), Is().True());
		AssertThat(db.HasEntityDescriptor(entityID), Is().True());
		const EntityDescriptor* ptr = db.GetEntityDescriptor(entityID);
		AssertThat(ptr->GetAttributes().size(), Equals(3));
	}

	/*
	// -------------------------------------------------------------------------
	Spec(DoDBStuff)
	{

		EntityDatabase& db = EntityDatabase::Instance();
		// Create an entity template:
		EntityDescriptor* descriptor = EntityDescriptor::Create("name");

		// Assign attributes
		descriptor->AddAttribute<float>(AttributeKey("FloatVal"), 0.f);
		descriptor->AddAttribute<String>(AttributeKey("StringVal"), "");
	
		// Get List of all registered components
		db.GetComponentRegistry()->GetComponentNameList();
	
		// Create a new Component Descriptor
		ComponentDescriptor* comp = ComponentDescriptor::Create("Comp");

		comp->AssignParameter<float>("Key");

		db.AddComponentDescriptor(comp);

		// This makes the entity descriptor have the attribute "Comp.Key"
		descriptor->AddComponent(comp);

		if(descriptor->HasComponent(ComponentID("name")))
		{
			// Removes the component and the attributes belonging to this component.
			descriptor->RemoveComponent(ComponentID("Comp"));
		}

		// Create a new category
		EntityCategory* cat = db.AddCategory("name");

		descriptor->SetCategory(cat);
	
		// Remove category "name", assigns all entities with that category back to the default category
		db.RemoveCategory("name");

		// List categories
		std::vector<EntityCategory*> cats = db.GetCategories();

		// Add Entity as Template
		db.AddEntityDescriptor(descriptor);
	
		// Remove
		db.RemoveEntityDescriptor("name");
	
		//db->GetFactory()->CreateEntity(1234);

		Entity* ent = db.GetFactory()->CreateEntity("Test2");

		// Create Savegame
		/*
		for(Entity* it : db.GetFactory()->GetInstancedEntities())
		{
			if(it)
			{
				Serialize::EntityInstance* ptr = savegame.add_entity_instance();
				it->OnSerialize(ptr);
			}
		}

	
		// Create a new entity
		ent->SetAttribute<bool>("Flag", false);

		// Assign a component, we're using a "null" component as mock object (we don't need to instantiate it to assign it).
		ent->AddComponent(ComponentRegistry::CreateMockComponent("DebugComponent"));
		db.GetFactory()->RegisterEntity(ent); // When registering, the data is copied into an "EntityDescriptor"
		db.GetFactory()->DestroyEntity(ent); // So we can destroy our ptr since we don't need it

		// Create the entity (from a template)
		ent = db.GetFactory()->CreateEntity("TestError");

		// Wait, that was an invalid key. Create the correct entity.
		ent = db.GetFactory()->CreateEntity("Test2");
	
		// Store Database
		db.Save("../data/GameEntities.db");

	}		*/

	// -------------------------------------------------------------------------
	void TearDown()
	{
		EntityDatabase::Instance().ClearAll();
	}
};
