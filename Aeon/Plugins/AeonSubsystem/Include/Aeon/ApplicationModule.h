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
#ifndef ApplicationModule_h__
#define ApplicationModule_h__

#include <Aeon/Subsystem/Export.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/Platform/Platform.h>
#include <Aeon/Application.h>

#if AEON_WINDOWS
#else
#	include <dlfcn.h>	
#endif

#if AEON_WINDOWS
#	define APPLICATION_HANDLE ::HMODULE;
#else
#	define APPLICATION_HANDLE void*;
#endif

#if AEON_WINDOWS
#	define	APP_INIT_FUNCT	extern "C" Application* CreateAeonApplication ();
#	define APP_CLEANUP_FUNCT	extern "C" void ReleaseAeonApplication (Application* module);
#	ifndef AeonSubsystem_EXPORTS
#		define BEGIN_APPLICATION_PLUGIN extern "C" { 
#		define INIT_APPLICATION_PLUGIN	__declspec(dllexport) Application* CreateAeonApplication()
#		define DESTROY_APPLICATION_PLUGIN	__declspec(dllexport) void ReleaseAeonApplication(Application* module)
#		define END_APPLICATION_PLUGIN }
#	endif
#else
	// TODO
#endif

#endif // ApplicationModule_h__