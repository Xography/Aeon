/*******************************************************************************
 *
 * This file is part of Sample Game.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 *
 * See LICENSE.txt for licensing details.
 *
 ******************************************************************************/
#ifndef AeonEditorWorld_h__
#define AeonEditorWorld_h__

#include <QString>
#include <QStringList>

#include <Aeon/WorldBase.h>
#include <Aeon/Entity.h>

class AeonWorldEditorWidget;

/**
 * Implementation of the World class for the editor.
 */
class AeonEditorWorld : public Aeon::WorldBase
{	
public:
	/**
	 * Ctor.
	 */
	AeonEditorWorld();

public:
	/**
	 * Sets the parent world editor widget.
	 */
	void SetWorldEditor(AeonWorldEditorWidget* widget);

	/**
	 * Provides the editor world with a list of parsed objects.
	 */
	void SetParsedVEngineObjects(const QStringList& list);
		
public:
	/**
	 * 
	 */
	void AddEntity(Aeon::Entity* entity);
	
	/**
	 * 
	 */
	void RemoveEntity(const Aeon::UUID& uuid);

	/**
	 * 
	 */
	Aeon::Entity* GetEntity(const Aeon::UUID& uuid);

	/**
	 * Refresh callback
	 */
	 void RefreshCallback();
	 
	 /**
	  * Validates the given entity, checking for missing attributes.
	  */
	 void OnValidateEntity(Aeon::Entity* ent);

	 /**
	  * Validates all entities that are currently loaded.
	  */
	 void ValidateEntities();

	 /**
	  * Clears the world
	  */
	 void ClearAll();

public:	// Implements Aeon::WorldBase
	virtual void OnUpdate(float delta);
	virtual void OnSerializeEntities(Aeon::Serialize::World* target);
	virtual void OnLoadEntity(const Aeon::Serialize::EntityInstance& data);
	
private:
	/**
	 * The parent Widget.
	 */
	AeonWorldEditorWidget* worldEditor;

	/**
	 * The list of VEngine object names we have parsed from the file.
	 */
	QStringList parsedVEngineObjects;

	/**
	 * The entities in this world.
	 */
	std::unordered_map<const Aeon::UUID, Aeon::Entity*, Aeon::UUIDKeyHashFunction> entities;
};

#endif // AeonEditorWorld_h__
