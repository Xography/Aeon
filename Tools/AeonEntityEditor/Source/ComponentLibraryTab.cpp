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
#include <QListView>
#include <QMessageBox>
#include <QListWidgetItem>

#include "ComponentLibraryTab.h"
#include "DatabaseManager.h"
#include "CheckedNameInputDialog.h"
#include "ParameterItemDelegate.h"
#include "ParameterTableItem.h"

// -------------------------------------------------------------------------
// Delegate item for the parameter table
static ParameterItemDelegate* delegateItem = new ParameterItemDelegate();

// -------------------------------------------------------------------------
// Checks whether a component already exists in the database
class ComponentValidator : public IValidationFunction
{
	bool Validate(const QString& input) 
	{
		return DatabaseManager::GetInstance()->HasComponentDescriptor(input);
	}
};

// -------------------------------------------------------------------------
// This validator excludes the name of the component to rename so it doesn't show an error
class ComponentRenameValidator : public IValidationFunction
{
public:
	QString previous;

	bool Validate(const QString& input) 
	{
		return input == previous ? true : DatabaseManager::GetInstance()->HasComponentDescriptor(input);
	}
};

// -------------------------------------------------------------------------
ComponentLibraryTab::ComponentLibraryTab(QWidget *parent /*= 0*/) :
	QWidget(parent),
	ui(new Ui::ComponentLibraryTab),
	prototype(0),
	edited(false),
	previous(0)
{
	ui->setupUi(this);

	connect(DatabaseManager::GetInstance(), SIGNAL(database_opened()), this, SLOT(onDatabaseOpened()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_created()), this, SLOT(onDatabaseOpened()));
	connect(ui->listWidgetComponents, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(OnComponentSelected(QListWidgetItem*,QListWidgetItem*)));
	connect(ui->listWidgetComponents, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(OnTableItemChanged(QListWidgetItem*)));
	connect(ui->lineEditFilter, SIGNAL(textChanged(const QString&)), this, SLOT(FilterTextChanged(const QString&)));
	connect(ui->buttonClearFilter, SIGNAL(clicked()), this, SLOT(OnClearFilter()));
	connect(ui->buttonRevert, SIGNAL(clicked()), this, SLOT(OnResetComponent()));	
	connect(ui->buttonSave, SIGNAL(clicked()), this, SLOT(OnApplyChanges()));
	connect(ui->buttonAddComponent, SIGNAL(clicked()), this, SLOT(OnAddComponent()));
	connect(ui->buttonCloneComponent, SIGNAL(clicked()), this, SLOT(OnCloneComponent()));
	connect(ui->buttonDeleteComponent, SIGNAL(clicked()), this, SLOT(OnRemoveComponent()));
	connect(ui->buttonAddParameter, SIGNAL(clicked()), this, SLOT(OnAddParameter()));
	connect(ui->buttonRemoveParameter, SIGNAL(clicked()), this, SLOT(OnRemoveParameter()));
	connect(ui->textEditDescription, SIGNAL(cursorPositionChanged()), this, SLOT(OnEdit()));
	connect(ui->tableWidgetParameters, SIGNAL(itemClicked (QTableWidgetItem *)), this, SLOT(OnTableItemClicked()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_changed()), this, SLOT(onDatabaseOpened()));

	ResetControls();
}

