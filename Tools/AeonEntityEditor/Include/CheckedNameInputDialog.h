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
#ifndef CheckedNameInputDialog_h__
#define CheckedNameInputDialog_h__

#include <QDialog>
#include <QPushButton>
#include <QTextCodec>

#include "ui_NameInputDialog.h"

/**
 * Interface to provide the Input Dialogue with a Validator.
 * Since moc doesn't support templates we have to use the strategy pattern here.
 */
class IValidationFunction
{
public:
	/**
	 * Function that validates the given input string against a user-provided implementation.
	 */
	virtual bool Validate(const QString& input) = 0;
};

/**
 * This class provides a field for input that needs to be validated, similar to a QValidator, but on a dialog basis.
 */
class CheckedNameInputDialog: public QDialog 
{

	Q_OBJECT

public:
	/**
	 * Constructor. Creates a new dialog.
	 */
	CheckedNameInputDialog();

public:
	/**
	 * Get the name that was successfully entered.
	 */
	const QString& GetName();

	/**
	 * Set the validation function.
	 */
	void SetValidator(IValidationFunction* funct);

	/**
	 * Set the text to be displayed when the input is OK.
	 */
	void SetStandardText(const QString& standard);

	/**
	 * Set the text to be displayed when the input is erroneous.
	 */
	void SetErrorText(const QString& error);

	/**
	 * Set the template text to be displayed in the line edit.
	 */
	void SetInputText(const QString& text);

private slots:
	/**
	 * Called when the input text is changed
	 */
	void OnTextChanged(const QString& str);

	/**
	 * Called when the Return key is pressed.
	 */
	void OnReturn();

private:
	/**
	 * 
	 */
	QString name, standardText,errorText;

	/**
	 * Validation strategy
	 */
	IValidationFunction* validator;

	/**
	 * UI
	 */
	Ui::NameInputDialog ui;

};

#endif // CheckedNameInputDialog_h__