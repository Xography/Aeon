/*******************************************************************************
 *
 * This file is part of the Aeon Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#include <iostream>
#include <Aeon/Logging/Logging.h>

#include <igloo/igloo.h>
using namespace igloo;
using namespace Aeon;

int main(void)
{
	Logging::Create(WSTR("Aeon_Tests.log"));
	VisualStudioResultsOutput output;
	TestRunner runner(output);
	LOG_INFO << "Running all tests.";
	return runner.RunAllTests();
}
