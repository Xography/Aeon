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
#ifndef AeonWorldEditorPlugin_h__
#define AeonWorldEditorPlugin_h__

#include "AeonEditorWorld.h"
#include "DatabaseManager.h"
#include "AeonWorldSettingsWidget.h"
#include "AeonWorldEditorWidget.h"

#include "Plugin/IWorldEditorPlugin.h"

/**
 * @brief World Editor Plugin for Aeon
 */
class AeonWorldEditorPlugin : public IWorldEditorPlugin
{
public:
	AeonWorldEditorPlugin();

public:
	/**
	 * Implements IWorldEditorPlugin
	 * 
	 * @see IWorldEditorPlugin
	 */
	bool OnCreateNewFile(const QString& filepath) override;
	bool OnFileOpen(const QString& filepath) override;
	void Refresh() override;

public: 
	QString GetPluginName() override;
	QString GetPluginVersion() override;
	QString GetPluginDate() override;

public: 
	Aeon::Entity* GetSettingsEntity() override;
	QString GetErrorMessage() override;

public: 
	QWidget* GetSettingsWidget(QWidget* parent) override;
	QWidget* GetWorldEditorWidget(QWidget* parent) override;
	const QString& GetFilename() override;

private:
	/**
	 * In case we dont have the required entities in the database, create their initial descriptor.
	 */
	void SetupDatabase(DatabaseManager* manager);

private:
	/**
	 * 
	 */
	AeonEditorWorld* world;
	
	/**
	 * 
	 */
	QString filename;
	
	/**
	 * 
	 */
	AeonWorldEditorWidget* worldWidget;
	
	/**
	 * 
	 */
	AeonSettingsWidget* settingsWidget;

};

#endif // AeonWorldEditorPlugin_h__