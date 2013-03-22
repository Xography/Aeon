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
#ifndef __NullPlatformTools_h__
#define __NullPlatformTools_h__

#include <Aeon/Subsystem/IPlatformTools.h>

namespace Aeon {
	/**
	 *  @brief Null implementation of the platform tools subsystem.
	 */
	class AEON_SUBSYSTEM_EXPORT NullPlatformTools : public IPlatformTools
	{
		DECLARE_SUBSYSTEM(NullPlatformTools, "Null Platformtools")

	public:
		/**
		 * Overrides for ISubsystem functions.
		 *
		 * @see ISubsystem
		 */
		void Initialise(CmdArgs& /*args*/) override { }
		void Shutdown() override { }

	public:
		/**
		 * Overrides for IPlatformTools functions.
		 *
		 * @see IPlatformTools
		 */
		const WString& GetAppDataDirectory() override { static WString tmp; return tmp; }
		bool DeleteFile(const WString& /*file*/) override { return false; }
		bool CreateDirectory(const WString& /*directory*/) override { return false; }
		bool DeleteDirectory(const WString& /*directory*/) override { return false; }
		bool FileExists(const WString& /*filename*/) override { return false; }
		bool CopyFile(const WString& /*src*/, const WString& /*dest*/) override { return false; }
		String GetExecutablePath() override { return String(); }

	public:
		void OnMessage(Aeon::Message* message) override {}
	};
}
#endif // __NullPlatformTools_h__
