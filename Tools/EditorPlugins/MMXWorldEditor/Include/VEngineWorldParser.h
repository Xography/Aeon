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
#ifndef VEngineWorldParser_h__
#define VEngineWorldParser_h__

#include <QStringList>
#include <QFile>
#include <QTextStream>

/**
 * Parses a VEngine .world file and matches for certain objects defined in their, so they can be used within the world editor.
 */
class VEngineWorldParser 
{
public:
	/**
	 * Ctor.
	 */
	VEngineWorldParser(const QString& file);

public:
	/**
	 * Parses the file, as identified by the name/path given in the file parameter.
	 * 
	 * @return a list of all objects in the VEngine file with their name matching the pattern E_SomeName.
	 */
	const QStringList& ParseFile();

	/**
	 * Checks whether the file passed to this class instance is valid
	 */
	bool IsFileValid();

private:
	/**
	 * List containing all valid VEngine objects eligible for the entity system
	 */
	QStringList found;

	/**
	 * The file name.
	 */
	QString file;
};

#endif // VEngineWorldParser_h__