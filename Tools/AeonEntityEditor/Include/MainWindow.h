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
#ifndef MainWindow_h__
#define MainWindow_h__

#include <QtGui/QMainWindow>

#include "ui_MainWindow.h"

class AboutDialog;
class WorldManagerTab;

/**
 * Represents the main window of the Aeon Editor.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 */
	MainWindow(QWidget* parent = 0, Qt::WFlags flags = 0);
	
	/**
	 * Destructor.
	 */
	~MainWindow();

private:
	/**
	 * Add actions and recently opened db files to File menu
	 */
	void AddFileActions();

	void UpdateRecentFiles();

	/**
	 * Loading settings
	 */
	void LoadSettings();

	/**
	 * Saving settings
	 */
	void SaveSettings();

	/**
	 * Open database file
	 */
	void OpenFile(const QString& fname);

	/**
	 * Save database file
	 */
	bool SaveFile(const QString& fname);

	void closeEvent(QCloseEvent *event);


public slots:
	/**
	 * These are slots that are formatted as on_<object_name>_<signal_name> and they are automatically triggered when the specified signal
	 * from the specified object is emitted.
	 */
	void on_ActionFileNew_triggered();
	void on_ActionFileOpen_triggered();
	void on_ActionFileSave_triggered();
	void on_ActionFileSaveAs_triggered();

	void on_ActionHelpAbout_triggered();

	void onRecentFile_triggered();

	void onDatabaseOpened();
	void onDatabaseClosed();


private:
	/**
	 * An instance of the about dialog.
	 */
	AboutDialog* dialogAbout;

	/**
	 * UI object.
	 */
	Ui::MainWindowClass *ui;
	WorldManagerTab* worldManagerTab;

	/**
	 * Actions for recently opened files
	 */
	QAction**	RecentFilesActions;

	QStringList	RecentFilesNames;
};

#endif // MainWindow_h__
