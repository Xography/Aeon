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
#include "AeonPropertyBrowser.h"

#include <QGridLayout>
#include <QtSpinBoxFactory>
#include <QtCheckBoxFactory>
#include <QtLineEditFactory>
#include <QtCheckBoxFactory>
#include <QtDoubleSpinBoxFactory>

#include <QtIntPropertyManager>
#include <QtBoolPropertyManager>
#include <QtGroupPropertyManager>
#include <QtDoublePropertyManager>
#include <QtStringPropertyManager>

#include <QMap>
#include <QVector>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QInputDialog>
#include <QSignalMapper>

#include <Aeon/Types/HashedKey.h>
#include <Aeon/Types/Blob.h>
#include <Aeon/Types/Matrix4x4.h>
#include <Aeon/Types/Quaternion.h>
#include <Aeon/Types/Range.h>
#include <Aeon/Types/UUID.h>
#include <Aeon/Types/Vector2.h>
#include <Aeon/Types/Vector3.h>
#include <Aeon/Types/Vector4.h>

using namespace Aeon;

// ----------------------------------------------------------------------------
AeonPropertyBrowser::AeonPropertyBrowser(QWidget *parent /*= 0*/):
	QWidget(parent)
{
	browser = new QtTreePropertyBrowser;

	QGridLayout *layout = new QGridLayout();
	layout->addWidget(browser, 0, 0);
	layout->setMargin(0);
	layout->setSpacing(0);
	setLayout(layout);

	// Create property managers.
	intManager = new QtIntPropertyManager;
	boolManager = new QtBoolPropertyManager;
	groupManager = new QtGroupPropertyManager;
	stringManager = new QtStringPropertyManager;
	doubleManager = new QtDoublePropertyManager;

	browser->setFactoryForManager(intManager, new QtSpinBoxFactory(browser));
	browser->setFactoryForManager(boolManager, new QtCheckBoxFactory(browser));
	browser->setFactoryForManager(stringManager, new QtLineEditFactory(browser));
	browser->setFactoryForManager(doubleManager, new QtDoubleSpinBoxFactory(browser));

	// Almost ready..
	browser->setAlternatingRowColors(true);

	connect(intManager, SIGNAL(valueChanged(QtProperty *, int)), this, SLOT(OnValueChanged(QtProperty *)));
	connect(boolManager, SIGNAL(valueChanged(QtProperty *, bool)), this, SLOT(OnValueChanged(QtProperty *)));
	connect(stringManager, SIGNAL(valueChanged(QtProperty *, const QString&)), this, SLOT(OnValueChanged(QtProperty *)));
	connect(doubleManager, SIGNAL(valueChanged(QtProperty *, double)), this, SLOT(OnValueChanged(QtProperty *)));
}