// -------------------------------------------------------------------------
ComponentLibraryTab::~ComponentLibraryTab()
{
	delete ui;
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::onDatabaseOpened()
{
	LOG_DEBUG << "ComponentLibraryTab: Database opened.";
	RefreshComponentList();
	ResetInputData();
	ResetControls();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::onDatabaseClosed()
{
	LOG_DEBUG << "ComponentLibraryTab: Database closed.";
	ui->listWidgetComponents->selectionModel()->reset();
	prototype = 0;
	ResetInputData();
	ResetControls();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::RefreshComponentList()
{
	DatabaseManager* ptr = DatabaseManager::GetInstance();
	
	for(int i = 0; i < ui->listWidgetComponents->count(); i++)
		delete ui->listWidgetComponents->item(i);
	
	ui->listWidgetComponents->clear();
	componentsListWidgetsMap.clear();
	
	for(int i = 0; i < ptr->GetComponentDescriptorCount(); i++)
		AddComponentToList(ptr->GetComponentDescriptor(i));
}

// -------------------------------------------------------------------------
bool ComponentLibraryTab::ConfirmDropChanges()
{
	return (QMessageBox::warning(this, tr("Confirm Discarding Changes"), tr(
		"All unsaved changes will be lost. "
		"Do you want to drop the changes you did to this component?"),
		QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::ResetControls()
{
	bool itemSelected = ui->listWidgetComponents->selectionModel()->currentIndex().row() >= 0;
	ui->groupBox->setEnabled(prototype || itemSelected);

	edited = false;
	ui->buttonSave->setEnabled(false);
	ui->buttonRevert->setEnabled(false);

	ui->buttonSave->setText(prototype ? tr("Add to Library") : tr("Apply Changes"));
	ui->buttonRevert->setText(prototype ? tr("Cancel") : tr("Revert Changes"));

	ui->buttonCloneComponent->setEnabled(itemSelected);
	ui->buttonDeleteComponent->setEnabled(itemSelected);
	ui->buttonAddParameter->setEnabled(itemSelected || prototype);
	ui->buttonRemoveParameter->setEnabled(false);

	ui->buttonClearFilter->setEnabled(false);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::ResetInputData()
{
	ui->textEditDescription->setPlainText("");

	QTableWidget* table = ui->tableWidgetParameters;
	
	table->clearContents();

	while(table->rowCount() > 0)
		table->removeRow(0);

	table->setItemDelegate(delegateItem);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::FetchData(const QString& name)
{
	Aeon::ComponentDescriptor* ptr = DatabaseManager::GetInstance()->GetComponentDescriptor(name);

	if(!ptr) return;

	ui->textEditDescription->blockSignals(true);
	ui->textEditDescription->setPlainText(QString::fromStdString(ptr->GetDescription()));
	ui->textEditDescription->blockSignals(false);

	// Add Parameters
	const Aeon::ComponentDescriptor::Parameters& parameters = ptr->GetParameters();

	LOG_DEBUG << "Getting " << parameters.size() << " parameters";

	QTableWidget* table = ui->tableWidgetParameters;
	static QList<QString> items = QString("Blob;Bool;Double;Float;Int32;Int64;Quaternion;Matrix4x4;Range;String;Vector2;Vector3;Vector4").split(";");
	
	for(Aeon::ComponentDescriptor::Parameters::const_iterator it = parameters.begin(); it != parameters.end(); ++it)
	{
		int row = table->rowCount();
		table->insertRow(row);
		table->setItem(row, 0, new ParameterTableItem(QString::fromStdString(it->first)));
		table->setItem(row, 1, new ParameterTableItem(items.at(it->second-1)));
		table->setVerticalHeaderItem(row, new QTableWidgetItem(QString("%0").arg(row+1)));
	}
	
	table->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::WriteData(Aeon::ComponentDescriptor* ptr)
{
	// Write Description
	ptr->SetDescription(ui->textEditDescription->toPlainText().toAscii().data());

	// Write Parameters
	QTableWidget* table = ui->tableWidgetParameters;

	for(int i = 0; i < table->model()->rowCount();i++)
	{
		ptr->SetParameter(table->item(i,0)->text().toAscii().data(), 
			(Aeon::IAttribute::Type)ParameterItemDelegate::StringToAttributeType(table->item(i,1)->text()));
	}
}

// -------------------------------------------------------------------------
QListWidgetItem* ComponentLibraryTab::AddComponentToList(Aeon::ComponentDescriptor* component)
{
	QListWidgetItem *item = new QListWidgetItem(
		QIcon(":/EntityEditor/Component"),
		QString(component->GetComponentID().key.c_str())
	);
	item->setFlags((item->flags() & ~Qt::ItemIsDropEnabled) | Qt::ItemIsEditable);
	ui->listWidgetComponents->addItem(item);
	componentsListWidgetsMap.insert(item, component);
	return item;
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnComponentSelected(QListWidgetItem* current, QListWidgetItem* previous)
{
	if(!current)
		return;

	else if(edited && !ConfirmDropChanges())
	{
		if(previous) previous->setSelected(true);
		if(current)  current->setSelected(false);
	}
	
	else
	{
		edited = false;
		previous = current;

		ResetInputData();
		ResetControls();
		FetchData(current->text());
	}
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::FilterTextChanged(const QString& str)
{
	for(int i(0); i < ui->listWidgetComponents->count(); ++i)
		ui->listWidgetComponents->setRowHidden(i, !ui->listWidgetComponents->item(i)->text().contains(str));
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnClearFilter()
{
	ui->lineEditFilter->setText("");
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnResetComponent()
{		
	ResetInputData();

	if(prototype)
	{
		Aeon::ComponentDescriptor::Destroy(prototype);
		prototype = 0;

		if(ui->listWidgetComponents->item(0))
		{
			ui->listWidgetComponents->item(0)->setSelected(true);
			FetchData(ui->listWidgetComponents->item(0)->text());
		}
	} 
	else
	{
		if(!ui->listWidgetComponents->selectedItems().size()) return;

		QListWidgetItem* item = ui->listWidgetComponents->currentItem();
		FetchData(item->text());
	}

	ResetControls();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnApplyChanges()
{
	// In case we have a prototype, just add it to the library and refresh the views.
	if(prototype)
	{
		DatabaseManager::GetInstance()->AddComponentDescriptor(prototype);
		WriteData(prototype);

		emit OnComponentDescriptorAdded(prototype);

		prototype = nullptr;

		RefreshComponentList();
		ResetInputData();
		ResetControls();

		return;
	}

	// Else, we apply the changes internally
	QString name = ui->listWidgetComponents->currentItem()->text();
	Aeon::ComponentDescriptor* ptr = DatabaseManager::GetInstance()->GetComponentDescriptor(name);
	if(!ptr)
	{
		LOG_ERROR << "Cannot find matching component descriptor to apply changes to: " << name.toAscii().data()
				  << "! Something went really wrong here.";
		return;
	}

	WriteData(ptr);
	ResetControls();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnAddComponent()
{
	if(edited && !ConfirmDropChanges()) return;

	QString compName = "New component";

	bool componentNameExists(false);
	int newComponentIndex(0);

	do {
		componentNameExists = false;
		for(int i(0); i < ui->listWidgetComponents->count(); ++i)
		{
			if(ui->listWidgetComponents->item(i)->text() == compName)
			{
				componentNameExists = true;
				compName = QString("New component %1").arg(++newComponentIndex);
				break;
			}
		}
	} while (componentNameExists);

	ResetInputData();
	Aeon::ComponentDescriptor* componentCreated = Aeon::ComponentDescriptor::Create(compName.toAscii().data());
	DatabaseManager::GetInstance()->AddComponentDescriptor(componentCreated);
	QListWidgetItem* itemAdded = AddComponentToList(componentCreated);
	ui->listWidgetComponents->setCurrentItem(itemAdded);
	ui->listWidgetComponents->editItem(itemAdded);

	ui->groupBox->setEnabled(true);
	ResetControls();
	OnEdit();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnRemoveComponent()
{
	QListWidgetItem* item = ui->listWidgetComponents->currentItem();
	if(item && (QMessageBox::question(this, tr("Confirm Deleting Component"), tr(
		"This operation will erase the component from the database. All entities referring to this component will be stripped of it! "
		"Also if you were editing an entity in the entity tab that has this component, the tables will be refreshed, so any unsaved "
		"changes there will be lost! Proceed with deleting this component?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes))
	{
		LOG_DEBUG << "Removing " << item->text().toAscii().data();

		emit OnComponentDescriptorRemoved(DatabaseManager::GetInstance()->GetComponentDescriptor(item->text()));

		DatabaseManager::GetInstance()->RemoveComponentDescriptor(item->text());
		RefreshComponentList();
		ResetInputData();
		ResetControls();
	}
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnCloneComponent()
{	
	CheckedNameInputDialog* dlg = new CheckedNameInputDialog();
	dlg->setWindowTitle(tr("Clone Component"));
	dlg->SetStandardText(tr("Enter a new name for the cloned component:"));
	dlg->SetErrorText(tr("A component with this name already exists!"));
	dlg->SetInputText(ui->listWidgetComponents->currentItem()->text()+"_1");	
	static IValidationFunction* validator = new ComponentValidator();
	dlg->SetValidator(validator);

	if(dlg->exec() == QDialog::Accepted)
	{
		// TODO: Needs to be implemented
		// [8/24/2012 Felix Bruckner]
	}
	delete dlg;
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnEdit()
{
	ui->buttonSave->setEnabled(true);
	ui->buttonRevert->setEnabled(true);
	edited = true;
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnAddParameter()
{
	QTableWidget* table = ui->tableWidgetParameters;
	int row = table->model()->rowCount();
	table->setItemDelegate(delegateItem);

	table->insertRow(row);
	table->setItem(row, 0, new ParameterTableItem(QString("Parameter%0").arg(row)));
	table->setItem(row, 1, new ParameterTableItem("Blob"));
	table->setVerticalHeaderItem(row, new QTableWidgetItem(QString("%0").arg(row+1)));
	
	// Select new item so the user can start entering a name straightaway
	QModelIndex index = table->model()->index(row,0);
	table->selectionModel()->select(index, QItemSelectionModel::Select);
	table->selectionModel()->setCurrentIndex(index,QItemSelectionModel::Select);
	table->setFocus();
	table->edit(index);

	OnEdit();
	ui->buttonRemoveParameter->setEnabled(ui->tableWidgetParameters->rowCount() > 0);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnRemoveParameter()
{
	ui->tableWidgetParameters->removeRow(ui->tableWidgetParameters->currentRow());

	// Correct row numbers to table
	for(int i = 0; i < ui->tableWidgetParameters->rowCount(); i++)
	{
		ui->tableWidgetParameters->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%0").arg(i+1)));
	}

	ui->buttonRemoveParameter->setEnabled(ui->tableWidgetParameters->rowCount() > 0);
	OnEdit();
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnTableItemClicked()
{
	ui->buttonRemoveParameter->setEnabled(true);
}

// -------------------------------------------------------------------------
void ComponentLibraryTab::OnTableItemChanged(QListWidgetItem* item)
{
	Aeon::ComponentDescriptor *component = componentsListWidgetsMap.value(item);

	for(int i(0); i < ui->listWidgetComponents->count(); ++i)
	{
		if(item != ui->listWidgetComponents->item(i) && 
			ui->listWidgetComponents->item(i)->text() == item->text())
		{
			QMessageBox::critical(this, tr("Error"), QString(tr("Component %1 already exists")).arg(item->text()), QMessageBox::Ok);
			if (component)
			{
				item->setText(QString::fromStdString(component->GetComponentID().key));
			}
			return;
		}
	}

	if (component && QString::fromStdString(component->GetComponentID().key).compare(item->text()))
	{
		Aeon::String* newName = new Aeon::String(item->text().toStdString());
		component->SetName(*newName);
	}
}