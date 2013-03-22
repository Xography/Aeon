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
#ifndef ModuleManager_h__
#define ModuleManager_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Export.h>
#include <Aeon/Singleton.h>
#include <Aeon/Modules/IModule.h>

namespace Aeon {
	/**
	 * Manages dynamic Aeon modules.
	 */
	class AEON_EXPORT ModuleManager : public Aeon::Singleton<Aeon::ModuleManager>
	{
		friend class Singleton<ModuleManager>;
	protected:
		/**
		 * Ctor.
		 */
		ModuleManager();
	public:
		
		/**
		 * Dtor.
		 */
		~ModuleManager();
	
	public:
		/**
		 * Attempts to load the given module into memory.
		 * @note This will also call the init functions of the module.
		 * @see IModule.
		 */
		void LoadModule(Aeon::String& moduleName);

		/**
		 * Releases the module from memory.
		 * @note This will also call the shutdown functions of the module.
		 * @see IModule.
		 */
		void ReleaseModule(Aeon::String& moduleName);

		/**
		 * Releases all modules from memory.
		 */
		void ReleaseAll();
	
	public:
		/**
		 * Returns to module handle for low level access.
		 */
		IModule* GetDynLibHandle(Aeon::String& moduleName);
	
	private:
		
		/**
		 * Pair of the module dll and the module instance.
		 */
		struct ModuleHandle
		{
			String moduleName;
			void* dllHandle;
			IModule* instance;
		};

	private:
		/**
		 * Internal helper function.
		 */
		void FreeHandle(const ModuleHandle& handle);

	private:
		/**
		 * List of loaded modules.
		 */
		Vector<ModuleHandle>::type modules; 
	};
}

#endif // ModuleManager_h__