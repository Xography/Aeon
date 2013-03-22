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
#include <typeinfo>

#include <Aeon/AeonTypes.h>

#include <Aeon/Subsystem/SubsystemRegistry.h>

#include <Aeon/Application.h>
#include <Aeon/CmdArgs.h>
#include <Aeon/Logging/NullLogging.h>
#include <Aeon/Subsystem/Impl/NullInput.h>
#include <Aeon/Subsystem/Impl/NullAudio.h>
#include <Aeon/Subsystem/Impl/NullRenderer.h>
#include <Aeon/Subsystem/Impl/NullPlatformTools.h>
#include <Aeon/Subsystem/Impl/NullUserInterface.h>

#ifdef AEON_WINDOWS
#include <Aeon/Platform/Win32PlatformTools.h>
	#include <Windows.h>
#else
	#include <Aeon/Platform/UnixPlatformTools.h>
#endif // WIN32

#include <Aeon/ApplicationModule.h>
#include <Aeon/SubsystemModule.h>

namespace Aeon {

	void* SubsystemRegistry::appHandle = 0;
	Application* SubsystemRegistry::application = 0;
	CmdArgs* SubsystemRegistry::args = 0;

	typedef Application* (*APP_FACTORY)();
	typedef void (*APP_CLEANUP)(Application*);


	typedef ISubsystem* (*SUBSYS_FACTORY)();
	typedef SubsystemInfo (*SUBSYS_INFO)();
	typedef void (*SUBSYS_CLEANUP)(ISubsystem*);
	
	#ifdef AEON_WINDOWS
	#	define LOAD_LIBRARY(file__) (void*)::LoadLibrary(TEXT(file__))
	#	define PROC_ADDRESS(handle__, proc__) ::GetProcAddress((::HMODULE)handle__, proc__)
	#	define FREE_LIBRARY(handle__) 		::FreeLibrary((::HMODULE)handle__)
	#	define PLATFORM_EXT ".dll"
	#else
	#	define LOAD_LIBRARY(file__) dlopen(file__, RTLD_LAZY)
	#	define PROC_ADDRESS(handle__, proc__) dlsym(handle__, proc__)
	#	define FREE_LIBRARY(handle__) dlclose(handle__)
	#	define PLATFORM_EXT ".so"
	#endif

	// -------------------------------------------------------------------------
	static inline Aeon::ISubsystem* GetMatchingPlatformSubsystem()
	{
		return
		#ifdef AEON_WINDOWS
			new Aeon::Win32PlatformTools()
		#else
			new Aeon::UnixPlatformTools()
		#endif
			;
	}

	// -------------------------------------------------------------------------
	ISubsystem* SubsystemRegistry::subsystems[5] =
	{
		GetMatchingPlatformSubsystem(),
		new NullAudio(),
		new NullRenderer(),
		new NullInput(),
		new NullUserInterface()
	};
	
