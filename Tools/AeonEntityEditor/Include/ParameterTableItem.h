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
#ifndef ParameterTableItem_h__
#define ParameterTableItem_h__

#include <QTableWidgetItem>
#include <QtGui>

class ParameterTableItem : public QTableWidgetItem
{
public:
	/**
	 * Constructor
	 */
	ParameterTableItem(const QString &text);

	/**
	 * Constructor
	 */
	 ParameterTableItem();

public:
	// Implements QTableWidgetItem
	QTableWidgetItem *clone() const  override;
	QVariant data(int role) const  override;
	void setData(int role, const QVariant &value) override;
};

#endif // ParameterTableItem_h__
