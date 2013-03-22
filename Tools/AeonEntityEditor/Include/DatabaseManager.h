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
#ifndef	DatabaseManager_h__
#define	DatabaseManager_h__

#include <QObject>

#include <Aeon/EntityDatabase.h>

/**
 * @brief Wrapper for Aeon entity database operations.
 */
class DatabaseManager : public QObject
{
	Q_OBJECT

public:
	/**
	 * Constructor
	 */
	DatabaseManager();

	/**
	 * Destructor
	 */
	virtual ~DatabaseManager();

public:
	/**
	 * Singleton
	 */
	static DatabaseManager* GetInstance();

	/**
	 * @return True if database is opened or false otherwise
	 */
	inline bool IsOpened() const
	{
		return isOpened;
	}

	/**
	 * @return Currently opened database file name
	 */
	inline const Aeon::String& GetOpenedDatabaseFileName()
	{
		return openedDatabaseFileName;
	}

public:
	/**
	 * Opens entities database and gets entities list
	 *
	 * @param filename Name of database file
	 *
	 * @return True on success or false if error occured
	 */
	bool Open(const Aeon::String& filename);

	/**
	 * Creates new database
	 *
	 * @param name Name of new database created
	 * @param description Short database description (not necessarily)
	 */
	void Create(const Aeon::String& name, const Aeon::String& description = "");

	/**
	 * Saves current database to currently opened file
	 *
	 * @return True on success or false if error occured
	 */
	bool Save();

	/**
	 * Saves current database to file with name specified
	 *
	 * @param filename Name of database file
	 *
	 * @return True on success or false if error occured
	 */
	bool Save(const Aeon::String& filename);

	/**
	 * Closes opened database and frees context
	 */
	void Close();

	/**
	 * @return Count of entities stored in database
	 */
	int	GetEntitiesCount();

	/**
	 * Returns entity from given position in the list
	 *
	 * @param position Position of entity in list. Must be between 0 and GetEntitiesCount() result
	 *
	 * @return Pointer to entity placed on specified position in entities list or 0 on incorrect position
	 */
	Aeon::EntityDescriptor* GetEntityDescriptor(quint32 position);

	/**
	 * Returns entity by name
	 *
	 * @param position Position of entity in list. Must be between 0 and GetEntitiesCount() result
	 *
	 * @return Pointer to entity with given name or 0 if there no entity with specified name
	 */
	Aeon::EntityDescriptor* GetEntityDescriptor(const Aeon::String& name);

	/**
	 * Add new entity to database
	 *
	 * @param name Name of entity to create
	 *
	 * @return Pointer to entity created or 0 if creation/addition failed
	 */
	Aeon::EntityDescriptor* AddNewEntity(const Aeon::String& name);

	/**
	 * Returns true if the entity with the given name exists in the database.
	 */
	bool HasEntityDescriptor(const QString& name);

	/**
	 * Remove entity from database
	 *
	 * @param entity Pointer to entity should be removed
	 *
	 * @return True on success of false if database wasn't opened
	 */
	bool DeleteEntity(Aeon::EntityDescriptor* entity);

public:
	/**
	 * Returns the amount of component descriptors in the database.
	 */
	int GetComponentDescriptorCount();

	/**
	 * Gets the corresponding descriptor by name.
	 */
	Aeon::ComponentDescriptor* GetComponentDescriptor(const QString& name);

	/**
	 * Gets the corresponding descriptor by its position.
	 */
	Aeon::ComponentDescriptor* GetComponentDescriptor(quint32 pos);

	/**
	 * Returns true in case the database has a component descriptor with that name.
	 */
	bool HasComponentDescriptor(const QString& str);

	/**
	 * Adds the given descriptor to the database.
	 */
	void AddComponentDescriptor(Aeon::ComponentDescriptor* ptr);

	/**
	 * Removes the given descriptor from the database for good.
	 */
	void RemoveComponentDescriptor(const QString& name);
public:
	void NotifyDatabaseChanged()
	{
		emit database_changed();
	}	 

signals:
	void database_created();
	void database_opened();
	void database_saved();
	void database_closed();
	void database_changed();

private:
	const Aeon::UUID& EntityLookup(const QString& id);
	const Aeon::UUID& ComponentLookup(const QString& id);

private:
	static DatabaseManager *instance;

	Aeon::EntityDatabase		&database;
	const Aeon::EntityDatabase::EntityDescriptors* entitiesList;
	const Aeon::EntityDatabase::ComponentDescriptors* componentsList;
	bool						isOpened;
	Aeon::String				openedDatabaseFileName;
};


#endif //DatabaseManager_h__
