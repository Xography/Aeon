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
#ifndef AeonPropertyBrowser_h__
#define AeonPropertyBrowser_h__

#include <Aeon/Aeon.h>
#include <Aeon/Entity.h>
#include <Aeon/Attribute.h>

#include <Aeon/AeonTypes.h>

#include <QtTreePropertyBrowser>
#include <QWidget>

class QComboBox;
class QtProperty;
class QTreeWidgetItem;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtGroupPropertyManager;
class QtStringPropertyManager;
class QtDoublePropertyManager;

#define MAX_DECIMALS_DOUBLE_SPINBOX		2		// The maximum allowed amount of decimals in a double spinbox (for float and double attributes).
#define ATTRIBUTE_TYPES_COUNT			14		// The amount of attributes in IAttribute::Type.
#define MAX_BLOB_VIEWABLE_BYTES			100		// The maximum amount of viewable read-only bytes for blob data.

/**
 * A property manager for Aeon::AttributeContainer classes.
 */
class AeonPropertyBrowser : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor.
	 */
	AeonPropertyBrowser(QWidget *parent = nullptr);

	/**
	 * Destructor.
	 */
	~AeonPropertyBrowser();

public: 
	/**
	 * Loads the attributes of the given attribute container (Entity or EntityDescriptor) into the browser.
	 */
	void SetContainer(Aeon::AttributeContainer* selectedEntity);
	
	/**
	 * Adds the specified attribute to the attribute property browser.
	 *
	 * @param id The ID of the attribute.
	 * @param type The type of the attribute (should be one of the IAttribute::Type enum).
	 * @param parentEntity The entity descriptor the attribute belongs to. Leave at null if this is a newly created attribute, it is used to retrieve the existing value of the
	 * attribute if it is already present in this entity.
	 */
	void AddNewProperty(const Aeon::AttributeKey& id, uint32_t type, Aeon::AttributeContainer* parentEntity);

	/**
	 * Returns the currently selected item.
	 */
	QtBrowserItem* GetCurrentItem();
	
	/**
	 * Removes the given property from the browser.
	 */
	void RemoveProperty(QtProperty* ptr);

	/**
	 * Applies the changes to given container.
	 */
	void ApplyChangesToContainer(Aeon::AttributeContainer* selectedEntity);

	/**
	 * Clears the browser.
	 */
	void Clear();

	/**
	 * Returns a list of all top level items in the browser.
	 */
	QList<QtBrowserItem*> GetTopLevelItems();

	/**
	 * Returns the amount of items in the browser (only toplevel items are counted).
	 */
	int GetTopLevelItemCount();

	/**
	 * Resets the selection to the topmost item, if any.
	 */
	void ResetSelection();

signals:
	/**
	 * Sent when a property was edited.
	 */
	void OnPropertyEdited(QtProperty* current);

private slots:
	/**
	 * Called when the value of a property was modified.
	 */
	void OnValueChanged(QtProperty* property);

private:
	/**
	 * The browser implementation
	 */
	QtTreePropertyBrowser* browser;
	
	/**
	 * Property manager instances for the property browser.
	 */
	QtIntPropertyManager* intManager;
	QtBoolPropertyManager* boolManager;
	QtGroupPropertyManager* groupManager;
	QtStringPropertyManager* stringManager;
	QtDoublePropertyManager* doubleManager;
};

#endif // AeonPropertyBrowser_h__
