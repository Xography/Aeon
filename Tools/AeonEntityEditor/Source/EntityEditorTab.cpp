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
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QSignalMapper>
#include <QTreeWidget>

#include "EntityEditorTab.h"
#include "DatabaseManager.h"
#include "CheckedNameInputDialog.h"

using namespace Aeon;

// -------------------------------------------------------------------------
class CategoryNameValidator : public IValidationFunction
{
public:
	QTreeWidget* widget;

	virtual bool Validate(const QString& input) 
	{
		for(int i = 0; i < widget->topLevelItemCount(); ++i)
		{
			QString name = widget->topLevelItem(i)->text(0);

			if(name == input) return true;
		}	
		return false;
	}

};
// ------------------------------------------------------------
static const QString& GetAttributeTypeName(IAttribute::Type type)
{
	static const QString attributeTypeNames[ATTRIBUTE_TYPES_COUNT] =
	{
		QObject::tr("Invalid"),
		QObject::tr("Blob"),
		QObject::tr("Boolean"),
		QObject::tr("Double"),
		QObject::tr("Float"),
		QObject::tr("32-bit Integer"),
		QObject::tr("64-bit Integer"),
		QObject::tr("4x4 Matrix"),
		QObject::tr("Quaternion"),
		QObject::tr("Range"),
		QObject::tr("String"),
		QObject::tr("2D Vector"),
		QObject::tr("3D Vector"),
		QObject::tr("4D Vector")
	};

	return attributeTypeNames[type];
}

// ------------------------------------------------------------
EntityEditorTab::EntityEditorTab(QWidget *parent):
	QWidget(parent),
	ui(new Ui::EntityEditorTab),
	selectedEntity(nullptr)
{
	ui->setupUi(this);

	connect(DatabaseManager::GetInstance(), SIGNAL(database_opened()), this, SLOT(OnDatabaseOpened()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_created()), this, SLOT(OnDatabaseOpened()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_closed()), this, SLOT(OnDatabaseClosed()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_changed()), this, SLOT(OnDatabaseOpened()));
	

	// Populate entity type combobox.
	for(unsigned i(1); i < ATTRIBUTE_TYPES_COUNT; ++i)
		ui->entityTypeCombo->addItem(GetAttributeTypeName(IAttribute::Type(i)));

	RefreshEntitiesTreeControls();
	SyncTableControls();

}

// ------------------------------------------------------------
EntityEditorTab::~EntityEditorTab()
{
	delete ui;
}

// ------------------------------------------------------------
void EntityEditorTab::RefreshEntitiesTreeControls()
{
	if(!DatabaseManager::GetInstance()->IsOpened())
	{
		ClearEntitiesTree();
		setEnabled(false);
	}

	else
	{
		ui->removeEntityButton->setEnabled(ui->entitiesTree->currentItem());
		setEnabled(true);
	}
}

// ------------------------------------------------------------
void EntityEditorTab::SyncTableControls()
{
	if(!DatabaseManager::GetInstance()->IsOpened() || !ui->entitiesTree->currentItem() || !ui->entitiesTree->currentItem()->parent())
	{
		ui->attributeBrowser->Clear();
		ui->componentsList->clear();

		ui->removeEntityButton->setEnabled(false);
		ui->setEntityCategoryButton->setEnabled(false);
		ui->entitySettingsGroup->setEnabled(false);

		ui->removeCategoryButton->setEnabled(true);
		ui->renameCategoryButton->setEnabled(true);
	}

	else
	{
		ui->entitySettingsGroup->setEnabled(true);
		ui->removeEntityButton->setEnabled(true);
		ui->setEntityCategoryButton->setEnabled(true);

		ui->removeCategoryButton->setEnabled(false);
		ui->renameCategoryButton->setEnabled(false);
		ui->removeAttributeButton->setEnabled(ui->attributeBrowser->GetCurrentItem());
		ui->removeComponentButton->setEnabled(ui->entitiesTree->currentItem());
	}
}

// ------------------------------------------------------------
void EntityEditorTab::RefreshAttributesTable()
{
	ui->attributeBrowser->Clear();
	ui->attributeBrowser->SetContainer(selectedEntity);
}

