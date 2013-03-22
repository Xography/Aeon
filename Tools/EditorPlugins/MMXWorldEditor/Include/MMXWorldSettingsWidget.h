/*******************************************************************************
 *
 * This file is part of Sample Game.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 *
 * See LICENSE.txt for licensing details.
 *
 ******************************************************************************/
#ifndef AeonSettingsWidget_h__
#define AeonSettingsWidget_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Entity.h>
#include <QMessageBox>

#include "ui_WorldSettingsWidget.h"

#include "AeonEditorWorld.h"

/**
 * TODO: Move this to attribute/property editor & main editor code.
 */
class AeonSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Ctor.
 	 */
	AeonSettingsWidget(QWidget* parent, AeonEditorWorld* world);
public:
	
	/**
	 * Sets another worldpointer as active world.
	 */
	void SetWorld(AeonEditorWorld* world);

private slots:
	/**
	 * A button in the button box was clicked.
	 */
	void OnButtonClicked(QAbstractButton* btn);
	
	/**
	 * Save button clicked
	 */
	void OnSave();

	/**
	 * Reset button clicked
	 */
	void OnReset();

private:
	/**
	 * Checks whether the attributes in the given entity are valid.
	 */
	bool CheckAttribute(Aeon::Entity* settings, const Aeon::String& name, Aeon::IAttribute::Type type);

private:
	/**
	 * 
	 */
	AeonEditorWorld* world;

	/**
	 * UI object.
	 */
	Ui::WorldSettings ui;
};

#endif // AeonSettingsWidget_h__
