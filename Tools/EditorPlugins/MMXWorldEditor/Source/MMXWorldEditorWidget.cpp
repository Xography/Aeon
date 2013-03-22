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
#include "AeonWorldEditorWidget.h"
#include "VEngineWorldParser.h"
#include "DatabaseManager.h"

#include <Plugin/PluginManager.h>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>

#include <Aeon/EntityDatabase.h>
#include <Aeon/Entity.h>
#include <Aeon/Component.h>
#include <Aeon/ComponentDescriptor.h>


// ----------------------------------------------------------------------------
AeonWorldEditorWidget::AeonWorldEditorWidget(QWidget* parent, AeonEditorWorld* world):
	QWidget(parent),
	world(world),
	lastItem(nullptr)
{
	ui.setupUi(this);

	ui.treeWidget->clear();

	const QString& settingsFile = PluginManager::GetSettingsFileName();

	if(QFile::exists(settingsFile))
	{
		QSettings settings(settingsFile, QSettings::IniFormat);
		
		settings.beginGroup("WorldEditor");
		{
			scriptPath = settings.value("ScriptsPath").toString();
		}

		settings.endGroup();
	} 
	
	SetupWorld();
	
	connect(ui.toolButtonAdd, SIGNAL(clicked()), this, SLOT(OnAddEntityInstance()));
	connect(ui.pushButtonSave, SIGNAL(clicked()), this, SLOT(OnSave()));
	connect(ui.toolButtonDelete, SIGNAL(clicked()), this, SLOT(OnDeleteEntityInstance()));
	connect(ui.toolButtonAssign, SIGNAL(clicked()), this, SLOT(OnAssignVEngineObject()));
	connect(ui.toolButtonUnassign, SIGNAL(clicked()), this, SLOT(OnUnassignVEngineObject()));
	connect(ui.treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(OnEntityActivated(QTreeWidgetItem*, QTreeWidgetItem*)));
	connect(ui.buttonResetEntity, SIGNAL(clicked()), this, SLOT(OnEntityResetAttributes()));
	connect(ui.buttonSaveEntity, SIGNAL(clicked()), this, SLOT(OnEntitySaveAttributes()));
	connect(ui.reparseWorldFile,SIGNAL(clicked()), this, SLOT(OnReparseWorldFile()));
	connect(PluginManager::Instance().GetTabWidgetPtr("Entity Editor"), SIGNAL(OnCategoriesModified()), this, SLOT(OnCategoriesModified()));

	ui.buttonSaveEntity->setEnabled(false);
	ui.buttonResetEntity->setEnabled(false);

	RefreshTree();
}

