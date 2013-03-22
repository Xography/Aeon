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

 #include "ParameterNameValidator.h"

 // -------------------------------------------------------------------------
ParameterNameValidator::ParameterNameValidator(QWidget* parent, const QList<QString>& invalidNames) :
	QValidator(),
	invalidNames(invalidNames),
	parent(parent)
{

}

// -------------------------------------------------------------------------
QValidator::State ParameterNameValidator::validate(QString& input, int& pos) const 
{
	if(invalidNames.indexOf(input.toLower()) >= 0) 
	{
		// In case the user adds something in the lines of "Test2" and "Test" already exists, we don't want to block input
		emit ValidationStatusChanged(parent, QValidator::Intermediate);
		return QValidator::Intermediate;
	}

	if(!QTextCodec::codecForName("latin1")->canEncode(input))
	{
		// Reject anything we can't store in the database
		emit ValidationStatusChanged(parent, QValidator::Invalid);
		return QValidator::Invalid;
	}

	if(input.trimmed().isEmpty())
	{
		emit ValidationStatusChanged(parent, QValidator::Intermediate);
		return QValidator::Intermediate;
	}

	emit ValidationStatusChanged(parent, QValidator::Acceptable);
	return QValidator::Acceptable;
}
