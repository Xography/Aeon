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
#ifndef Win32PlatformTools_h__
#define Win32PlatformTools_h__

#include <map>

#include <Aeon/Subsystem/IPlatformTools.h>

namespace Aeon {
	/**
	 * @brief Platform tools implementation for the Windows platform.
	 */
	class AEON_SUBSYSTEM_EXPORT Win32PlatformTools : public IPlatformTools
	{
		DECLARE_SUBSYSTEM(Win32PlatformTools, "Windows Platform")

	public:
		/**
		 * Subsystem overrides.
		 *
		 * @see ISubsystem
		 */
		void Initialise(CmdArgs& args) override;
		void Shutdown() override;
	
	public:
		void OnMessage(Aeon::Message* message) override;

	public:
		/**
		 * Platform tools overrides.
		 *
		 * @see IPlatformTools
		 */
		const WString& GetAppDataDirectory() override;
		bool DeleteFile(const WString& file) override;
		bool CreateDirectory(const WString& directory) override;
		bool DeleteDirectory(const WString& directory) override;
		bool FileExists(const WString& filename) override;
		bool CopyFile(const WString& src, const WString& dest) override;
		String GetExecutablePath() override;

	private:
		/**
		 * Map to store some path constants.
		 */
		std::map<const int, WString> paths;
	};
}

#endif // Win32PlatformTools_h__
