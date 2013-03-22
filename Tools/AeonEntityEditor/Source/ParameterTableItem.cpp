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

 #include "ParameterTableItem.h"

// -------------------------------------------------------------------------
ParameterTableItem::ParameterTableItem():
	QTableWidgetItem()
{

}

// -------------------------------------------------------------------------
ParameterTableItem::ParameterTableItem(const QString &text) : 
	QTableWidgetItem(text)
{

}

// -------------------------------------------------------------------------
QTableWidgetItem * ParameterTableItem::clone() const 
{
	ParameterTableItem *item = new ParameterTableItem();
	*item = *this;
	return item;
}

// -------------------------------------------------------------------------
QVariant ParameterTableItem::data(int role) const 
{
	return QTableWidgetItem::data(role);
}

// -------------------------------------------------------------------------
void ParameterTableItem::setData(int role, const QVariant &value)
{
	QTableWidgetItem::setData(role, value);
}
