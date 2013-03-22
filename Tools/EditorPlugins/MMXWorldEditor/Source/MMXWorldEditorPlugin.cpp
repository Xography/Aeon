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
#include "AeonWorldEditorPlugin.h"
#include <QFile>
#include <QFileInfo>

// ----------------------------------------------------------------------------
AeonWorldEditorPlugin::AeonWorldEditorPlugin():
	world(nullptr),
	worldWidget(nullptr),
	settingsWidget(nullptr)
{

}

// ----------------------------------------------------------------------------	
bool AeonWorldEditorPlugin::OnCreateNewFile(const QString& filepath)
{
	AeonEditorWorld* newWorld  = new AeonEditorWorld();	
	DatabaseManager* manager = DatabaseManager::GetInstance();

	if(!manager->HasEntityDescriptor("WorldSettings"))
	{
		SetupDatabase(manager);
	}

	const Aeon::UUID uuid = Aeon::EntityDatabase::Instance().LookupEntityUUID(Aeon::EntityID("WorldSettings"));
	newWorld->SetSettings(Aeon::EntityDatabase::Instance().GetFactory()->CreateEntity(uuid));

	newWorld->GetSettings()->SetAttribute<Aeon::String>(ATTR("VEngine World File"), filepath.split("/").back().split(".").front().append(".world").toAscii().data());
	bool success = newWorld->Save(filepath.toAscii().data());
	newWorld->ClearAll();
	delete newWorld;
	return success;
}

// ----------------------------------------------------------------------------	
QString AeonWorldEditorPlugin::GetPluginName()
{
	return "World Editor Plugin for Aeon";
}

// ----------------------------------------------------------------------------	
QString AeonWorldEditorPlugin::GetPluginVersion()
{
	return "0.0.1";
}

// ----------------------------------------------------------------------------	
QString AeonWorldEditorPlugin::GetPluginDate()
{
	return "Compiled on " __DATE__;
}

// ----------------------------------------------------------------------------	
bool AeonWorldEditorPlugin::OnFileOpen(const QString& filepath)
{
	filename = QFileInfo(filepath).fileName();

	if(world)
	{
		world->ClearAll();
		delete world;
	}
	world = new AeonEditorWorld();

	return world->Load(filepath.toAscii().data());
}

// ----------------------------------------------------------------------------	
QString AeonWorldEditorPlugin::GetErrorMessage()
{
	return "Something failed.";
}

// ----------------------------------------------------------------------------	
QWidget* AeonWorldEditorPlugin::GetSettingsWidget(QWidget* parent)
{
	if(!settingsWidget)
		settingsWidget = new AeonSettingsWidget(parent, world);
	return settingsWidget;
}

// ----------------------------------------------------------------------------	
QWidget* AeonWorldEditorPlugin::GetWorldEditorWidget(QWidget* parent)
{
	if(!worldWidget)
		worldWidget = new AeonWorldEditorWidget(parent, world);
	return worldWidget;
}

// ----------------------------------------------------------------------------	
Aeon::Entity* AeonWorldEditorPlugin::GetSettingsEntity()
{
	return world->GetSettings();
}

// ----------------------------------------------------------------------------	
void AeonWorldEditorPlugin::SetupDatabase(DatabaseManager* manager)
{
	Aeon::EntityDescriptor* ptr = manager->AddNewEntity("WorldSettings");
	ptr->SetAttribute<Aeon::String>(ATTR("VEngine World File"), "");
	ptr->SetAttribute<bool>(ATTR("Layered Track"), false);
	ptr->SetAttribute<Aeon::String>(ATTR("Soundtrack ID"), "");
	manager->NotifyDatabaseChanged();
}

// -------------------------------------------------------------------------
const QString& AeonWorldEditorPlugin::GetFilename()
{
	return filename;
}

// -------------------------------------------------------------------------
void AeonWorldEditorPlugin::Refresh()
{
	if(worldWidget) 
		worldWidget->SetWorld(world);

	if(settingsWidget)
		settingsWidget->SetWorld(world);
}
