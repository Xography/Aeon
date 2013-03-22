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
#include <Aeon/Logging/Logging.h>
#include <Aeon/Subsystems/SubsystemRegistry.h>

#include <Aeon/Util/STLHelpers.h>
#include <Aeon/Platform/UnixPlatformTools.h>

#include <unistd.h>
#include <sys/stat.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	static inline String PidToString(__pid_t i)
	{
		std::ostringstream s;
		s << i;
		return s.str();
	}
	
	// -------------------------------------------------------------------------
	void UnixPlatformTools::Initialise(CmdArgs& args)
	{
		appDataPath = L"~/.Aeon";
	}

	// -------------------------------------------------------------------------
	void UnixPlatformTools::Shutdown()
	{

	}

	// -------------------------------------------------------------------------
	const WString& UnixPlatformTools::GetAppDataDirectory()
	{
		return appDataPath;
	}

	// -------------------------------------------------------------------------
	bool UnixPlatformTools::DeleteFile(const WString& file)
	{
		char* asciiString = new char[file.length()];
		wcstombs(asciiString, file.c_str(), file.length());
		
		if(unlink(asciiString))
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Failed to delete file.";
			delete asciiString;
			return false;
		}
		
		delete asciiString;

		return true;
	}

	// -------------------------------------------------------------------------
	bool UnixPlatformTools::CreateDirectory(const WString& directory)
	{
		char* asciiString = new char[directory.length()];
		wcstombs(asciiString, directory.c_str(), directory.length());
		
		if(system((String("mkdir -p ") + asciiString).c_str()))
		{
			LOG_WARNING << this->subsystemRTTI.Classname() << ": Failed to create directory.";
			delete asciiString;
			return false;
		}
		
		delete asciiString;

		return true;
	}

	// -------------------------------------------------------------------------
	bool UnixPlatformTools::DeleteDirectory(const WString& directory)
	{
		char* asciiString = new char[directory.length()];
		wcstombs(asciiString, directory.c_str(), directory.length());
		
		if(system((String("rmdir ") + asciiString).c_str()))
		{
			LOG_WARNING << this->subsystemRTTI.Classname() << ": Failed to remove directory.";
			delete asciiString;
			return false;
		}
		
		delete asciiString;

		return true;
	}

	// -------------------------------------------------------------------------
	bool UnixPlatformTools::FileExists(const WString& filename)
	{
		char* asciiString = new char[filename.length()];
		wcstombs(asciiString, filename.c_str(), filename.length());
		
		struct stat s;
		int result = stat(asciiString, &s);
		
		delete asciiString;
		
		return (result == 0);
	}

	// -------------------------------------------------------------------------
	bool UnixPlatformTools::CopyFile(const WString& src, const WString& dest)
	{
		// TODO: This may also be useful for the Win32 implementation, since it's portable.
		
		// Grab contents of source file.
		std::wifstream srcFile(ToString(src), std::ios::in | std::ios::binary);
		
		if(!srcFile.is_open() || srcFile.fail() || srcFile.bad())
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Failed to copy file, could not open source file!";
			return false;
		}
		
		size_t srcSize = 0;
		srcFile.seekg(0, std::ios::end);
		srcSize = srcFile.tellg();
		srcFile.seekg(0, std::ios::beg);
		
		// This may be a slightly too large buffer, tellg() may or may not return the filesize in bytes (it could also be in whcar_tS).
		wchar_t* data = new wchar_t[srcSize];
		srcFile.read(data, srcSize);
		srcFile.close();
		
		// Write contents to destination file.
		std::wofstream destFile(ToString(dest), std::ios::in | std::ios::binary);
		
		if(!destFile.is_open() || destFile.fail() || destFile.bad())
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Failed to copy file, could not open destination file!";
			return false;
		}
		
		destFile.write(data, srcSize);
		destFile.close();
		
		delete[] data;

		return true;
	}

	// -------------------------------------------------------------------------
	String UnixPlatformTools::GetExecutablePath()
	{
		// Some POSIX magic to get the absolute path to the current executable, don't even ask.
		FILE* procPipe = popen((String("readlink /proc/") + PidToString(getpid()) + String("/exe")).c_str(), "r");
		
		if(!procPipe)
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Couldn't open a pipe to read the absolute path to the executable!";
			return String();
		}
		
		// Read the output sent by the pipe.
		String path;
		char buffer[64];
		
		while(!feof(procPipe))
			if(fgets(buffer, sizeof(buffer), procPipe) != nullptr)
				path += buffer;
			
		pclose(procPipe);
		return path.substr(0, path.length() - 1); // Remove the trailing newline.
	}
}
