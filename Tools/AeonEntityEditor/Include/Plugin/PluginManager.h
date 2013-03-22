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
#ifndef PluginManager_h__
#define PluginManager_h__

#include <QString>
#include <map>
#include <QWidget>

/**
 * This (future singleton) class provides editor plugins with required global information.
 */
class PluginManager : public Aeon::Singleton<PluginManager>
{
	friend class Aeon::Singleton<PluginManager>;
public:
	/**
	 * Returns the name of global Aeon Editor settings file.
	 */
	static QString GetSettingsFileName()
	{
		return "Settings.ini";
	}

public:
	QWidget* GetTabWidgetPtr(const QString& name)
	{
		return tabs[name];
	}

	void RegisterTabWidget(const QString& name, QWidget* tab)
	{
		tabs[name] = tab;
	}
private:
	std::map<const QString, QWidget*> tabs;
};

#endif // PluginManager_h__