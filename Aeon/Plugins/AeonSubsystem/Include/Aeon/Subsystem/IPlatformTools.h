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
#ifndef __IPlatformTools_h__
#define __IPlatformTools_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Subsystem/ISubsystem.h>

namespace Aeon {
	/**
	 * @brief Interface for platform tools subsystems. Platform tools are system-dependant, and should only have system-specific code compile when it is detected
	 * (through preprocessor checks).
	 */
	class AEON_SUBSYSTEM_EXPORT IPlatformTools : public ISubsystem
	{
	public:
		/**
		 * Returns the subsystem type.
		 *
		 * @see ISubsystem
		 */
		SubsystemType GetSubsystemType() const override
		{
			return SUBSYS_PFT;
		}
		
	public:
		/**
		 * Returns the directory where application-specific data is saved.
		 */
		virtual const WString& GetAppDataDirectory() = 0;

		/**
		 * Deletes the specified file and returns whether the action succeeded.
		 */
		virtual bool DeleteFile(const WString& file) = 0;

		/**
		 * Creates a new directory at the specified location and returns whether the action succeeded.
		 */
		virtual bool CreateDirectory(const WString& directory) = 0;

		/**
		 * Deletes the specified directory and returns whether the action succeeded.
		 */
		virtual bool DeleteDirectory(const WString& directory) = 0;

		/**
		 * Checks if the specified file exists.
		 */
		virtual bool FileExists(const WString& filename) = 0;

		/**
		 * Copies a file from one location to another and returns whether the action succeeded.
		 */
		virtual bool CopyFile(const WString& src, const WString& dest) = 0;

		/**
		 * Returns the absolute path to the executable.
		 */
		virtual String GetExecutablePath() = 0;
	};
}
#endif // __IPlatformTools_h__