// ------------------------------------------------------------
void EntityEditorTab::RefreshComponentsList()
{
	ui->componentsList->clear();
	ui->componentsCombo->clear();

	auto& entityComponents = selectedEntity->GetComponents();
	auto definedComponents = EntityDatabase::Instance().GetComponentDescriptors();

	for(auto i(definedComponents->begin()); i != definedComponents->end(); ++i)
	{
		bool continueLoop(true);

		// Check if the entity we're editing has this component descriptor assigned to it already.
		for(auto j(entityComponents.begin()); j != entityComponents.end(); ++j)
		{
			if((*j)->GetUUID() == i->first)
			{
				// Yes it does, add it to the list.
				QListWidgetItem* item = new QListWidgetItem(QString::fromStdString((*j)->GetComponentID().key));
				item->setData(Qt::UserRole, QByteArray::fromRawData(reinterpret_cast<const char*>(i->first.GetData()), sizeof(uint64_t) * 2));

				ui->componentsList->addItem(item);

				continueLoop = false;
				break;
			}
		}

		// No it doesn't, add it to the combobox so it can be added.
		if(continueLoop)
		{
			// Store the UUID as data because it can't be regenerated based on the key (it's based on time factors among others).
			ui->componentsCombo->addItem(QString::fromStdString(i->second->GetComponentID().key),
				QByteArray::fromRawData(reinterpret_cast<const char*>(i->first.GetData()), sizeof(uint64_t) * 2));
		}
	}

	ui->componentsCombo->setEnabled(ui->componentsCombo->count());
	ui->removeComponentButton->setEnabled(ui->componentsList->count());
	ui->addComponentButton->setEnabled(ui->componentsCombo->count());
}

// ------------------------------------------------------------
QTreeWidgetItem *EntityEditorTab::AddEntityToTree(EntityDescriptor *entity)
{	
	if(QTreeWidgetItem *topItem = FindCategory(entity->GetCategory()))
	{
		QTreeWidgetItem *treeItem = new QTreeWidgetItem(topItem);
		treeItem->setFlags((treeItem->flags() & ~Qt::ItemIsDropEnabled) | Qt::ItemIsEditable);
		treeItem->setIcon(0, QIcon(":/EntityEditor/Entity"));
		treeItem->setText(0, QString::fromStdString(entity->GetID().key));
		entitiesTreeWidgetsMap.insert(treeItem, entity);
		return treeItem;
	}
	
	return nullptr;
}

// ------------------------------------------------------------
void EntityEditorTab::ClearEntitiesTree()
{
	for (int i = 0; i < ui->entitiesTree->topLevelItemCount();)
	{
		delete ui->entitiesTree->takeTopLevelItem(i);
	}
	ui->entitiesTree->clear();
	entitiesTreeWidgetsMap.clear();
}

// ------------------------------------------------------------
void EntityEditorTab::OnDatabaseOpened()
{
	ClearEntitiesTree();

	if(DatabaseManager::GetInstance()->IsOpened())
	{
		Aeon::EntityDatabase& db = Aeon::EntityDatabase::Instance();

		LOG_DEBUG << "Adding " << db.GetEntityDescriptors()->size() << " entities to the tree...";

		QList<QTreeWidgetItem*> rootNodes;

		for(Aeon::Vector<Aeon::EntityCategory*>::type::const_iterator i(db.GetCategories().begin()); i != db.GetCategories().end(); ++i)
		{
			QTreeWidgetItem* node = new QTreeWidgetItem(ui->entitiesTree, QStringList() << QString::fromStdString((*i)->GetName()));
			rootNodes.push_back(node);
			node->setIcon(0, QIcon(":/EntityEditor/CategoryIcon"));
			node->setExpanded(true);
		}

		// If a new database is created, or the database file uses an old format,add the default category
		if(rootNodes.isEmpty())
		{
			QTreeWidgetItem* node = new QTreeWidgetItem(ui->entitiesTree, QStringList() << "General");
			rootNodes.push_back(node);
			node->setIcon(0, QIcon(":/EntityEditor/CategoryIcon"));
			node->setExpanded(true);
		}
		ui->entitiesTree->addTopLevelItems(rootNodes);

		for(auto i(db.GetEntityDescriptors()->begin()); i != db.GetEntityDescriptors()->end(); ++i)
			AddEntityToTree(i->second);

	}

	RefreshEntitiesTreeControls();
	SyncTableControls();
}

