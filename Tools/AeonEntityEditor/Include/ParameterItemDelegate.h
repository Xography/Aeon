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
#ifndef ParameterItemDelegate_h__
#define ParameterItemDelegate_h__

#include <QComboBox>
#include <QItemDelegate>

#include "ComponentLibraryTab.h"
#include "ParameterNameValidator.h"

/**
 * Table Item Delegate that allows us to edit parameters properly by providing a validating name input and a combobox for the parameter type.
 */
class ParameterItemDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 */
	ParameterItemDelegate(QObject* parent = 0);

public:
	/**
	 * Implements QItemDelegate
	 *
	 * @see QItemDelegate
	 */
	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem&,const QModelIndex& index) const override;
	void setEditorData(QWidget* editor, const QModelIndex& index) const override;
	void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex& index) const override;

public:
	/**
	 * Converts a QString to an Aeon::IAttribute::Type.
	 */
	static int StringToAttributeType(const QString& match);
	
public slots:
	/**
	 * This slot is called whenever a ParameterNameValidator changes its state so we can change the color of the line edit field.
	 */
	void OnValidationStatusChanged(QWidget* parent, QValidator::State state) const;

private:
	/**
	 *
	 */
	mutable bool lastStateValid;
};

#endif // ParameterItemDelegate_h__