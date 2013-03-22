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

#ifndef EntityEditorTab_h__
#define EntityEditorTab_h__

#include <QWidget>
#include <QMap>
#include <QVector>

#include <Aeon/EntityDescriptor.h>
#include <Aeon/Attribute.h>
#include <Aeon/EntityCategory.h>
#include "ui_EntityEditorTab.h"

class QComboBox;
class QtProperty;
class QTreeWidgetItem;
class QtIntPropertyManager;
class QtBoolPropertyManager;
class QtGroupPropertyManager;
class QtStringPropertyManager;
class QtDoublePropertyManager;

/**
* Represents widget of Entity editor tab
*/
class EntityEditorTab : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 */
	EntityEditorTab(QWidget* parent = 0);

	/**
	 * Destructor
	 */
	virtual ~EntityEditorTab();

signals:
	void OnCategoriesModified();

public slots:
	/**
	 * Manually connected slots.
	 */
	void OnDatabaseOpened();
	void OnDatabaseClosed();

	void OnComponentDescriptorAdded(Aeon::ComponentDescriptor* descriptor);
	void OnComponentDescriptorRemoved(Aeon::ComponentDescriptor* descriptor);

	/**
	 * Automatically connected slots.
	 */
	void on_addEntityButton_clicked();
	void on_removeEntityButton_clicked();
	void on_addAttributeButton_clicked();
	void on_removeAttributeButton_clicked();
	void on_addComponentButton_clicked();
	void on_removeComponentButton_clicked();
	void on_saveButton_clicked();
	void on_revertButton_clicked();
	void on_entitiesTree_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
	void on_entitiesTree_itemChanged(QTreeWidgetItem * item, int column);
	void on_addCategoryButton_clicked();
	void on_removeCategoryButton_clicked();
	void on_renameCategoryButton_clicked();
	void on_setEntityCategoryButton_clicked();

private:
	/**
	 * Enable or disable needed entities tree controls according database open state
	 */
	void RefreshEntitiesTreeControls();

	/**
	 *  * Makes sure the tables are either enabled or disabled based on the selected entity and the amount of attributes, components, ...
	 */
	void SyncTableControls();

	/**
	 * Load entity attributes to table widget
	 */
	void RefreshAttributesTable();

	/**
	 * Load entity components to list widget
	 */
	void RefreshComponentsList();
	
	/**
	 * Puts entity object in tree widget and adds new entry to entitiesTreeWidgetsMap
	 *
	 * @param entity Pointer to entity object
	 */
	QTreeWidgetItem* AddEntityToTree(Aeon::EntityDescriptor* entity);

	/**
	 * Removes all elements from Entities tree widget and clears entitiesTreeWidgetsMap
	 */
	void ClearEntitiesTree();
	
private:

	QTreeWidgetItem* FindCategory(const Aeon::EntityCategory* cat);

private:
	/**
	 * UI object.
	 */
	Ui::EntityEditorTab *ui;

	/**
	 * Map used to quickly obtain entity by tree item selection.
	 */
	QMap<QTreeWidgetItem*, Aeon::EntityDescriptor*>	entitiesTreeWidgetsMap;

	/**
	 * Pointer to currently selected entity in the tree
	 */
	Aeon::EntityDescriptor *selectedEntity;

	/**
	 * Property manager instances for the property browser.
	 */
	QtIntPropertyManager* intManager;
	QtBoolPropertyManager* boolManager;
	QtGroupPropertyManager* groupManager;
	QtStringPropertyManager* stringManager;
	QtDoublePropertyManager* doubleManager;
};

#endif // EntityEditorTab_h__