// ------------------------------------------------------------
void EntityEditorTab::OnDatabaseClosed()
{
	RefreshEntitiesTreeControls();
	SyncTableControls();
}

// ------------------------------------------------------------
void EntityEditorTab::OnComponentDescriptorAdded(Aeon::ComponentDescriptor* descriptor)
{
	ui->componentsCombo->addItem(QString::fromStdString(descriptor->GetComponentID().key),
		QByteArray::fromRawData(reinterpret_cast<const char*>(descriptor->GetUUID().GetData()), sizeof(uint64_t) * 2));
}

// ------------------------------------------------------------
void EntityEditorTab::OnComponentDescriptorRemoved(Aeon::ComponentDescriptor* descriptor)
{
	Aeon::EntityDatabase& db = Aeon::EntityDatabase::Instance();

	for(auto i(db.GetEntityDescriptors()->begin()); i != db.GetEntityDescriptors()->end(); ++i)
	{
		if(i->second->HasComponent(descriptor->GetUUID()))
		{
			LOG_WARNING << "The removed component descriptor was attached to entity descriptor " << i->second->GetID().key << ", detaching it!";
			i->second->RemoveComponent(descriptor->GetUUID());

			if(i->second == selectedEntity)
			{
				LOG_WARNING << "The entity currently selected for editing also used the component descriptor that's being removed, any unsaved changes to the tables are lost!";

				RefreshAttributesTable();
				RefreshComponentsList();
			}
		}
	}
}

// ------------------------------------------------------------
void EntityEditorTab::on_entitiesTree_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* /*previous*/)
{
	selectedEntity = nullptr;

	if(current && current->parent())
	{
		if((selectedEntity = entitiesTreeWidgetsMap[current]))
		{
			RefreshAttributesTable();
			RefreshComponentsList();
		}

		else
		{
			QMessageBox::critical(this, tr("Error"), tr("Selected entity no longer exists"), QMessageBox::Ok);
			delete current;
		}
	}

	SyncTableControls();
}

// ------------------------------------------------------------
void EntityEditorTab::on_entitiesTree_itemChanged(QTreeWidgetItem *item, int column)
{
	Aeon::EntityDescriptor *entity = entitiesTreeWidgetsMap.value(item);

	for(int y(0); y < ui->entitiesTree->topLevelItemCount(); ++y)
	{
		for(int i(0); i < ui->entitiesTree->topLevelItem(y)->childCount(); ++i)
		{
			if(item != ui->entitiesTree->topLevelItem(y)->child(i) &&
			   ui->entitiesTree->topLevelItem(y)->child(i)->text(0) == item->text(column))
			{
				QMessageBox::critical(this, tr("Error"), QString(tr("Entity %1 already exists")).arg(item->text(column)), QMessageBox::Ok);
				if (entity)
				{
					item->setText(column, QString::fromStdString(entity->GetName()));
				}
				return;
			}
		}
	}

	if (entity && QString::fromStdString(entity->GetName()).compare(item->text(column)))
	{
		Aeon::String *newName = new Aeon::String(item->text(column).toStdString());
		entity->SetName(*newName);
	}
}

