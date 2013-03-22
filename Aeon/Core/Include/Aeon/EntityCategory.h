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
#ifndef EntityCategory_h__
#define EntityCategory_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Types/UUID.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief An entity category used do specify the group an entity belongs to. Entities belong to the category "Default" by default. 
	 * 
	 * If an entity requires another category, use this class to indicate those. 
	 * @important do not use the factory methods provided here directly. To ensure consistency (i.e. when a category
	 * is removed -> assign all entities with this category back to "Default"), the EntityDatabase should be used.
	 * 
	 * @See EntityDatabase::AddCategory
	 *  
	 */
	class AEON_EXPORT EntityCategory
	{
	private:
		/**
		 *  Constructor
		 */
		EntityCategory(const String& name);

		/**
		 * Constructor
		 */
		EntityCategory(const String& name, const UUID& id);

	public:
		/**
		 * Creates a new category.
		 */
		static EntityCategory* Create(const String& name);
		
		/**
		 * Creates a category from serialized data.
		 */
		static EntityCategory* Create(const Serialize::CategoryDescriptor& data);

		/**
		 * Destroys the given category.
		 */
		static void Destroy(EntityCategory* ptr);

	public:
		/**
		 * Get the default instance
		 */
		static EntityCategory* GetDefaultInstance();

		/**
		 * Get the name of the category.
		 */
		const String& GetName() const;

		/**
		 * Sets the name of the category.
		 */
		void SetName(const String& name);

		/**
		 * Get the ID of the category.
		 */
		const UUID& GetID() const;

	private:
		/**
		 * Name.
		 */
		String name;

		/**
		 * Id.
		 */
		UUID id;
	};
}

#endif // EntityCategory_h__