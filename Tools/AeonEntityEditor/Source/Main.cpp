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
#include <Aeon/CmdArgs.h>
#include <Aeon/Logging/Logging.h>
#include <QCleanlooksStyle>

#include "MainWindow.h"

#ifdef _MSC_VER
#	ifdef _DEBUG
#		pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#	else
// This is so we don't need WinMain for release builds (or when /SUBSYSTEM:Windows is set).
#		pragma comment(linker, "/ENTRY:\"mainCRTStartup\"")
#	endif // _DEBUG
#endif // _MSC_VER

// -------------------------------------------------------------------------
// ----- Entry point.
int main(int argc, char** argv)
{
	// Create Logging system upfront.
	Aeon::Logging::Create(WSTR("AeonEditor.log"));

	LOG_INFO << "Initialising Aeon Editor.";

	QApplication application(argc, argv);
	//application.setStyle(new QCleanlooksStyle());
	MainWindow w;
	w.show();
	return application.exec();
}