// ------------------------------------------------------------
void EntityEditorTab::on_addEntityButton_clicked()
{
	QString entityName = "New entity";
	QTreeWidgetItem* item = ui->entitiesTree->currentItem();

	QList<QTreeWidgetItem*> selectedItems = ui->entitiesTree->selectedItems();
	if (selectedItems.isEmpty())
	{
		return;
	}

	QTreeWidgetItem* selectedTopItem = selectedItems.at(0)->parent();
	if (!selectedTopItem)
	{
		//if selected item hasn't parents then selected item is top item (category)
		selectedTopItem = selectedItems.at(0);
	}

	bool entityNameExists(false);
	int newEntityIndex(0);

	do {
		entityNameExists = false;
		for(int y(0); y < ui->entitiesTree->topLevelItemCount(); ++y)
		{
			for(int i(0); i < ui->entitiesTree->topLevelItem(y)->childCount(); ++i)
			{
				if(ui->entitiesTree->topLevelItem(y)->child(i)->text(0) == entityName)
				{
					entityNameExists = true;
					entityName = QString("New entity %1").arg(++newEntityIndex);
					y = ui->entitiesTree->topLevelItemCount();
					break;
				}
			}
		}
	} while (entityNameExists);

	if(EntityDescriptor* entityCreated  = DatabaseManager::GetInstance()->AddNewEntity(entityName.toAscii().data()))
	{
		if(item)
		{
			if(!item->parent())
				entityCreated->SetCategory(EntityDatabase::Instance().GetCategory(item->text(0).toAscii().data()));
			else 
			{
				if(item->parent())
					entityCreated->SetCategory(EntityDatabase::Instance().GetCategory(item->parent()->text(0).toAscii().data()));
			}
			
		}
		QTreeWidgetItem* itemAdded = AddEntityToTree(entityCreated);
		ui->entitiesTree->setCurrentItem(itemAdded, 0);
		ui->entitiesTree->editItem(itemAdded);
	}
}

// ------------------------------------------------------------
void EntityEditorTab::on_removeEntityButton_clicked()
{
	assert(selectedEntity && ui->entitiesTree->currentItem());

	if(selectedEntity && DatabaseManager::GetInstance()->DeleteEntity(selectedEntity))
		delete ui->entitiesTree->currentItem();
}

// ------------------------------------------------------------
void EntityEditorTab::on_addAttributeButton_clicked()
{
	if(!ui->entityName->text().isEmpty() && !ui->entityName->text().contains('.'))
	{
		QList<QtBrowserItem*> list = ui->attributeBrowser->GetTopLevelItems();
		
		for(QList<QtBrowserItem*>::const_iterator i = list.constBegin(); i != list.constEnd(); ++i)
		{
			if(ui->entityName->text() == (*i)->property()->propertyName())
			{
				QMessageBox::critical(this, tr("Error"), tr("An attribute with this name or key already exists! Please use another name."), QMessageBox::Ok);
				return;
			}
		}

		ui->attributeBrowser->AddNewProperty(Aeon::AttributeKey(ui->entityName->text().toAscii().data()), ui->entityTypeCombo->currentIndex() + 1, 0);
			
	}

	else
		QMessageBox::critical(this, tr("Error"), tr("The attribute name may not be empty nor contain dots!"), QMessageBox::Ok);
}

// ------------------------------------------------------------
void EntityEditorTab::on_removeAttributeButton_clicked()
{
	QtProperty* curProp = ui->attributeBrowser->GetCurrentItem()->property();

	if(!curProp->propertyName().contains('.'))
	{
		ui->attributeBrowser->RemoveProperty(curProp);

		if(ui->attributeBrowser->GetTopLevelItemCount())
		{
			ui->removeAttributeButton->setEnabled(true);
		}

		else
			ui->removeAttributeButton->setEnabled(false);
	}

	else
		QMessageBox::critical(this, tr("Error"), tr("This attribute belongs to a component descriptor you attached! If you want to remove it, you'll have to remove the "
			"component descriptor ") + curProp->propertyName().mid(0, curProp->propertyName().indexOf('.')) + tr(" first."), QMessageBox::Ok);
}

// ------------------------------------------------------------
void EntityEditorTab::on_addComponentButton_clicked()
{
	QListWidgetItem* item = new QListWidgetItem(ui->componentsCombo->currentText());
	item->setData(Qt::UserRole, ui->componentsCombo->itemData(ui->componentsCombo->currentIndex()));
	
	// See what component descriptor is being added and add its parameters to the attribute browser.
	QByteArray array = item->data(Qt::UserRole).toByteArray();
	UUID componentUUID(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));

	ComponentDescriptor* descr = EntityDatabase::Instance().GetComponentDescriptor(componentUUID);

	const ComponentDescriptor::Parameters& params = descr->GetParameters();

	for(auto i(params.begin()); i != params.end(); ++i)
		ui->attributeBrowser->AddNewProperty(Aeon::AttributeKey(descr->GetComponentID().key + '.' + i->first), i->second, 0);

	ui->attributeBrowser->ResetSelection();
	
	// Make sure the lists remain updated.
	ui->componentsList->addItem(item);
	ui->componentsCombo->removeItem(ui->componentsCombo->currentIndex());

	ui->componentsCombo->setEnabled(ui->componentsCombo->count());
	ui->removeComponentButton->setEnabled(ui->componentsList->count());
	ui->addComponentButton->setEnabled(ui->componentsCombo->count());
}

