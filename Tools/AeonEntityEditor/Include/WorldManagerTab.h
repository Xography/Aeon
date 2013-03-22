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
#ifndef WorldManagerTab_h__
#define WorldManagerTab_h__

#include <QWidget>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "Plugin/IWorldEditorPlugin.h"
#include "CheckedNameInputDialog.h"
#include "ui_WorldManagerTab.h"

/**
 * An editor tab that helps the user to manager world/level files (.wdb)
 */
class WorldManagerTab : public QWidget
{
	Q_OBJECT

public:
	/**
		* The World manager tab.
		*/
	WorldManagerTab(QWidget* parent, QString settingsfile, IWorldEditorPlugin* plugin);

	/**
	* Returns the current path to the .wdb files.
	*/
	QString GetCurrentWDBPath();

private slots:
	/**
	 * Whenever a change of the current folder was requested.
	 */
	void OnSelectFolder();

	/**
	 * Refreshes the list of .wdb files.
	 */
	void OnRefreshList();

	/**
	 * Callback when a file in the list was activated.
	 */
	void OnCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

	/**
	 * User wants to delete a file
	 */
	void OnDelete();

	/**
	 * User wants to open a file.
	 */
	void OnOpen();

	/**
	 * User wants to create a new file.
	 */
	void OnNew();

private:
	/**
	 * The UI
	 */
	Ui::WorldManagerTab ui;
	
	/**
	 * The currently selected file.
	 */
	QString currentItem;

	/**
	 * The plugin providing world editor functionality.
	 */
	IWorldEditorPlugin* plugin;
	
	/**
	 * Determines if a world file is currently opened.
	 */
	bool isOpened;
};

#endif // WorldManagerTab_h__