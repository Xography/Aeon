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
#ifndef AboutDialog_h__
#define AboutDialog_h__

#include <QtGui/QDialog>

#include "ui_AboutDialog.h"

/**
 * Represents the about dialog.
 */
class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 */
	AboutDialog(QWidget* parent = 0, Qt::WFlags flags = 0);
	
	/**
	 * Destructor.
	 */
	~AboutDialog();

private:
	/**
	 * UI object.
	 */
	Ui::AboutDialogClass ui;
};

#endif // AboutDialog_h__
