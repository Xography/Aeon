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
#include <Aeon/Modules/ModuleManager.h>

#include <algorithm>

#include <Aeon/Logging/Logging.h>

#ifdef AEON_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace Aeon {
	typedef IModule* (*MODULE_CREATE)();
	typedef void (*MODULE_CLEANUP)(IModule*);

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
	ModuleManager::ModuleManager()
	{

	}

	// -------------------------------------------------------------------------
	ModuleManager::~ModuleManager()
	{

	}

	// -------------------------------------------------------------------------
	void ModuleManager::LoadModule(Aeon::String& moduleName)
	{
		ModuleHandle handle = {"",0,0};

		handle.dllHandle = LOAD_LIBRARY(moduleName.append(PLATFORM_EXT).c_str());

		if(handle.dllHandle == nullptr)
		{
			LOG_FATAL << "Unable to load module " << moduleName;
			return;
		}

		MODULE_CREATE initializer = (MODULE_CREATE)PROC_ADDRESS(handle.dllHandle, "CreateAeonModule");

		if(initializer == NULL) 
		{ 
			LOG_FATAL << "Unable to locate module entry point \"CreateAeonModule\" in " << moduleName; 
			return; 
		}  
		else 
		{  
			handle.instance = (*initializer)(); 
			LOG_INFO << "Loaded code from module: " << moduleName; 
		}

		handle.moduleName = moduleName;

		IModule* module = handle.instance;
		

		LOG_INFO << "Initializing module: " << moduleName; 

		module->OnInitialise();

		LOG_INFO << moduleName << ": Creating messaging channels..."; 
		module->CreateMessageChannels();

		LOG_INFO << moduleName << ": Creating buckets..."; 
		module->CreateBuckets();		

		LOG_INFO << moduleName << ": Creating component template instances..."; 
		module->CreateComponents();

		LOG_INFO << "Done."; 

		modules.push_back(handle);
	}

	// -------------------------------------------------------------------------
	void ModuleManager::ReleaseModule(Aeon::String& moduleName)
	{
		auto it = std::find_if(modules.begin(), modules.end(), [&moduleName](const ModuleHandle& data ){ return data.moduleName == moduleName; });

		if(it == modules.end())
			return;
				
		ModuleHandle handle = (*it);
	
		modules[(it - modules.begin())] = std::move(modules.back());
		modules.pop_back();

		FreeHandle(handle);
	}

	// -------------------------------------------------------------------------
	void ModuleManager::ReleaseAll()
	{
		LOG_INFO << "Freeing all modules...";

		auto it = modules.begin();
		while( it != modules.end())
		{
			ModuleHandle handle = *it;
			it = modules.erase(it);

			FreeHandle(handle);
		}
	}

	// -------------------------------------------------------------------------
	IModule* ModuleManager::GetDynLibHandle(Aeon::String& moduleName)
	{
		auto it = std::find_if(modules.begin(), modules.end(), [&moduleName](const ModuleHandle& data ){ return data.moduleName == moduleName; });

		if(it == modules.end())
		{
			LOG_WARNING << "Requested module " << moduleName << " not loaded/found. Returning nullptr.";
			return nullptr;
		}

		return (*it).instance;
	}

	// -------------------------------------------------------------------------
	void ModuleManager::FreeHandle(const ModuleHandle& handle)
	{
		LOG_INFO << "Releasing module: " << handle.moduleName; 

		LOG_INFO << handle.moduleName << ": Destroying component template instances..."; 
		handle.instance->DestroyComponents();

		LOG_INFO << handle.moduleName << ": Destroying buckets..."; 
		handle.instance->DestroyBuckets();		

		LOG_INFO << handle.moduleName << ": Destroying messaging channels..."; 
		handle.instance->DestroyMessageChannels();

		handle.instance->OnShutdown();

		LOG_INFO << "Done."; 

		MODULE_CLEANUP funct = (MODULE_CLEANUP)PROC_ADDRESS(handle.dllHandle, "ReleaseAeonModule");
		(*funct)(handle.instance);

		FREE_LIBRARY(handle.instance);
	}

}