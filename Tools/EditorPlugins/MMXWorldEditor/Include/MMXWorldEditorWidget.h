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
#ifndef AeonWorldEditorWidget_h__
#define AeonWorldEditorWidget_h__

#include "ui_AeonWorldEditor.h"

#include "AeonEditorWorld.h"

/**
 * World editor widget, allowing the user to assign entities to a VEngine world.
 */
class AeonWorldEditorWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Ctor.
	 */
	AeonWorldEditorWidget(QWidget* parent, AeonEditorWorld* world);

	/**
	 * Dtor.
	 */
	 ~AeonWorldEditorWidget();

public:
	/**
	 * Adds the given entity information to the tree.
	 */
	void AddEntityToTree(Aeon::Entity* ptr);
	
	/**
	 * Refreshes the entity tree
	 */
	void RefreshTree();

	/**
	 * Parses the VEngine script and .wdb file and wires the data.
	 */
	 void SetupWorld();


	 /**
	  * Sets another world, triggering a refresh.
	  */
	 void SetWorld(AeonEditorWorld* world);

private slots:
	/**
	 * User wants to add an entity instance. 
	 */
	void OnAddEntityInstance();
	
	/**
	 * User wants to save .wdb
	 */
	void OnSave();

	/**
	 * User wants to delete an entity instance.
	 */
	void OnDeleteEntityInstance();

	/**
	 * User wants to assign a VEngine object to an entity.
	 */
	 void OnAssignVEngineObject();
	 
	 /**
	  * User wants to remove assigned VEngine object from an entity.
	  */
	void OnUnassignVEngineObject();

	/**
	 * User activates (clicks or selects) an entity.
	 */
	void OnEntityActivated(QTreeWidgetItem* current, QTreeWidgetItem* previous);

	/**
	 * User wants to save changes to the entity.
	 */
	void OnEntitySaveAttributes();

	/**
	 * User wants to reset changes. 
	 */
	void OnEntityResetAttributes();

	/**
	 * Signal emitted by the entity editor whenever categories have been modified.
	 */
	void OnCategoriesModified();

	/**
	 * User wants to reparse the current world file.
	 */
	void OnReparseWorldFile();

private:

	/**
	 * "Fixes" the database; adds missing behaviours
	 */
	void FixDatabase();

	/**
	 * Retrieves the path to VEngine .world scripts.
	 */
	void FixScriptPath();

	/**
	 * Parses the v-engine file
	 */
	void ParseVEngineFile();

private:
	/**
	 * UI object.
	 */
	Ui::AeonWorldEditor ui;

	/**
	 * List of available VEngine objects
	 */
	 QStringList availableVEngineObjects;

	 /**
	  * The actual world.
	  */
	 AeonEditorWorld* world;

	 /**
	  * Path to the VEngine scripts folder.
	  */
	  QString scriptPath;

	  /**
	   * the last selected item.
	   */
	  QTreeWidgetItem* lastItem;
};

#endif // AeonWorldEditorWidget_h__
