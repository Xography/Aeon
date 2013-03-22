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
#ifndef IWorldEditorPlugin_h__
#define IWorldEditorPlugin_h__

#include <QWidget>
#include <QString>
#include <Aeon/Entity.h>

/**
 * This interface allows to implement custom world editors.
 */
class IWorldEditorPlugin {
public:
	/**
	 * Plugin name.
	 */
	virtual QString GetPluginName() = 0;
	
	/**
	 * Plugin version.
	 */
	virtual QString GetPluginVersion() = 0;
	
	/**
	 * Plugin date.
	 */
	virtual QString GetPluginDate() = 0;

public:

	/**
	 * Callback; the user wants to create a new file (but not open it!).
	 */
	virtual bool OnCreateNewFile(const QString& filepath) = 0;
	
	/**
	 * Callback; the user wants to create a open a file.
	 */
	virtual bool OnFileOpen(const QString& filepath) = 0;
	
	/**
	 * Triggers a refresh for all widgets/tabs.
	 */
	virtual void Refresh() = 0;

public:
	/**
	 * Widget for the "World Settings" tab.
	 */
	virtual QWidget* GetSettingsWidget(QWidget* parent) = 0;
	
	/**
	 * Widget for the "World Editor" tab.
	 */
	virtual QWidget* GetWorldEditorWidget(QWidget* parent) = 0;

public:
	/**
	 * Get the string of error messages.
	 */
	virtual QString GetErrorMessage() = 0;

	/**
	 * Get the current filename.
	 */
	virtual const QString& GetFilename() = 0;

public:
	/**
	 * Returns the settigns entity.
	 */
	virtual Aeon::Entity* GetSettingsEntity() = 0;

};


#endif // IWorldEditorPlugin_h__