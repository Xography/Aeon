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
#ifndef ComponentLibraryTab_h__
#define ComponentLibraryTab_h__

#include <QWidget>

#include "ui_ComponentLibraryTab.h"

// Forward Decl.
namespace Aeon {
	class ComponentDescriptor;
}

/**
 * This tab allows modifying component descriptors.
 */
class ComponentLibraryTab : public QWidget
{
	Q_OBJECT

public:
	/**
	* Constructor
	*/
	ComponentLibraryTab(QWidget *parent = 0);

	/**
	* Destructor
	*/
	virtual ~ComponentLibraryTab();

signals:
	void OnComponentDescriptorAdded(Aeon::ComponentDescriptor* descriptor);
	void OnComponentDescriptorRemoved(Aeon::ComponentDescriptor* descriptor);

public slots:
	// DatabaseManager signals
	void onDatabaseOpened();
	void onDatabaseClosed();

private slots:
	/**
	 * Called when a component was clicked in the list.
	 */
	void OnComponentSelected(QListWidgetItem* current, QListWidgetItem* previous);
	
	/**
	 * Called when the text inside the component name filter box changes.
	 */
	void FilterTextChanged(const QString& str);
	
	/**
	 * Called when the user presses the "Clear Filter" button.
	 */
	void OnClearFilter();
	
	/**
	 * Called when the user presses the "Reset Changes" button.
	 */
	void OnResetComponent();

	/**
	 * Called when the user presses the "Apply Changes" button.
	 */
	void OnApplyChanges();
	
	/**
	 * Called when the user presses the "Add Component" button.
	 */
	void OnAddComponent();

	/**
	 * Called when the user presses the "Remove Component" button.
	 */
	void OnRemoveComponent();
	
	/**
	 * Called when the user presses the "Clone Component" button.
	 */
	void OnCloneComponent();

	/**
	 * Called whenever an edit occurred.
	 */
	void OnEdit();

	/**
	 * Called when the user presses the "Add Parameter" button.
	 */
	void OnAddParameter();

	/**
	 * Called when the user presses the "Remove Parameter" button.
	 */
	void OnRemoveParameter();
	
	/**
	 * Called when the user clicks an item in the parameter table.
	 */
	void OnTableItemClicked();

	/**
	 * Called when the table item text was changed
	 */
	void OnTableItemChanged(QListWidgetItem* item);

private:
	/**
	 * Fetches data from database.
	 * 
	 * @param name name of the component
	 */
	void FetchData(const QString& name);

	/**
	 * Writes the given descriptor back into the database
	 */
	void WriteData(Aeon::ComponentDescriptor* ptr);

	/**
	 * Asks the user to confirm dropping his changes
	 */
	bool ConfirmDropChanges();

	/**
	 * Refreshes the list of components
	 */
	void RefreshComponentList();

	/**
	 * Resets UI buttons
	 */
	void ResetControls();

	/**
	 * Resets all fields that may had their input data changed.
	 */
	void ResetInputData();

	/**
	 * Adds component data to components list
	 *
	 * @param pointer to component descriptor
	 *
	 * @return pointer to created list item
	 */
	QListWidgetItem* AddComponentToList(Aeon::ComponentDescriptor* component);

private:
	/**
	 * UI object.
	 */
	Ui::ComponentLibraryTab* ui;

	/**
	 * Prototype object where edits occur before being commited back into the database.
	 */
	Aeon::ComponentDescriptor* prototype;

	/**
	 * Edit flag.
	 */
	bool edited;

	/**
	 * Previous item that has been clicked in the list to be able to revert back.
	 */
	QListWidgetItem* previous;

	/**
	 * This map contains list items and components according to these items
	 */
	QMap<QListWidgetItem*, Aeon::ComponentDescriptor*>	componentsListWidgetsMap;
};

#endif // ComponentLibraryTab_h__