	// -------------------------------------------------------------------------
	void SubsystemRegistry::Initialise(CmdArgs* args)
	{
		assert(args);

		SubsystemRegistry::args = args;

		LOG_INFO << "Subsystem Registry initializsed.";
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::LoadSubsystemModule(const char* module_file)
	{
		void* handle = 0;

		handle = LOAD_LIBRARY(String(module_file).append(PLATFORM_EXT).c_str());		

		if(!handle) return;

		SubsystemInfo info;

		SUBSYS_INFO funct = (SUBSYS_INFO)::GetProcAddress((::HMODULE)handle, "AboutSubsystem");

		if(!funct)
		{
			LOG_FATAL << "Unable to call AboutSubsystem in loaded module " <<  module_file;
			FREE_LIBRARY(handle);
			return;
		}

		info = (*funct)();

		ISubsystem* ptr = 0;
		SUBSYS_FACTORY initializer = (SUBSYS_FACTORY)PROC_ADDRESS(handle, "CreateAeonSubsystem");
		
		if(!initializer)
		{
			LOG_FATAL << "Unable call CreateAeonSubsystem in loaded module " << module_file;
			FREE_LIBRARY(handle);
			return;
		}

		ptr = (*initializer)();

		if(!ptr) 
			return;

		SubsystemRegistry::RegisterSubsystem(ptr);
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::RegisterSubsystem(ISubsystem* service)
	{
		if(!service)
		{
			LOG_WARNING << "Received Null Subsystem!";
			return;
		}

		SubsystemType type = service->GetSubsystemType();

		// In case we have run time type information (RTTI) enabled, print verbose information. Intended for debug builds.
#ifdef _CPPRTTI
		try
		{
			String type_id = typeid(*service).name();
			LOG << "Registered Subsystem: " << service->GetSubsystemName().Classname();
			LOG_DEBUG << "Registered Subsystem Type: " << (type_id.substr(6,type_id.length()));
		}		
		catch(std::bad_typeid &)
		{
			LOG << "Registered Subsystem: " << service->GetSubsystemName().Classname();
			LOG_DEBUG << "Registered Subsystem Type: " << GetSubsystemTypeName(type);
		}
#else
		LOG << "Registered Subsystem: " << service->GetSubsystemName().Classname();
#endif // _CPPRTTI

		if (subsystems[int(type) - 1])
			UnregisterSubsystem(subsystems[int(type) - 1]); // Delete previous subsystem.

		LOG_DEBUG << "Attempting to initialise Subsystem " << service->GetSubsystemName().Classname();

		subsystems[int(type) - 1] = service;

		service->Initialise(*args);
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::UnregisterSubsystem(ISubsystem* service)
	{
		if(!service)
		{
			LOG_WARNING << "Received Null Subsystem!";
			return;
		}

		SubsystemType type = service->GetSubsystemType();
		LOG << "Unregistering Subsystem: " << service->GetSubsystemName().Classname();

		service->Shutdown();
		delete service;

		// This subsystem won't be replaced with the null subsystem! If the user wants that, he'll have to register it. Usually this function is only called
		// on shutdown, so allocating a new null subsystem is useless.
		subsystems[int(type) - 1] = nullptr;
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::UnregisterSubsystem(SubsystemType type)
	{
		UnregisterSubsystem (subsystems[int(type) - 1]);
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::DestroyAllSubsystems()
	{
		unsigned i(0);
		const unsigned count = (sizeof(subsystems) / sizeof(ISubsystem*));

		while(i < count)
		{
			UnregisterSubsystem(subsystems[i]);
			subsystems[i++] = nullptr;
		}
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::RegisterApplicationModule(Application* module)
	{
		SubsystemRegistry::application = module;
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::LoadApplicationModule(const char* module_file)
	{	
		SubsystemRegistry::appHandle = LOAD_LIBRARY(String(module_file).append(PLATFORM_EXT).c_str());

		if(SubsystemRegistry::appHandle == NULL)
		{
			LOG_FATAL << "Unable to load application module " << module_file;
			return;
		}

		APP_FACTORY initializer = (APP_FACTORY)PROC_ADDRESS(SubsystemRegistry::appHandle, "CreateAeonApplication");

		if(initializer == NULL) 
		{ 
			LOG_FATAL << "Unable to locate application entry point \"CreateAeonApplication\" in " << module_file; 
			return; 
		}  
		else 
		{  
			SubsystemRegistry::application = (*initializer)(); 
			LOG_INFO << "Loaded application code from module: " << module_file; 
		} 
	}

	// -------------------------------------------------------------------------
	Application* SubsystemRegistry::GetApplicationModule()
	{
		return application;
	}

	// -------------------------------------------------------------------------
	void SubsystemRegistry::UnloadApplicationModule()
	{
		if(application->IsRunning())
			application->Shutdown(true);

		APP_CLEANUP funct = (APP_CLEANUP)PROC_ADDRESS(SubsystemRegistry::appHandle, "ReleaseAeonApplication");
		(*funct)(SubsystemRegistry::application);

		FREE_LIBRARY(SubsystemRegistry::appHandle);

	}

	// -------------------------------------------------------------------------
	CmdArgs* SubsystemRegistry::GetArgs()
	{
		return SubsystemRegistry::args;
	}

	// -------------------------------------------------------------------------
	Aeon::SubsystemInfo SubsystemRegistry::SubsystemModuleInfo(const char* module_file)
	{
		void* handle = 0;
		SubsystemInfo info = {0,0,SUBSYS_CUSTOM};

		handle = LOAD_LIBRARY(module_file);		

		if(!handle) 
			return info;

		SUBSYS_INFO funct = (SUBSYS_INFO)PROC_ADDRESS(handle, "AboutSubsystem");

		if(!funct)
		{
			LOG_FATAL << "Unable to call AboutSubsystem() in loaded module " <<  module_file;
			FREE_LIBRARY(handle);
			return info;
		}

		info = (*funct)();

		FREE_LIBRARY(handle);

		return info;
	}

}
