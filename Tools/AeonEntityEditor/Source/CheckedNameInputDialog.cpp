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
#include "CheckedNameInputDialog.h"

// -------------------------------------------------------------------------
CheckedNameInputDialog::CheckedNameInputDialog() :
	standardText("Enter Name"),
	errorText("Invalid name"),
	validator(0)
{
	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);		
	ui.setupUi(this);
	connect(ui.lineEdit, SIGNAL(textChanged ( const QString& )), this, SLOT(OnTextChanged(const QString&)));
	connect(ui.lineEdit, SIGNAL(returnPressed()), this, SLOT(OnReturn()));
	ui.label->setText(standardText);
	ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	ui.icon->setPixmap(QIcon(":/EntityEditor/Info").pixmap(16,16));

	layout()->setSizeConstraint( QLayout::SetFixedSize );
}

// -------------------------------------------------------------------------
const QString& CheckedNameInputDialog::GetName()
{
	return name;
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::SetValidator(IValidationFunction* funct)
{
	this->validator = funct;
	OnTextChanged(ui.lineEdit->text());
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::SetStandardText(const QString& standard)
{
	standardText = standard;
	ui.label->setText(standardText);
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::SetErrorText(const QString& error)
{
	errorText = error;
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::SetInputText(const QString& text)
{
	ui.lineEdit->setText(text);
	OnTextChanged(text);
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::OnTextChanged(const QString& str)
{
	if(validator && validator->Validate(str))
	{
		ui.label->setText(tr("<font color=red>%0</font>").arg(errorText));
		ui.icon->setPixmap(QIcon(":/EntityEditor/Error").pixmap(16,16));
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	} 
	else 
	{
		if(str.length() < 1) 
		{
			ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
			return;
		}

		if(!QTextCodec::codecForName("latin1")->canEncode(str))
		{
			ui.label->setText(tr("<font color=red>Name must be in ASCII format.</font>"));
			ui.icon->setPixmap(QIcon(":/EntityEditor/Error").pixmap(16,16));
			return;
		}

		ui.label->setText(tr("%0").arg(standardText));			
		ui.icon->setPixmap(QIcon(":/EntityEditor/Info").pixmap(16,16));
		ui.buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		name = str;
	}
}

// -------------------------------------------------------------------------
void CheckedNameInputDialog::OnReturn()
{
	if(validator && validator->Validate(ui.lineEdit->text()) && ui.lineEdit->text().length() > 0)
	{
		ui.buttonBox->button(QDialogButtonBox::Ok)->click();
	}
}

