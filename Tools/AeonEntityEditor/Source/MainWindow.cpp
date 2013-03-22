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
#include <QFileDialog>
#include <QMessageBox>

#include <qsettings.h>
#include <qtpropertymanager.h>

#include "MainWindow.h"
#include "AboutDialog.h"
#include "EntityEditorTab.h"
#include "ComponentLibraryTab.h"
#include "WorldManagerTab.h"

#include "DatabaseManager.h"
#include "AeonWorldEditorPlugin.h"
#include "Plugin/PluginManager.h"

#define SETTINGS_FILE			"Settings.ini"
#define MAX_RECENT_FILES		5

// -------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags):
	QMainWindow(parent, flags),
	ui(new Ui::MainWindowClass),
	worldManagerTab(nullptr)
{
	ui->setupUi(this);

	connect(DatabaseManager::GetInstance(), SIGNAL(database_opened()),  this, SLOT(onDatabaseOpened()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_created()), this, SLOT(onDatabaseOpened()));
	connect(DatabaseManager::GetInstance(), SIGNAL(database_closed()),  this, SLOT(onDatabaseClosed()));

	// Create the necessary dialogs.
	dialogAbout = new AboutDialog(this);

	EntityEditorTab* entityEditorTab = new EntityEditorTab(this);
	ComponentLibraryTab* componentLibraryTab = new ComponentLibraryTab(this);	
	worldManagerTab = new WorldManagerTab(this, SETTINGS_FILE, new AeonWorldEditorPlugin());

	connect(componentLibraryTab, SIGNAL(OnComponentDescriptorAdded(Aeon::ComponentDescriptor*)),   entityEditorTab, SLOT(OnComponentDescriptorAdded(Aeon::ComponentDescriptor*)));
	connect(componentLibraryTab, SIGNAL(OnComponentDescriptorRemoved(Aeon::ComponentDescriptor*)), entityEditorTab, SLOT(OnComponentDescriptorRemoved(Aeon::ComponentDescriptor*)));

	PluginManager::Instance().RegisterTabWidget("Entity Editor", entityEditorTab);

	ui->MainTabWidget->addTab(entityEditorTab, tr("Entity Editor"));
	ui->MainTabWidget->addTab(componentLibraryTab, tr("Component Library"));
	ui->MainTabWidget->addTab(worldManagerTab, tr("World Manager"));
	ui->MainTabWidget->setDisabled(true);

	RecentFilesActions = new QAction*[MAX_RECENT_FILES];

	LoadSettings();
	AddFileActions();
	UpdateRecentFiles();
}

// -------------------------------------------------------------------------
MainWindow::~MainWindow()
{
	SaveSettings();
	delete dialogAbout;
	delete ui;
}

// -------------------------------------------------------------------------
void MainWindow::AddFileActions()
{
	ui->MenuFile->addAction(ui->ActionFileNew);
	ui->MenuFile->addAction(ui->ActionFileOpen);
	ui->MenuFile->addAction(ui->ActionFileSave);
	ui->MenuFile->addAction(ui->ActionFileSaveAs);
	ui->MenuFile->addSeparator();
	
	for (int i(0); i < MAX_RECENT_FILES; ++i)
	{
		RecentFilesActions[i] = new QAction(ui->MenuFile);
		ui->MenuFile->addAction(RecentFilesActions[i]);
		connect(RecentFilesActions[i], SIGNAL(triggered()), this, SLOT(onRecentFile_triggered()));
	}

	ui->MenuFile->addSeparator();
	ui->MenuFile->addAction(ui->ActionFileQuit);
}

// -------------------------------------------------------------------------
void MainWindow::UpdateRecentFiles()
{
	for(int i(0); i < MAX_RECENT_FILES; ++i)
	{
		if (i >= RecentFilesNames.size())
			RecentFilesActions[i]->setVisible(false);
		
		else
		{
			RecentFilesActions[i]->setText(RecentFilesNames.at(i));
			RecentFilesActions[i]->setVisible(true);
		}
	}
}

// -------------------------------------------------------------------------
void MainWindow::LoadSettings()
{
	if(QFile::exists(SETTINGS_FILE))
	{
		QSettings settings(SETTINGS_FILE, QSettings::IniFormat);

		settings.beginGroup("EntityEditor");
		{
			dialogAbout->restoreGeometry(settings.value("DialogAboutGeometry").toByteArray());
			restoreGeometry(settings.value("MainWindowGeometry").toByteArray());
			restoreState(settings.value("MainWindowState").toByteArray());
			RecentFilesNames = settings.value("RecentlyOpenedFiles").toStringList();
		}

		settings.endGroup();
	}
}