// ------------------------------------------------------------
void EntityEditorTab::on_removeComponentButton_clicked()
{
	if(QListWidgetItem* item = ui->componentsList->currentItem())
	{
		ui->componentsCombo->addItem(item->text(), item->data(Qt::UserRole));
		delete item;

		ui->componentsCombo->setEnabled(ui->componentsCombo->count());
		ui->removeComponentButton->setEnabled(ui->componentsList->count());
		ui->addComponentButton->setEnabled(ui->componentsCombo->count());
	}
}

// ------------------------------------------------------------
void EntityEditorTab::on_saveButton_clicked()
{
	/**
	 * Disclaimer: Do *NOT* touch unless you know what you're doing, also see AddAttributeToBrowser for the reason why!
	 */

	if(!selectedEntity)
		return;

	selectedEntity->ClearComponents();
	selectedEntity->ClearAttributes();

	LOG_DEBUG << "Saving entity " << selectedEntity->GetID().key;

	// Save the list of component descriptors.
	for(int i(0); i < ui->componentsList->count(); ++i)
	{
		QListWidgetItem* item = ui->componentsList->item(i);

		LOG_DEBUG << "Adding component descriptor " << item->text().toAscii().data();

		QByteArray array = item->data(Qt::UserRole).toByteArray();
		UUID componentUUID(*reinterpret_cast<uint64_t*>(array.data()), *(reinterpret_cast<uint64_t*>(array.data()) + 1));

		selectedEntity->AddComponent(EntityDatabase::Instance().GetComponentDescriptor(componentUUID));
	}

	ui->attributeBrowser->ApplyChangesToContainer(selectedEntity);
}

// ------------------------------------------------------------
void EntityEditorTab::on_revertButton_clicked()
{
	SyncTableControls();
	RefreshAttributesTable();
	RefreshComponentsList();
}

// -------------------------------------------------------------------------
QTreeWidgetItem* EntityEditorTab::FindCategory(const Aeon::EntityCategory* cat)
{
	QString name = QString::fromStdString(cat->GetName());
	
	for(int i = 0; i < ui->entitiesTree->topLevelItemCount(); i++)
	{
		QTreeWidgetItem* ptr = ui->entitiesTree->topLevelItem(i);
		if(ptr && ptr->text(0)==name) return ptr;
	}

	return ui->entitiesTree->topLevelItem(0);
}

// -----------------------------------------------------------------------------
void EntityEditorTab::on_addCategoryButton_clicked()
{
	bool ok;
	QString catName = "New category";

	for(;;)
	{
		catName = QInputDialog::getText(this, "", tr("New unique category name:"), QLineEdit::Normal, catName, &ok);

		if(!ok)
			return;

		if(catName.isEmpty())
		{
			QMessageBox::critical(this, tr("Error"), tr("Category name cannot be empty!"), QMessageBox::Ok);
			continue;
		}

		bool catExists(false);

		for(int y(0); y < ui->entitiesTree->topLevelItemCount(); ++y)
		{
			if(ui->entitiesTree->topLevelItem(y)->text(0) == catName)
			{
				catExists = true;
				break;
			}		
		}

		if(catExists)
		{
			QMessageBox::critical(this, tr("Error"), tr("An category with such a name already exists!"), QMessageBox::Ok);
			continue;
		}

		break;
	}


	if(EntityDatabase::Instance().AddCategory((catName.toAscii().data())))
	{
		QTreeWidgetItem* node = new QTreeWidgetItem(ui->entitiesTree, QStringList() << catName);
		node->setIcon(0, QIcon(":/EntityEditor/CategoryIcon"));
		node->setExpanded(true);
		ui->entitiesTree->addTopLevelItem(node);
	}
	
}