// ----------------------------------------------------------------------------
AeonPropertyBrowser::~AeonPropertyBrowser()
{
	// Yes, this seems to crash for some peculiar reason.
	delete intManager;
	delete boolManager;
	delete groupManager;
	delete stringManager;
	delete doubleManager;

	delete browser;
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::SetContainer(Aeon::AttributeContainer* selectedEntity)
{
	const Attributes& attrs = selectedEntity->GetAttributes();

	for(auto i(attrs.begin()); i != attrs.end(); ++i)
		AddNewProperty(i->first, i->second->GetType(), selectedEntity);

	ResetSelection();
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::AddNewProperty(const Aeon::AttributeKey& id, uint32_t type, Aeon::AttributeContainer* parentEntity)
{
	/**
	* Disclaimer: Do *NOT* touch this function unless you know what you are doing. Do not even change the order in which the subproperties
	* are added, assumptions are made based on these orderings and the type of the fields! If you are brave enough to modify this, you
	* should probably also take a look at on_saveButton_clicked.
	*/

	QtProperty* property = nullptr;

	switch(type)
	{
	case IAttribute::BLOB:
		{
			property = stringManager->addProperty(QString::fromStdString(id.key));
			//property->setEnabled(false); // Works but can't select for removal anymore.

			if(parentEntity)
			{
				// Convert blob to read-only data in hex format.
				QString readableData;
				auto& blob = parentEntity->GetAttribute<Blob>(id).GetBlob();

				size_t max = qMin<size_t>(blob.memSize, MAX_BLOB_VIEWABLE_BYTES);

				readableData.reserve((QString::number(0xFF, 16).length() + 1) * sizeof(uint8_t));

				for(size_t i(0); i < max; ++i)
				{
					readableData += QString::number(blob.data[i], 16);
					readableData += ' ';
				}

				stringManager->setValue(property, readableData);
			}

			break;
		}

	case IAttribute::BOOL:
		property = boolManager->addProperty(QString::fromStdString(id.key));

		if(parentEntity)
			boolManager->setValue(property, parentEntity->GetAttribute<bool>(id));

		break;

	case IAttribute::DOUBLE:
		property = doubleManager->addProperty(QString::fromStdString(id.key));
		doubleManager->setDecimals(property, MAX_DECIMALS_DOUBLE_SPINBOX);

		if(parentEntity)
			doubleManager->setValue(property, parentEntity->GetAttribute<double>(id));

		break;

	case IAttribute::FLOAT:
		property = doubleManager->addProperty(QString::fromStdString(id.key));
		doubleManager->setDecimals(property, MAX_DECIMALS_DOUBLE_SPINBOX);

		if(parentEntity)
			doubleManager->setValue(property, parentEntity->GetAttribute<float>(id));

		break;

	case IAttribute::INT32:
		property = intManager->addProperty(QString::fromStdString(id.key));

		if(parentEntity)
			intManager->setValue(property, parentEntity->GetAttribute<int32_t>(id));

		break;

	case IAttribute::INT64:
		property = stringManager->addProperty(QString::fromStdString(id.key));

		// QString("18446744073709551616").length() = 20, any 64-bit decimal integer can not be longer than 20 characters.
		stringManager->setRegExp(property, QRegExp("\\d{20}"));		

		if(parentEntity)
			stringManager->setValue(property, QString::number(static_cast<qlonglong>(parentEntity->GetAttribute<int64_t>(id))));

		break;

	case IAttribute::MATRIX4X4:
		{
			property = groupManager->addProperty(QString::fromStdString(id.key));

			Matrix4x4 m(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

			if(parentEntity)
				m = parentEntity->GetAttribute<Matrix4x4>(id);

			for(unsigned i(0); i < 4; ++i)
			{
				QtProperty* subPropertyX = doubleManager->addProperty("X"); doubleManager->setDecimals(subPropertyX, MAX_DECIMALS_DOUBLE_SPINBOX);
				QtProperty* subPropertyY = doubleManager->addProperty("Y"); doubleManager->setDecimals(subPropertyY, MAX_DECIMALS_DOUBLE_SPINBOX);
				QtProperty* subPropertyZ = doubleManager->addProperty("Z"); doubleManager->setDecimals(subPropertyZ, MAX_DECIMALS_DOUBLE_SPINBOX);
				QtProperty* subPropertyW = doubleManager->addProperty("W"); doubleManager->setDecimals(subPropertyW, MAX_DECIMALS_DOUBLE_SPINBOX);

				QtProperty* columnVector = groupManager->addProperty("x " + QString::number(i + 1));
				columnVector->addSubProperty(subPropertyX);
				columnVector->addSubProperty(subPropertyY);
				columnVector->addSubProperty(subPropertyZ);
				columnVector->addSubProperty(subPropertyW);
				property->addSubProperty(columnVector);

				if(parentEntity)
				{
					Vector4 v(0, 0, 0, 0);

					switch(i)
					{
					case 0: v = m.GetX0(); break;
					case 1: v = m.GetX1(); break;
					case 2: v = m.GetX2(); break;
					case 3: v = m.GetX3(); break;
					}

					doubleManager->setValue(subPropertyX, v.GetX());
					doubleManager->setValue(subPropertyY, v.GetY());
					doubleManager->setValue(subPropertyZ, v.GetZ());
					doubleManager->setValue(subPropertyW, v.GetW());
				}
			}

			break;
		}

	case IAttribute::RANGE:
		{
			QtProperty* subPropertyMin = doubleManager->addProperty("Minimum"); doubleManager->setDecimals(subPropertyMin, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyMax = doubleManager->addProperty("Maximum"); doubleManager->setDecimals(subPropertyMax, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyVal = doubleManager->addProperty("Value");   doubleManager->setDecimals(subPropertyVal, MAX_DECIMALS_DOUBLE_SPINBOX);

			property = groupManager->addProperty(QString::fromStdString(id.key));
			property->addSubProperty(subPropertyMin);
			property->addSubProperty(subPropertyMax);
			property->addSubProperty(subPropertyVal);

			if(parentEntity)
			{
				Range r = parentEntity->GetAttribute<Range>(id);

				doubleManager->setValue(subPropertyMin, r.GetLowerBound());
				doubleManager->setValue(subPropertyMax, r.GetUpperBound());
				doubleManager->setValue(subPropertyVal, r.GetValue());
			}

			break;
		}

	case IAttribute::STRING:
		property = stringManager->addProperty(QString::fromStdString(id.key));

		if(parentEntity)
			stringManager->setValue(property, QString::fromStdString(parentEntity->GetAttribute<String>(id)));

		break;

	case IAttribute::VECTOR2:
		{
			QtProperty* subPropertyX = doubleManager->addProperty("X"); doubleManager->setDecimals(subPropertyX, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyY = doubleManager->addProperty("Y"); doubleManager->setDecimals(subPropertyY, MAX_DECIMALS_DOUBLE_SPINBOX);

			property = groupManager->addProperty(QString::fromStdString(id.key));
			property->addSubProperty(subPropertyX);
			property->addSubProperty(subPropertyY);

			if(parentEntity)
			{
				Vector2 v = parentEntity->GetAttribute<Vector2>(id);

				doubleManager->setValue(subPropertyX, v.GetX());
				doubleManager->setValue(subPropertyY, v.GetY());
			}

			break;
		}

	case IAttribute::VECTOR3:
		{
			QtProperty* subPropertyX = doubleManager->addProperty("X"); doubleManager->setDecimals(subPropertyX, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyY = doubleManager->addProperty("Y"); doubleManager->setDecimals(subPropertyY, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyZ = doubleManager->addProperty("Z"); doubleManager->setDecimals(subPropertyZ, MAX_DECIMALS_DOUBLE_SPINBOX);

			property = groupManager->addProperty(QString::fromStdString(id.key));
			property->addSubProperty(subPropertyX);
			property->addSubProperty(subPropertyY);
			property->addSubProperty(subPropertyZ);

			if(parentEntity)
			{
				Vector3 v = parentEntity->GetAttribute<Vector3>(id);

				doubleManager->setValue(subPropertyX, v.GetX());
				doubleManager->setValue(subPropertyY, v.GetY());
				doubleManager->setValue(subPropertyZ, v.GetZ());
			}

			break;
		}

	case IAttribute::QUATERNION:
	case IAttribute::VECTOR4:
		{
			QtProperty* subPropertyX = doubleManager->addProperty("X"); doubleManager->setDecimals(subPropertyX, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyY = doubleManager->addProperty("Y"); doubleManager->setDecimals(subPropertyY, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyZ = doubleManager->addProperty("Z"); doubleManager->setDecimals(subPropertyZ, MAX_DECIMALS_DOUBLE_SPINBOX);
			QtProperty* subPropertyW = doubleManager->addProperty("W"); doubleManager->setDecimals(subPropertyW, MAX_DECIMALS_DOUBLE_SPINBOX);

			property = groupManager->addProperty(QString::fromStdString(id.key));
			property->addSubProperty(subPropertyX);
			property->addSubProperty(subPropertyY);
			property->addSubProperty(subPropertyZ);
			property->addSubProperty(subPropertyW);

			if(parentEntity)
			{
				Vector4 v(0, 0, 0, 0);

				if(type == IAttribute::VECTOR4)
					v = parentEntity->GetAttribute<Vector4>(id);

				else
					v = parentEntity->GetAttribute<Quaternion>(id).GetVector4();

				doubleManager->setValue(subPropertyX, v.GetX());
				doubleManager->setValue(subPropertyY, v.GetY());
				doubleManager->setValue(subPropertyZ, v.GetZ());
				doubleManager->setValue(subPropertyW, v.GetW());
			}

			break;
		}
	}

	if(property)
	{
		// Eh.. there is nowhere else to set this value and I need to retrieve it later?
		property->setWhatsThis(QString::number(type));
		browser->addProperty(property);
	}

	else
		LOG_WARNING << "An entity descriptor contains an attribute of an unknown type (key = " << id.key << ", type = " << type << ")";
}

// ----------------------------------------------------------------------------
QtBrowserItem* AeonPropertyBrowser::GetCurrentItem()
{
	return browser->currentItem();
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::RemoveProperty(QtProperty* ptr)
{
	browser->removeProperty(ptr);
	if(browser->topLevelItems().count()) browser->setCurrentItem(browser->topLevelItems()[0]);
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::ApplyChangesToContainer(Aeon::AttributeContainer* selectedEntity)
{
	// Save the attributes assigned to the entity descriptor.
	for(auto i(browser->properties().constBegin()); i != browser->properties().constEnd(); ++i)
	{
		LOG_DEBUG << "Adding attribute " << (*i)->propertyName().toAscii().data() << " (type " << (*i)->whatsThis().toInt() << ")";

		AttributeKey attrKey = AttributeKey(Aeon::String((*i)->propertyName().toAscii().data()));

		// Eh.. see AddAttributeToBrowser for this hacky business.
		switch((*i)->whatsThis().toInt())
		{
			// TODO: Finish the other attribute saving too.
		case IAttribute::BLOB:			selectedEntity->AddAttribute<Blob>(			attrKey, Blob()); /* Note: blob values aren't saved. */		break;
		case IAttribute::BOOL:			selectedEntity->AddAttribute<bool>(			attrKey, boolManager->value(*i));							break;
		case IAttribute::DOUBLE:		selectedEntity->AddAttribute<double>(		attrKey, doubleManager->value(*i));							break;
		case IAttribute::FLOAT:			selectedEntity->AddAttribute<float>(		attrKey, static_cast<float>(doubleManager->value(*i)));		break;
		case IAttribute::INT32:			selectedEntity->AddAttribute<int32_t>(		attrKey, intManager->value(*i));							break;
		case IAttribute::INT64:
			{
				bool ok(false);
				qlonglong value = stringManager->value(*i).toLongLong(&ok);

				if(ok)
					selectedEntity->AddAttribute<int64_t>(attrKey, static_cast<int64_t>(value));

				else // Should never happen since the field has a regex that only allows digits. Maybe in case of an overflow?
					LOG_WARNING << "The attribute " << attrKey.key << " is a 64-bit int but doesn't have a valid value set for it, this attribute will not be saved!";

				break;
			}

		case IAttribute::MATRIX4X4:
			{
				Matrix4x4 m(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

				m.SetX0(Vector4(
					doubleManager->value((*i)->subProperties()[0]->subProperties()[0]),
					doubleManager->value((*i)->subProperties()[0]->subProperties()[1]),
					doubleManager->value((*i)->subProperties()[0]->subProperties()[2]),
					doubleManager->value((*i)->subProperties()[0]->subProperties()[3])
					));

				m.SetX1(Vector4(
					doubleManager->value((*i)->subProperties()[1]->subProperties()[0]),
					doubleManager->value((*i)->subProperties()[1]->subProperties()[1]),
					doubleManager->value((*i)->subProperties()[1]->subProperties()[2]),
					doubleManager->value((*i)->subProperties()[1]->subProperties()[3])
					));

				m.SetX2(Vector4(
					doubleManager->value((*i)->subProperties()[2]->subProperties()[0]),
					doubleManager->value((*i)->subProperties()[2]->subProperties()[1]),
					doubleManager->value((*i)->subProperties()[2]->subProperties()[2]),
					doubleManager->value((*i)->subProperties()[2]->subProperties()[3])
					));

				m.SetX3(Vector4(
					doubleManager->value((*i)->subProperties()[3]->subProperties()[0]),
					doubleManager->value((*i)->subProperties()[3]->subProperties()[1]),
					doubleManager->value((*i)->subProperties()[3]->subProperties()[2]),
					doubleManager->value((*i)->subProperties()[3]->subProperties()[3])
					));

				selectedEntity->AddAttribute<Matrix4x4>(attrKey, m);

				break;
			}

		case IAttribute::QUATERNION:	selectedEntity->AddAttribute<Quaternion>(	attrKey, Quaternion(
											doubleManager->value((*i)->subProperties()[0]),
											doubleManager->value((*i)->subProperties()[1]),
											doubleManager->value((*i)->subProperties()[2]),
											doubleManager->value((*i)->subProperties()[3])));		break;
		case IAttribute::RANGE:			selectedEntity->AddAttribute<Range>(		attrKey, Range(
											doubleManager->value((*i)->subProperties()[0]),
											doubleManager->value((*i)->subProperties()[1]),
											doubleManager->value((*i)->subProperties()[2])));		break;
		case IAttribute::STRING:		selectedEntity->AddAttribute<String>(		attrKey, stringManager->value(*i).toAscii().data());	break;
		case IAttribute::VECTOR2:		selectedEntity->AddAttribute<Vector2>(		attrKey, Vector2(
											doubleManager->value((*i)->subProperties()[0]),
											doubleManager->value((*i)->subProperties()[1])));		break;
		case IAttribute::VECTOR3:		selectedEntity->AddAttribute<Vector3>(		attrKey, Vector3(
											doubleManager->value((*i)->subProperties()[0]),
											doubleManager->value((*i)->subProperties()[1]),
											doubleManager->value((*i)->subProperties()[2])));		break;
		case IAttribute::VECTOR4:		selectedEntity->AddAttribute<Vector4>(		attrKey, Vector4(
											doubleManager->value((*i)->subProperties()[0]),
											doubleManager->value((*i)->subProperties()[1]),
											doubleManager->value((*i)->subProperties()[2]),
											doubleManager->value((*i)->subProperties()[3])));		break;
		}
	}
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::Clear()
{
	browser->clear();
}

// ----------------------------------------------------------------------------
QList<QtBrowserItem*> AeonPropertyBrowser::GetTopLevelItems()
{
	return browser->topLevelItems();
}

// ----------------------------------------------------------------------------
int AeonPropertyBrowser::GetTopLevelItemCount()
{
	return browser->topLevelItems().count();
}

// ----------------------------------------------------------------------------
void AeonPropertyBrowser::ResetSelection()
{
	if(browser->topLevelItems().count())
		browser->setCurrentItem(browser->topLevelItems()[0]);
}

// -------------------------------------------------------------------------
void AeonPropertyBrowser::OnValueChanged(QtProperty * property)
{
	emit OnPropertyEdited(property);
}
