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
#include <Aeon/Platform/Win32PlatformTools.h>
#include <Aeon/Subsystem/SubsystemRegistry.h>

#include <Shlobj.h>

#ifdef _MSC_VER
#	include <SDKDDKVer.h> // Including SDKDDKVer.h defines the highest available Windows platform.
#endif

// *Very* strangely enough on VC11 the compiler suddenly starts seeing the below definition Win32PlatformTools::DeleteFile 
// as Win32PlatformTools::DeleteFileA (because it's a preprocessor define), it didn't do this in previous versions. We'll
// just have to use the direct variants of these functions instead (which we already did).
#undef CopyFile
#undef DeleteFile
#undef CreateDirectory

namespace Aeon {
	// -------------------------------------------------------------------------
	void Win32PlatformTools::Initialise(CmdArgs& args)
	{
		// TODO: Use SHGetFolderPath to support clients older than Vista.
		
		//wchar_t* roamingAppData = 0;
		//SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &roamingAppData);
		//paths[CSIDL_APPDATA] = WString(roamingAppData);
		//CoTaskMemFree(static_cast<void*>(roamingAppData));
	}

	// -------------------------------------------------------------------------
	void Win32PlatformTools::Shutdown()
	{

	}

	// -------------------------------------------------------------------------
	const WString& Win32PlatformTools::GetAppDataDirectory()
	{
		return paths[CSIDL_APPDATA];
	}

	// -------------------------------------------------------------------------
	bool Win32PlatformTools::DeleteFile(const WString& file)
	{
		if(!DeleteFileW(file.c_str()))
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Failed to delete file.";
			return false;
		}

		return true;
	}

	// -------------------------------------------------------------------------
	bool Win32PlatformTools::CreateDirectory(const WString& directory)
	{
		if(!CreateDirectoryW(directory.c_str(), nullptr))
		{
			LOG_WARNING << this->subsystemRTTI.Classname() << ": Failed to create directory.";
			return false;
		}

		return true;
	}

	// -------------------------------------------------------------------------
	bool Win32PlatformTools::DeleteDirectory(const WString& directory)
	{
		if(!RemoveDirectoryW(directory.c_str()))
		{
			LOG_WARNING << this->subsystemRTTI.Classname() << ": Failed to remove directory.";
			return false;
		}

		return true;
	}

	// -------------------------------------------------------------------------
	bool Win32PlatformTools::FileExists(const WString& filename)
	{
		return (GetFileAttributesW(filename.c_str()) != 0xFFFFFFFF);
	}

	// -------------------------------------------------------------------------
	bool Win32PlatformTools::CopyFile(const WString& src, const WString& dest )
	{
		if(!CopyFileW(src.c_str(), dest.c_str(), FALSE))
		{
			LOG_WARNING << subsystemRTTI.Classname() << ": Failed to copy file.";
			return false;
		}

		return true;
	}

	// -------------------------------------------------------------------------
	String Win32PlatformTools::GetExecutablePath()
	{
		LPSTR name = new TCHAR[MAX_PATH];

		GetModuleFileNameA(nullptr, name, MAX_PATH);

		if(GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			LOG_ERROR << "The executable path is too long to retrieve, there may be too many subfolders to reach the executable!";

		String ret = name;
		delete[] name;
		return ret;
	}

	// -------------------------------------------------------------------------
	void Win32PlatformTools::OnMessage(Aeon::Message* message)
	{

	}

}
