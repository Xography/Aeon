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
 
 namespace Aeon {
	
	/**
	 * Forward declares all serialization classes, so we don't need to include 
	 * the heavyweight implementations or forward declare them in each header all the time.
	 */
	namespace Serialize {
		class Attribute;
		class AttributeInstance;
		class CategoryDescriptor;
		class Component;
		class ComponentDescriptor;
		class ComponentInstance;
		class EntityDB;
		class EntityDescriptor;
		class EntityInstance;
		class UUID;
		class Value;
		class World;
	}

	/**
	 * Aeon forward declarations
	 */
	class AttributeContainer;
	class ComponentRegistry;
	class Entity;
	class EntityDatabase;
}