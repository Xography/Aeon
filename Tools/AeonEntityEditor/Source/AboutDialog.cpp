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
#include "AboutDialog.h"

// -------------------------------------------------------------------------
AboutDialog::AboutDialog(QWidget *parent, Qt::WFlags flags):
	QDialog(parent, flags)
{
	ui.setupUi(this);
}

// -------------------------------------------------------------------------
AboutDialog::~AboutDialog()
{

}
