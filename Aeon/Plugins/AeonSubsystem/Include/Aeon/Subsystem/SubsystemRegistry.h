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
#ifndef SubsystemRegistry_h__
#define SubsystemRegistry_h__

#include <Aeon/CmdArgs.h>
#include <Aeon/TimeProvider.h>
#include <Aeon/Platform/Platform.h>
#include <Aeon/Logging/ILogging.h>

#include <Aeon/Subsystem/IInput.h>
#include <Aeon/Subsystem/IAudio.h>
#include <Aeon/Subsystem/IRenderer.h>
#include <Aeon/Subsystem/IPlatformTools.h>
#include <Aeon/Subsystem/IUserInterface.h>
#include <Aeon/Application.h>

namespace Aeon {
	/**
	 * @brief Register that keeps track of all the potentially available subsystems. If a subsystem is not explicitly registered by the application, a null
	 * variant will be used. This class is entirely static.
	 */
	class AEON_SUBSYSTEM_EXPORT SubsystemRegistry
	{
	public:
		/**
		 * Retrieves the subsystem of the specified type and immediately casts it to the template parameter type.
		 */
		template<typename T> static T GetSubsystem(SubsystemType type)
		{
			// SUBSYS_PFT is index 0, SUBSYS_LOG is index 1, ...
			return reinterpret_cast<T>( subsystems[int( type ) - 1] );
		}

		/**
		 * Retrieves the subsystem of the specified type.
		 */
		static ISubsystem* GetSubsystem(SubsystemType type)
		{
			return subsystems[int( type ) - 1];
		}

		/**
		 * Retrieves the time provider.
		 */
		static TimeProvider* GetTimeProvider();

	public:
		/**
		 * 
		 */
		static void Initialise(CmdArgs* args);

		/**
		 * 
		 */
		static CmdArgs* GetArgs();

	public:
		/**
		 * Registers the application module.
		 */
		static void RegisterApplicationModule(Application* module);
		
		/**
		 * Registers an application module from the specified file.
		 */
		static void LoadApplicationModule(const char* module_file);

		/**
		 * Returns the loaded application module, or 0.
		 */
		static Application* GetApplicationModule();

		/**
		 * Unloads the application module;
		 */
		static void UnloadApplicationModule();

	public:
		
		/**
		 * Loads a subsystem module from the specified file.
		 */
		static void LoadSubsystemModule(const char* module_file);

		/**
		 * Reads the subsystem information from the given module file.
		 */
		static SubsystemInfo SubsystemModuleInfo(const char* module_file);

		/**
		 * Registers the specified subsystem with the registry. The existing subsystem, if any, is automatically unregistered.
		 */
		static void RegisterSubsystem(ISubsystem* service);

		/**
		 * Unregisters the specified subsystem. The unregistered subsystem isn't implicitely replaced with a null subsystem.
		 */
		static void UnregisterSubsystem(ISubsystem* service);

		/**
		 * Unregisters the subsystem of the specified type. This is simply a wrapper for the previous function and behaves in the same way.
		 */
		static void UnregisterSubsystem(SubsystemType type);

		/**
		 * Unregisters, shuts down and destroys all the present subsystems.
		 */
		static void DestroyAllSubsystems();

	private:
		/**
		 * Array of subsystems.
		 */
		static ISubsystem* subsystems[5];
		static Application* application;
		static void* appHandle;

		/**
		 * Time provider.
		 */
		static TimeProvider* time;

		/**
		 * A pointer to the command line arguments passed to the application.
		 */
		static CmdArgs* args;
   };
}

#endif // SubsystemRegistry_h__
