/*******************************************************************************
* 
*  This file is part of the AEON Framework.
* 
*  -----------------------------------------------------------------------------
*  Copyright (C) 2012- The Aeon Development Team.
* 
*  See LICENSE.txt for licensing details.
* 
* *****************************************************************************/
#include "ParameterItemDelegate.h"

#define TYPE_COUNT					13

// -------------------------------------------------------------------------
static const char* const typeStrings[TYPE_COUNT] =
{
	"Blob",
	"Bool",
	"Double",
	"Float",
	"Int32",
	"Int64",
	"Quaternion",
	"Matrix4x4",
	"Range",
	"String",
	"Vector2",
	"Vector3",
	"Vector4"
};

// -------------------------------------------------------------------------
enum ParameterColumnPos
{
	PARAMETER_COL_NAME = 0,
	PARAMETER_COL_TYPE = 1
};

// -------------------------------------------------------------------------
ParameterItemDelegate::ParameterItemDelegate(QObject* parent):
	lastStateValid(false)
{

}

// -------------------------------------------------------------------------
QWidget* ParameterItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem&, const QModelIndex& index) const 
{
	if(index.column() == PARAMETER_COL_TYPE)
	{
		QComboBox* box = new QComboBox(parent);

		for(int i(0); i < TYPE_COUNT; ++i)
			box->addItem(typeStrings[i]);

		box->setEditable(false);
		box->setCurrentIndex(StringToAttributeType(index.model()->data(index.sibling(index.row(), index.column()), Qt::EditRole).toString())-1);

		return box;
	}

	// We need a validating editor to ensure that the user can't name two parameters the same
	QLineEdit* validatingEditor = new QLineEdit(parent);

	QList<QString> invalidNames;

	for (int i = 0; i<index.model()->rowCount(); i++)
	{
		if(i == index.row()) continue; // we don't want to add the current name to the list of invalid names
		invalidNames.push_back(index.model()->data(index.sibling(i, 0),  Qt::EditRole).toString().toLower());
	}

	ParameterNameValidator* validator = new ParameterNameValidator(validatingEditor, invalidNames);
	validatingEditor->setValidator(validator);

	connect(validator, SIGNAL(ValidationStatusChanged(QWidget*, QValidator::State)), this, SLOT(OnValidationStatusChanged(QWidget*, QValidator::State)));
	return validatingEditor;
}

// -------------------------------------------------------------------------
void ParameterItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const 
{
	if(index.column() == PARAMETER_COL_TYPE)
	{
		QComboBox* box = qobject_cast<QComboBox*>(editor);
		box->setCurrentIndex(StringToAttributeType(index.model()->data(index, Qt::EditRole).toString())-1);
		return;
	}

	if(index.column() == PARAMETER_COL_NAME)
	{
		QLineEdit* edit = qobject_cast<QLineEdit*>(editor);
		edit->setText(index.model()->data(index, Qt::EditRole).toString());
		return;
	}
}

// -------------------------------------------------------------------------
void ParameterItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const 
{
	if(index.column() == PARAMETER_COL_TYPE)
	{
		QComboBox* box = qobject_cast<QComboBox*>(editor);
		model->setData(index, box->currentText());
		return;
	}

	if(index.column() == PARAMETER_COL_NAME)
	{
		QLineEdit* edit = qobject_cast<QLineEdit*>(editor);

		if(lastStateValid)
			model->setData(index, edit->text());

		return;
	}
}

// -------------------------------------------------------------------------
int ParameterItemDelegate::StringToAttributeType(const QString& match)
{
	for(int i = 0; i < TYPE_COUNT; i++)
		if(typeStrings[i] == match)
			return ++i; // IAttributeTypes start at 1

	return 0; // IAttributeType::INVALID = 0
}

// -------------------------------------------------------------------------
void ParameterItemDelegate::OnValidationStatusChanged(QWidget* parent, QValidator::State state) const
{
	switch(state)
	{
	case QValidator::Invalid:
		parent->setStyleSheet("color:red;");
		lastStateValid = false;
		break;
	case QValidator::Intermediate:
		parent->setStyleSheet("color:orange");
		lastStateValid = false;
		break;
	case QValidator::Acceptable:
		parent->setStyleSheet("color:green;");
		lastStateValid = true;
	default:
		break;
	}
}
