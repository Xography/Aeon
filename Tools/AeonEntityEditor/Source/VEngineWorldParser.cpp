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
 #include "VEngineWorldParser.h"

 // -------------------------------------------------------------------------
VEngineWorldParser::VEngineWorldParser(const QString& file) :
	file(file)
{

}

// -------------------------------------------------------------------------
bool VEngineWorldParser::IsFileValid()
{
	QFile inputFile(file);

	// File has to exist
	if(!inputFile.exists()) return false;

	// We need read only file access
	if (!inputFile.open(QIODevice::ReadOnly)) return false;		

	bool valid = false;

	QTextStream in(&inputFile);

	// We want to parse the second line
	QString line = in.readLine();
	line = in.readLine();

	// Verify that it is indeed a vengine world file
	valid = line.contains("V Engine world file");
	line = in.readLine();

	// Maybe even require a specific engine version later on?
	valid = line.contains("Engine:");
	inputFile.close();

	return valid;
}

// -------------------------------------------------------------------------
const QStringList& VEngineWorldParser::ParseFile()
{
	QFile inputFile(file);
	found.clear();

	if (inputFile.open(QIODevice::ReadOnly))
	{
		QTextStream in(&inputFile);

		while ( !in.atEnd() )
		{
			QString line = in.readLine();

			// We want to match the following pattern
			// <mesh|trigger>.setName("E_<any>".str());
			if(line.contains("mesh.setName(\"E_")
				|| line.contains("trigger.setName(\"E_"))
			{
				found.push_back(line.split("\"").at(1));
			}

		}
	}

	inputFile.close();

	return found;
}
