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
#ifndef Application_h__
#define Application_h__

#include <Aeon/CmdArgs.h>
#include <Aeon/Platform/Platform.h>

#include <Aeon/Subsystem/Export.h>

namespace Aeon {
	/**
	 * @brief Application class, should be derived from to initialize subsystems and set up the application.
	 */
	class AEON_SUBSYSTEM_EXPORT Application 
	{
	public:
		/**
		 * Constructor
		 */
		Application();

		/**
		 * Destructor.
		 */
		virtual ~Application();

	public:
		/**
		 * Indicates if the application is running.
		 */
		virtual bool IsRunning() = 0;

	public:
		/**
		 * Initializes the application.
		 */
		virtual void Initialise(CmdArgs& args) = 0;

		/**
		 * Prompts the application to create the window based on an external window handle.
		 * 
		 * On Windows, cast externalWindowHandle to HWND, on *NIX to long.
		 */
		virtual void CreateExternalRenderWindow(void* externalWindowHandle, int width, int height) = 0;
		
		/**
		 * Initializes the subsytems.
		 */
		virtual void InitialiseSubsystems() = 0;
		
		/**
		 * Register Components.
		 */
		virtual void RegisterComponents() = 0;
		
		/**
		 * Unregister Components.
		 */
		virtual void UnregisterComponents() = 0;

	public:
		/**
		 * Get the application name.
		 */
		virtual const char* GetApplicationName() = 0;

	public:
		/**
		 * Shuts down the game application.
		 */
		virtual void Shutdown(bool immediate) = 0;

	public:
		/**
		 * Called when the application needs to started, enters the game loop.
		 */
		virtual void Run() = 0;
	};
}

#endif // Application_h__