// -----------------------------------------------------------------------------
void EntityEditorTab::on_removeCategoryButton_clicked()
{
	if(QTreeWidgetItem* item = ui->entitiesTree->currentItem())
	{
		if(item->text(0)== "General") 
		{
			QMessageBox::critical(this, tr("Error"), tr("Cannot remove default category."), QMessageBox::Ok);
			return;
		}

		if(item->parent() != 0) return;

		if(item->childCount() &&
			QMessageBox::warning(this,tr("Confirm category removal."), tr("This category has entities assigned. " 
				"Removing this category will prompt you to select a category to assign those entities to."
				"Continue?"), QMessageBox::Yes|QMessageBox::No) == QMessageBox::No)
			return;
		
			
		QStringList available;
		for (int i(0); i < ui->entitiesTree->topLevelItemCount(); ++i)
		{
			if(ui->entitiesTree->topLevelItem(i)->text(0) != item->text(0)) 
				available << ui->entitiesTree->topLevelItem(i)->text(0);
		}


		bool ok;

		if(item->childCount())
		{
			QString selectedCat = QInputDialog::getItem(this, tr("Select Category"),
				tr("Assign entities to category:"), available, 0, false, &ok);

			if(!ok) return;

			Aeon::EntityCategory* newCat = EntityDatabase::Instance().GetCategory(selectedCat.toAscii().data());
			QTreeWidgetItem* cat = FindCategory(newCat);

			for(int i(0); i < item->childCount(); ++i)
			{
				QTreeWidgetItem* child = item->child(i);
				item->removeChild(child);
				cat->addChild(item);
				entitiesTreeWidgetsMap[child]->SetCategory(newCat);
			}
		}
		EntityDatabase::Instance().RemoveCategory(item->text(0).toAscii().data());

		delete ui->entitiesTree->takeTopLevelItem(ui->entitiesTree->indexOfTopLevelItem(item));

		emit OnCategoriesModified();
	}
}

// -----------------------------------------------------------------------------
void EntityEditorTab::on_renameCategoryButton_clicked()
{
	if(QTreeWidgetItem* item = ui->entitiesTree->currentItem())
	{
		if(item->parent()) return;
		if(item->text(0) == "General") return;
		
		QString oldName = item->text(0);

		CheckedNameInputDialog* dlg = new CheckedNameInputDialog();
		dlg->setWindowTitle(tr("Rename Category"));
		dlg->SetStandardText(tr("Enter new category name:"));
		dlg->SetErrorText(tr("A category with this name already exists!"));
		dlg->SetInputText(item->text(0));
		CategoryNameValidator* validator = new CategoryNameValidator();
		validator->widget = ui->entitiesTree;

		dlg->SetValidator(validator);

		if(dlg->exec() == QDialog::Accepted)
		{	
			item->setText(0, dlg->GetName());
			EntityDatabase::Instance().GetCategory(oldName.toAscii().data())->SetName(dlg->GetName().toAscii().data());
			emit OnCategoriesModified();
		}

		delete dlg;

	}
}

// -------------------------------------------------------------------------
void EntityEditorTab::on_setEntityCategoryButton_clicked()
{
	if(QTreeWidgetItem* item = ui->entitiesTree->currentItem())
	{
		if(!item->parent() || ui->entitiesTree->topLevelItemCount() < 2) return;

		QString catName = QString::fromStdString(entitiesTreeWidgetsMap[item]->GetCategory()->GetName());
		QStringList available;

		for (int i(0); i < ui->entitiesTree->topLevelItemCount(); ++i)
		{
			if(ui->entitiesTree->topLevelItem(i)->text(0) != catName) 
				available << ui->entitiesTree->topLevelItem(i)->text(0);
		}
		
		bool ok;

		QString selectedCat = QInputDialog::getItem(this, tr("Select Category"),
			tr("Assign entity to category:"), available, 0, false, &ok);

		if(!ok) return;

		Aeon::EntityCategory* newCat = EntityDatabase::Instance().GetCategory(selectedCat.toAscii().data());
		QTreeWidgetItem* cat = FindCategory(newCat);

		QTreeWidgetItem* oldCat = FindCategory(entitiesTreeWidgetsMap[item]->GetCategory());
		oldCat->removeChild(item);
		cat->addChild(item);
		entitiesTreeWidgetsMap[item]->SetCategory(newCat);

		emit OnCategoriesModified();
	}
}
