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
#ifndef SubsystemModule_h__
#define SubsystemModule_h__

#include <Aeon/Subsystem/Export.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/Platform/Platform.h>
#include <Aeon/Subsystem/ISubsystem.h>

#if AEON_WINDOWS

#else
#	include <dlfcn.h>	
#endif

#if AEON_WINDOWS
#	define SUBSYS_HANDLE ::HMODULE;
#else
#	define SUBSYS_HANDLE void*;
#endif

#if AEON_WINDOWS
#	define	SUBSYS_INFO_FUNCT	extern "C" SubsystemInfo AboutSubsystem();
#	define	SUBSYS_INIT_FUNCT	extern "C" ISubsystem* CreateAeonSubsystem ();
#	define SUBSYS_CLEANUP_FUNCT	extern "C" void DestroyAeonSubsystem(ISubsystem* subsystem);
#	ifndef AeonSubsystem_EXPORTS
#		define BEGIN_SUBSYSTEM_MODULE extern "C" { 
#		define INIT_SUBSYSTEM_MODULE	__declspec(dllexport) ISubsystem* CreateAeonSubsystem()
#		define SUBSYSTEM_MODULE_INFO	__declspec(dllexport) SubsystemInfo AboutSubsystem()
#		define DESTROY_SUBSYSTEM_MODULE	__declspec(dllexport) void DestroyAeonSubsystem(ISubsystem* subsystem)
#		define END_SUBSYSTEM_MODULE }
#	endif
#else
	// TODO
#endif

#endif // SubsystemModule_h__