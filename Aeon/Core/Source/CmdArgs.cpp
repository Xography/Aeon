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
#include <Aeon/CmdArgs.h>
#include <Aeon/Logging/Logging.h>
#include <Aeon/Platform/Platform.h>

#include <regex>
#include <cstring>

#ifdef AEON_WINDOWS
#include <windows.h>
#else
#endif
namespace Aeon {
	// -------------------------------------------------------------------------
	std::string GetExecPath()
	{
#ifdef AEON_WINDOWS
		char path[MAX_PATH + 1];
		GetModuleFileName( NULL, path, MAX_PATH + 1 );
		return path;
#else
#endif
	}
	// -------------------------------------------------------------------------
	CmdArgs::~CmdArgs()
	{
		
	}

	// -------------------------------------------------------------------------
	bool CmdArgs::Has(const String& argKey)
	{
		return args.count(argKey) == 1;
	}

	// -------------------------------------------------------------------------
	const String& CmdArgs::Get(const String& argKey)
	{
		return args[argKey];
	}

	// -------------------------------------------------------------------------
	CmdArgs::CmdArgs(int argc, char** argv)
	{
		if(argc == 0) 
			return;

		this->rawArgs.argc = argc;
		this->rawArgs.argv = argv;

		// Extract executable path and name.
		std::string exePath =  GetExecPath();
		
		size_t index = exePath.find_last_of('/');

		if(index == String::npos) // Try Windows separators last (since Windows reserves the / separator, but *NIX systems don't reserve the \ separator).
			index = exePath.find_last_of('\\');

		assert(index != String::npos); // This should never happen because the platform tools should return an absolute path.

		if(index != String::npos)
		{
			this->executablePath = exePath.substr(0, index);
			this->executableName = exePath.substr(index + 1);
		}

		// Matches: --key=val
		for(int i = 1; i < argc; i++)
		{
			if(strlen(argv[i]) <= 2 && argv[i][0] != '-' && argv[i][1] != '-')
				continue;

			String src(argv[i]);

			if(src.find_first_of("=") == String::npos)
				continue;

			args[src.substr(2, src.find_first_of("=") - 2)] = src.substr(src.find_first_of("=") + 1, src.length() - 1);
		}
	}

	// -------------------------------------------------------------------------
	const CmdArgs::RawArgs& CmdArgs::GetRawArguments()
	{
		return rawArgs;
	}

	// -------------------------------------------------------------------------
	const String& CmdArgs::GetExecutableName()
	{
		return executableName;
	}

	// -------------------------------------------------------------------------
	const String& CmdArgs::GetExecutablePath()
	{
		return executablePath;
	}

	// -------------------------------------------------------------------------
	CmdArgs* Aeon::CmdArgs::Create(int argc, char** argv)
	{
		return new CmdArgs(argc, argv);
	}

	// -------------------------------------------------------------------------
	void CmdArgs::Destroy(CmdArgs* ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}