// -------------------------------------------------------------------------
void MainWindow::SaveSettings()
{
	QSettings settings(SETTINGS_FILE, QSettings::IniFormat);

	settings.beginGroup("EntityEditor");
	{
		settings.setValue("MainWindowState", saveState());
		settings.setValue("MainWindowGeometry", saveGeometry());
		settings.setValue("DialogAboutGeometry", dialogAbout->saveGeometry());
		settings.setValue("RecentlyOpenedFiles", RecentFilesNames);
	}
	settings.endGroup();

	settings.beginGroup("WorldEditor");
	{
		settings.setValue("WDBPath", worldManagerTab->GetCurrentWDBPath());
	}
	settings.endGroup();
}

// -------------------------------------------------------------------------
void MainWindow::OpenFile(const QString& fname)
{
	if(!DatabaseManager::GetInstance()->Open(fname.toAscii().data()))
	{
		QMessageBox::critical(this, tr("Error"), QString(tr("Failed to open %1")).arg(fname), QMessageBox::Ok);
	}

	RecentFilesNames.prepend(fname);
	RecentFilesNames.removeDuplicates();
	UpdateRecentFiles();
}

// -------------------------------------------------------------------------
bool MainWindow::SaveFile(const QString& fname)
{
	if(!DatabaseManager::GetInstance()->Save(fname.toAscii().data()))
	{
		QMessageBox::critical(this, tr("Error"), QString(tr("Failed to save %1")).arg(fname), QMessageBox::Ok);
		return false;
	}

	RecentFilesNames.prepend(fname);
	RecentFilesNames.removeDuplicates();
	UpdateRecentFiles();

	return true;
}

// -------------------------------------------------------------------------
void MainWindow::on_ActionFileNew_triggered()
{
	if(DatabaseManager::GetInstance()->IsOpened() && QMessageBox::warning(this, tr("Warning"), tr("Another database is currently opened. All unsaved changes will be lost. Continue anyway?"),
		QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
			return;

	DatabaseManager::GetInstance()->Create("EntitiesDb");
}

// -------------------------------------------------------------------------
void MainWindow::on_ActionFileOpen_triggered()
{
	if(DatabaseManager::GetInstance()->IsOpened())
	{
		if(QMessageBox::warning(this, tr("Warning"), tr("Another database currently opened. "
							  "All unsaved changes will be lost. Do You want to open another database anyway?"),
								 QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
		{
			return;
		}

		DatabaseManager::GetInstance()->Close();
	}

	QString filename = QFileDialog::getOpenFileName(this, tr("Open entity database"), QDir::currentPath(), tr("Entity database file (*.db)"));

	if(!filename.isEmpty())
	{
		OpenFile(filename);
	}
}

// -------------------------------------------------------------------------
void MainWindow::on_ActionFileSave_triggered()
{
	if(DatabaseManager::GetInstance()->GetOpenedDatabaseFileName().empty())
	{
		emit on_ActionFileSaveAs_triggered();
	}
	
	else
	{
		SaveFile(QString::fromStdString(DatabaseManager::GetInstance()->GetOpenedDatabaseFileName()));
	}
}

// -------------------------------------------------------------------------
void MainWindow::on_ActionFileSaveAs_triggered()
{
	QString filename = QFileDialog::getSaveFileName(this, tr("Save entity database"), QDir::currentPath(), tr("Entity database file (*.db)"));

	if(!filename.isEmpty())
	{
		SaveFile(filename);
	}
}

// -------------------------------------------------------------------------
void MainWindow::on_ActionHelpAbout_triggered()
{
	dialogAbout->exec();
}

// -------------------------------------------------------------------------
void MainWindow::onDatabaseOpened()
{
	ui->ActionFileSave->setEnabled(true);
	ui->ActionFileSaveAs->setEnabled(true);
	ui->MainTabWidget->setEnabled(true);
}

// -------------------------------------------------------------------------
void MainWindow::onDatabaseClosed()
{
	ui->ActionFileSave->setEnabled(false);
	ui->ActionFileSaveAs->setEnabled(false);
	ui->MainTabWidget->setEnabled(false);
}

//--------------------------------------------------------------------------
void MainWindow::onRecentFile_triggered()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action) {
		if(DatabaseManager::GetInstance()->IsOpened())
		{
			if(QMessageBox::warning(this, tr("Warning"), tr("Another database currently opened. "
								  "All unsaved changes will be lost. Do You want to open another database anyway?"),
									 QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
			{
				return;
			}

			DatabaseManager::GetInstance()->Close();
		}

		OpenFile(action->text());
	}
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(QMessageBox::warning(this, tr("Warning"), tr("Database currently opened. "
						  "All unsaved changes will be lost. Do You want to save changes?"),
							 QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	{
		QString filename = QString::fromStdString(DatabaseManager::GetInstance()->GetOpenedDatabaseFileName());
		if(filename.isEmpty())
		{
			filename = QFileDialog::getSaveFileName(this, tr("Save entity database"), QDir::currentPath(), tr("Entity database file (*.db)"));
			if(filename.isEmpty())
			{
				event->ignore();
				return;
			}
		}

		if (!SaveFile(filename))
		{
			event->ignore();
			return;
		}
	}

	event->accept();
}
