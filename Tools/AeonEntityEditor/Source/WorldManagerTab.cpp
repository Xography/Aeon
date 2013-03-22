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
#include "WorldManagerTab.h"
#include "Plugin/PluginManager.h"

// -------------------------------------------------------------------------
// A validator that helps us to detect whether the given file already exists
class FileInputValidtor : public IValidationFunction
{
public:
	QString basepath;

	bool Validate(const QString& input) 
	{
		return QFile(basepath + "/" + input + (input.endsWith(".wdb") ? "" : ".wdb" )).exists();
	}
};

// -------------------------------------------------------------------------
WorldManagerTab::WorldManagerTab(QWidget* parent, QString settingsfile, IWorldEditorPlugin* plugin):
	QWidget(parent),
	plugin(plugin)
{
	isOpened = false;

	ui.setupUi(this);
	
	if(!plugin)
	{
		setEnabled(false);
		return;
	}
	
	if(QFile::exists(settingsfile))
	{
		QSettings settings(settingsfile, QSettings::IniFormat);
		settings.beginGroup("WorldEditor");
		{
			ui.lineEdit->setText(settings.value("WDBPath").toString());
		}

		settings.endGroup();
	} 

	if(ui.lineEdit->text().isEmpty()) ui.lineEdit->setText("../data/levels/");			

	connect(ui.pushButtonRefreshList, SIGNAL(clicked()), this, SLOT(OnRefreshList()));
	connect(ui.toolButtonSelectFolder, SIGNAL(clicked()), this, SLOT(OnSelectFolder()));
	connect(ui.pushButtonNew, SIGNAL(clicked()), this, SLOT(OnNew()));
	connect(ui.pushButtonDelete, SIGNAL(clicked()), this, SLOT(OnDelete()));
	connect(ui.pushButtonOpen, SIGNAL(clicked()), this, SLOT(OnOpen()));

	connect(ui.listWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(OnCurrentItemChanged(QListWidgetItem*, QListWidgetItem*)));

	ui.pushButtonOpen->setEnabled(false);
	ui.pushButtonDelete->setEnabled(false);

	ui.labelPluginName->setText(tr("Name: %1").arg(plugin->GetPluginName()));
	ui.labelPluginVersion->setText(tr("Version: %1").arg(plugin->GetPluginVersion()));
	ui.labelPluginDate->setText(tr("Date: %1").arg(plugin->GetPluginDate()));

	OnRefreshList();
}

// -------------------------------------------------------------------------
QString WorldManagerTab::GetCurrentWDBPath()
{
	return ui.lineEdit->text();
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnSelectFolder()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), ui.lineEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.lineEdit->setText(dir);
	OnRefreshList();
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnRefreshList()
{
	ui.listWidget->clear();

	if(!QDir(ui.lineEdit->text()).exists())
		return;

	QDir dir(ui.lineEdit->text());
	QStringList files = dir.entryList(QStringList("*.wdb"), QDir::Files | QDir::NoSymLinks);

	for(QStringList::iterator it = files.begin(); it != files.end(); ++it)
		ui.listWidget->addItem(*it);

	ui.pushButtonOpen->setEnabled(false);
	ui.pushButtonDelete->setEnabled(false);
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnDelete()
{
	if(QMessageBox::warning(this, tr("Confirm file removal"), tr(
		"This will delete the world file from disk. This action cannot be undone. Are you sure you want to delete this file?"),
		QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) return;

	QFile(currentItem).remove();
	OnRefreshList();
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnCurrentItemChanged(QListWidgetItem* current, QListWidgetItem* /*previous*/)
{
	if(!current)
		return;

	currentItem = ui.lineEdit->text() + "/" + current->text();

	ui.pushButtonOpen->setEnabled(true);
	ui.pushButtonDelete->setEnabled(true);
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnOpen()
{
	if(plugin->GetFilename() == currentItem)
		return;
	
	/*
	if(!plugin->GetFilename().isEmpty())
	{
		if(QMessageBox::warning(this, tr("File already opened"), tr("There is already a file opened in the world editor (%1)."
										" Proceeding will drop all unsaved changes.").arg(currentItem), 
										QMessageBox::Ok|QMessageBox::Abort) == QMessageBox::Abort)
			return;
		
	}
	*/
	
	if(!plugin->OnFileOpen(currentItem))
	{
		QMessageBox::critical(this, tr("File Loading Error"), tr("Plugin error while trying to load WDB file: %1. Please see the log file.").arg(currentItem));
		return;
	}	
	
	if(!isOpened)
	{
		ui.WorldSubTabs->addTab(plugin->GetSettingsWidget(this), tr("World Settings"));
		ui.WorldSubTabs->addTab(plugin->GetWorldEditorWidget(this), tr("World Editor"));
		isOpened = true;
	}
	
	else
		plugin->Refresh();

	ui.WorldSubTabs->setTabText(2, plugin->GetFilename());
}

// -------------------------------------------------------------------------
void WorldManagerTab::OnNew()
{
	CheckedNameInputDialog* dlg = new CheckedNameInputDialog();
	dlg->setWindowTitle(tr("Create new World File"));
	dlg->SetStandardText(tr("Enter world file name:"));
	dlg->SetErrorText(tr("A file with this name already exists!"));
	dlg->SetInputText("new");	
	FileInputValidtor* validator = new FileInputValidtor();
	validator->basepath = ui.lineEdit->text(); 
	dlg->SetValidator(validator);

	if(dlg->exec() == QDialog::Accepted)
	{
		QString file = dlg->GetName();
		if(!file.endsWith(".wdb")) file.append(".wdb");

		if(!plugin->OnCreateNewFile(ui.lineEdit->text() + "/" + file))
		{
			QMessageBox::critical(this, tr("File Creation Error"), tr("Plugin error while trying to create WDB file: %1").arg(file));
			return;
		}

		OnRefreshList();
	}
}
