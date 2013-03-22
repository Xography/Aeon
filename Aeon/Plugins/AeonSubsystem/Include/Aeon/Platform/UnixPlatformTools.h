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
#ifndef UnixPlatformTools_h__
#define UnixPlatformTools_h__

#include <Aeon/Subsystem/IPlatformTools.h>

namespace Aeon {
	/**
	 * @brief Platform tools implementation for the Windows platform.
	 */
	class AEON_SUBSYSTEM_EXPORT UnixPlatformTools : public IPlatformTools
	{
		DECLARE_SUBSYSTEM(UnixPlatformTools, "UNIX Platform")

	public:
		/**
		 * Subsystem overrides.
		 *
		 * @see ISubsystem
		 */
		void Initialise(CmdArgs& args) override;
		void Shutdown() override;

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
		 * AppData path.
		 */
		WString appDataPath;
	};
}

#endif // UnixPlatformTools_h__