// -------------------------------------------------------------------------
AeonWorldEditorWidget::~AeonWorldEditorWidget()
{
	QSettings settings(PluginManager::GetSettingsFileName(), QSettings::IniFormat);

	settings.beginGroup("WorldEditor");
	{
		settings.setValue("ScriptsPath", scriptPath);
	}
	settings.endGroup();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::FixDatabase()
{
	if(DatabaseManager::GetInstance()->HasComponentDescriptor("VEngineObjectBehaviour")) return;	

	Aeon::ComponentDescriptor* descriptor = Aeon::ComponentDescriptor::Create("VEngineObjectBehaviour");
	descriptor->SetDescription("This behaviour adds support for attaching entities to VEngine objects.");
	descriptor->AssignParameter<Aeon::String>("VEngine Object Name");
	
	DatabaseManager::GetInstance()->AddComponentDescriptor(descriptor);
	DatabaseManager::GetInstance()->NotifyDatabaseChanged();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::AddEntityToTree(Aeon::Entity* ptr)
{
	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText(0, ptr->GetID().key.c_str());
	item->setData(0, Qt::UserRole, QByteArray::fromRawData(reinterpret_cast<const char*>(ptr->GetUniqueID().GetData()), sizeof(uint64_t) * 2));

	if(ptr->CheckAttribute(ATTR("VEngineObjectBehaviour.VEngineObject")) == Aeon::IAttribute::STRING)
	{
		Aeon::String vengineObj = ptr->GetAttribute<Aeon::String>(ATTR("VEngineObjectBehaviour.VEngineObject"));
		item->setText(1, vengineObj.c_str());
	}

	item->setIcon(0, QIcon(":/EntityEditor/EntityInstance"));

	QTreeWidgetItem* topLevelItem = 0;

	for(int i(0); i < ui.treeWidget->topLevelItemCount(); ++i)
	{
		const Aeon::EntityDescriptor* desc = ptr->GetDescriptor();

		if(!desc) continue;
		QString name = QString::fromStdString(desc->GetCategory()->GetName());

		if(ui.treeWidget->topLevelItem(i)->text(0) == name)
		{
			topLevelItem = ui.treeWidget->topLevelItem(i);
		}
	}

	if(!topLevelItem)
	{
		topLevelItem = new QTreeWidgetItem(); 
		topLevelItem->setText(0, QString::fromStdString(ptr->GetDescriptor()->GetCategory()->GetName())); 
		topLevelItem->setIcon(0, QIcon(":/EntityEditor/CategoryIcon"));

		ui.treeWidget->addTopLevelItem(topLevelItem);

	}

	topLevelItem->addChild(item);
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnAddEntityInstance()
{
	QStringList items;

	const Aeon::EntityDatabase::EntityDescriptors* db = Aeon::EntityDatabase::Instance().GetEntityDescriptors();

	for(Aeon::EntityDatabase::EntityDescriptors::const_iterator it = db->begin(); it != db->end(); ++it)
	{	
		if(it->second->GetName()=="WorldSettings") continue;

		items << it->second->GetName().c_str();
	}

	bool ok;

	QString item = QInputDialog::getItem(this, tr("Select Entity"),
		tr("Entity to spawn:"), items, 0, false, &ok);

	if (ok && !item.isEmpty())
	{
		Aeon::Entity* ptr = Aeon::EntityDatabase::Instance().GetFactory()->CreateEntity(item.toAscii().data());
		world->AddEntity(ptr);
		AddEntityToTree(ptr);
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnSave()
{
	world->Save(world->GetFilename());
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnDeleteEntityInstance()
{
	QTreeWidgetItem* item = ui.treeWidget->currentItem();
	if(item->childCount()) return;

	if(!item) return;

	QByteArray array = item->data(0, Qt::UserRole).toByteArray();
	Aeon::UUID uuid(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));
	
	world->RemoveEntity(uuid);

	RefreshTree();
}

// -------------------------------------------------------------------------
void remove(QStringList& list, const QStringList& toDelete){
	QStringListIterator i(toDelete);
	while(i.hasNext()){
		list.removeAll(i.next());
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnAssignVEngineObject()
{
	QTreeWidgetItem* item = ui.treeWidget->currentItem();

	if(!item) return;
	if(item->childCount()) return;

	QStringList alreadyAssigned;

	for(int i(0); i < ui.treeWidget->topLevelItemCount(); ++i)
	{
		QTreeWidgetItem* tl = ui.treeWidget->topLevelItem(i);

		for(int y = 0; y < tl->childCount(); y++)
		{
			alreadyAssigned << tl->child(y)->text(1);
		}

	}

	QStringList available = availableVEngineObjects.toSet().subtract(alreadyAssigned.toSet()).toList();

	if(available.isEmpty())
	{
		QMessageBox::warning(this,tr("No object available"), tr("There are either no VEngine objects defined for entity "
					"usage or all have already been assigned an entity. You should create additional objects with the VEngine editor."), QMessageBox::Ok);
		return;
	}

	QByteArray array = item->data(0, Qt::UserRole).toByteArray();
	Aeon::UUID uuid(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));
	Aeon::Entity* ptr = world->GetEntity(uuid);

	if(!ptr) return;
	
	
	if(ptr->CheckAttribute(ATTR("VEngineObjectBehaviour.VEngineObject")) != Aeon::IAttribute::STRING)
	{
		QMessageBox::information(this, tr("VEngineObjectBehaviour missing"), tr(
		"This entity requires the \"VEngineObjectBehaviour\" to have a VEngine object attached. "
		"You need to add the behaviour to the entities' descriptor to perform this action.\n\n You can do this in the Entity Editor. Note that this will affect all entities of this type."),
			QMessageBox::Ok);
		return;
	} 

	bool ok;

	QString selectedItem = QInputDialog::getItem(this, tr("Select VEngine Object"),
		tr("Assign VEngine Object:"), available, 0, false, &ok);

	if (ok && !selectedItem.isEmpty())
	{
		ptr->SetAttribute<Aeon::String>(ATTR("VEngineObjectBehaviour.VEngineObject"), selectedItem.toAscii().data());

		RefreshTree();
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnUnassignVEngineObject()
{
	QTreeWidgetItem* item = ui.treeWidget->currentItem();

	if(!item) return;
	if(item->childCount()) return;

	QByteArray array = item->data(0, Qt::UserRole).toByteArray();
	Aeon::UUID uuid(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));
	Aeon::Entity* ptr = world->GetEntity(uuid);

	if(!ptr) return;


	if(ptr->CheckAttribute(ATTR("VEngineObjectBehaviour.VEngineObject")) == Aeon::IAttribute::STRING)
	{
		ptr->SetAttribute<Aeon::String>(ATTR("VEngineObjectBehaviour.VEngineObject"), "");
	}

	RefreshTree();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::RefreshTree()
{
	ui.treeWidget->clear();
	
	if(world)
	{
		AddEntityToTree(world->GetSettings());
		world->RefreshCallback();
	}
	ui.treeWidget->expandAll();

	ui.buttonSaveEntity->setEnabled(false);
	ui.buttonResetEntity->setEnabled(false);

	ui.propertyBrowser->Clear();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::FixScriptPath()
{
	while(scriptPath.isEmpty() || !QDir(scriptPath).exists())
	{
		if(QMessageBox::information(this, tr("Path required"), tr("In order to edit this file, you need to specify the directory "
									"containing VEngine .world scripts. Press Ok to select a folder or abort to cancel this operation. "), 
									QMessageBox::Ok | QMessageBox::Abort) == QMessageBox::Abort)	break;
		scriptPath = QFileDialog::getExistingDirectory(this, tr("Select VEngine Scripts directory."), scriptPath, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);	
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::SetupWorld()
{
	if(!world) return;

	FixScriptPath();
	ParseVEngineFile();
	FixDatabase();

	world->SetWorldEditor(this);
	world->SetParsedVEngineObjects(availableVEngineObjects);
	world->OnSetupScene();

}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnEntityActivated(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
	if(!current || current->childCount())
		return;

	QByteArray array = current->data(0, Qt::UserRole).toByteArray();
	Aeon::UUID uuid(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));
	Aeon::Entity* ptr = world->GetEntity(uuid);

	if(!ptr)
	{
		ptr = world->GetSettings();
	}

	if(ptr)
	{
		ui.propertyBrowser->Clear();
		ui.propertyBrowser->SetContainer(ptr);

		lastItem = current;

		ui.buttonSaveEntity->setEnabled(true);
		ui.buttonResetEntity->setEnabled(true);
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnEntitySaveAttributes()
{
	if(!lastItem) return;

	QByteArray array = lastItem->data(0, Qt::UserRole).toByteArray();
	Aeon::UUID uuid(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));
	Aeon::Entity* ptr = world->GetEntity(uuid);

	if(ptr)
	{
		ptr->ClearAttributes();
		ui.propertyBrowser->ApplyChangesToContainer(ptr);

		//ui.treeWidget->setItemSelected(lastItem, true);
		OnEntityActivated(lastItem, nullptr);
	}
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnEntityResetAttributes()
{
	//ui.treeWidget->setItemSelected(ui.treeWidget->currentItem(), true);
	OnEntityActivated(ui.treeWidget->currentItem(), nullptr);
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnCategoriesModified()
{
	RefreshTree();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::ParseVEngineFile()
{
	availableVEngineObjects.clear();
	VEngineWorldParser parser(scriptPath + "/" + world->GetSettings()->GetAttribute<Aeon::String>(ATTR("VEngine World File")).c_str());

	if(!parser.IsFileValid())
	{
		if(QMessageBox::warning(this, tr("Invalid .world file"), tr("Cannot find or parse the VEngine .world file. The scripts directory that was previously set might"
			" be invalid, do you want to select a folder now?"),QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes)
		{
			scriptPath = "";
			FixScriptPath();
			parser = VEngineWorldParser(scriptPath + "/" + world->GetSettings()->GetAttribute<Aeon::String>(ATTR("VEngine World File")).c_str());
		}

		if(!parser.IsFileValid())
		{
			QMessageBox::critical(this, tr("Unable to parse files"), tr("Your script path seems to be invalid or a file cannot be parsed. Your world will be empty."));
			return;
		}
	}

	availableVEngineObjects = parser.ParseFile();
}
// -------------------------------------------------------------------------
void AeonWorldEditorWidget::OnReparseWorldFile()
{
	ParseVEngineFile();
	world->ValidateEntities();
	RefreshTree();
}

// -------------------------------------------------------------------------
void AeonWorldEditorWidget::SetWorld(AeonEditorWorld* world)
{
	this->world = world;

	RefreshTree();
	SetupWorld();
	ui.treeWidget->expandAll();
}
