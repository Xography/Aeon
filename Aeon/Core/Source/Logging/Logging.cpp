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
#include <Aeon/Platform/Platform.h> // Contains platform defines, needs to be put first.

#include <Aeon/Logging/Logging.h>
#include <Aeon/Logging/NullLogging.h>

#ifdef AEON_WINDOWS
#	include <Windows.h>
#endif // AEON_WINDOWS

// TODO: This class has certain code smells.
// The proxy stuff was quickly refactored into it in order not to break the rest of the code, but it should probably be put into
// a seperate class to create a FileLogger and a ProxyLogger instead of this joint class.

namespace Aeon {

	ILogging* Logging::instance = new NullLogging();

	// -------------------------------------------------------------------------
	Logging::Logging() : 
		proxy(nullptr)
	{		
		level = 
		#if _DEBUG 
			LOGSEV_DEBUG
		#else
			LOGSEV_INFO
		#endif
			;
	}

	// -------------------------------------------------------------------------
	Logging::~Logging()
	{
		Shutdown();
	}

	// -------------------------------------------------------------------------
	void Logging::Init()
	{
		if(proxy)
			return;

		if(file.empty())
			file = L"Aeon.log";

		// Truncate the log file.
#ifdef _MSC_VER
		fileStream.open(file, std::ios::out | std::ios::trunc);
#elif defined(__GNUC__)
		// I have a hunch that one of the two C++ libs is not conformant; wofstream on UNIX takes a wide string for everything but open?
		char* tmp = new char[file.length()];
		wcstombs(tmp, file.c_str(), file.length());
		fileStream.open(tmp, std::ios::out | std::ios::trunc);
#endif // AEON_WINDOWS

		if(!fileStream.is_open() || fileStream.bad() || fileStream.fail())
			LOG_WARNING << "Couldn't open " << fileStream << " for logging, messages will not be persisted!";
	}

	// -------------------------------------------------------------------------
	void Logging::Shutdown()
	{
		fileStream.close();
	}

	// -------------------------------------------------------------------------
	void Logging::SetLogLevel(int level)
	{
		this->level = level;
	}

	// -------------------------------------------------------------------------
	void Logging::LogMessage(const String& msg, int severity)
	{
		if(severity < level)
			return;

		if(proxy)
		{
			proxy->OnMessageLogged(msg, severity);
			return;
		}

		writeLock.lock();

		if(toConsole)
		{
			std::cout << msg;

			#ifdef AEON_WINDOWS
			OutputDebugStringA(msg.c_str());
			#endif // AEON_WINDOWS
		}

		// Log the message in the log file.
		if(fileStream.is_open() && !fileStream.fail() && !fileStream.bad())
		{
			// Convert ASCII to Unicode and write to file.
			Aeon::WString wmsg;
			wmsg.assign(msg.begin(), msg.end());

			fileStream << wmsg;
			fileStream.flush();
		}

		writeLock.unlock();
	}

	// -------------------------------------------------------------------------
	void Logging::Create(const WString& file)
	{
		if(Logging::instance)
			Logging::Destroy(instance);

		Logging* ptr = new Logging();
		ptr->file = file;
		ptr->Init();

		Logging::instance = ptr;
	}

	// -------------------------------------------------------------------------
	void Logging::Create(ILogProxy* target)
	{	
		if(Logging::instance)
			Logging::Destroy(instance);

		Logging* ptr = new Logging();
		ptr->proxy = target;
		ptr->Init();

		Logging::instance = ptr;
	}

	// -------------------------------------------------------------------------
	void Logging::SetLogFile(const WString& file)
	{
		this->file = file;
	}

	// -------------------------------------------------------------------------
	ILogging* Logging::GetInstance()
	{
		return Logging::instance;
	}

	// -------------------------------------------------------------------------
	void Logging::Destroy(ILogging* logging)
	{
		if(logging)
			logging->Shutdown();
		delete logging;
	}

}
