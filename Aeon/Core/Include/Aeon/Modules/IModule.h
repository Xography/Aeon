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
#ifndef IModule_h__
#define IModule_h__

#include <Aeon/Export.h>
#include <Aeon/AeonTypes.h>
#include <Aeon/Platform/Platform.h>
#include <Aeon/Modules/IModule.h>

#if AEON_WINDOWS

#else
#	include <dlfcn.h>	
#endif

namespace Aeon {

#if AEON_WINDOWS
#	define AEON_MODULE_HANDLE ::HMODULE;
#else
#	define AEON_MODULE_HANDLE void*;
#endif

#if AEON_WINDOWS
#	define AEON_HANDLE_MODULE_CREATE	extern "C" IModule* CreateAeonModule ();
#	define AEON_HANDLE_MODULE_RELEASE	extern "C" void ReleaseAeonModule (IModule* module);
#	ifndef Aeon_EXPORTS
#		define BEGIN_AEON_MODULE extern "C" { 
#		define AEON_MODULE_CREATE	__declspec(dllexport) IModule* CreateAeonModule()
#		define AEON_MODULE_RELEASE	__declspec(dllexport) void ReleaseAeonModule(IModule* module)
#		define END_AEON_MODULE }
#	endif
#else
// TODO
#endif

	/**
	 * @brief Pure virtual interface class to implement dynamically loadable code modules for Aeon.
	 *
	 * The initialization order is:
	 *  - Create Message Channels
	 *  - Create Buckets
	 *  - Create Component template instances
	 */
	class IModule
	{
	public:
		virtual void OnInitialise() = 0;
		virtual void OnShutdown() = 0;
	public:
		/**
		 * Called first, the module should initialize the messaging channels it is going to use.
		 */
		virtual void CreateMessageChannels(/*const MessageQueue& context*/) = 0;
	
		/**
		 * Called after setting up channels, allowing the module to set up an arbitrary amount of buckets.
		 */
		virtual void CreateBuckets(/*const BucketManager& context*/) = 0;

		/**
		 * This function allows the module to initialize and register the component template classes.
		 */
		virtual void CreateComponents(/*const ComponentRegistry& context*/) = 0;

	public:
		/**
		 * Requests to destroy the components belonging to this module.
		 */
		virtual void DestroyComponents(/*const ComponentRegistry& context*/) = 0;

		/**
		 * Requests to destroy buckets belonging to this module.
		 */
		virtual void DestroyBuckets(/*const BucketManager& context*/) = 0;

		/**
		 *  Requests to destroy messaging channels belonging to this module.
		 */
		virtual void DestroyMessageChannels() = 0;
	};
}

#endif // IModule_h__
