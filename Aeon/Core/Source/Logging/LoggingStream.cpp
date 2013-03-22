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
#include <ctime>

#include <Aeon/Logging/ILogging.h>
#include <Aeon/Logging/Logging.h>

#include <Aeon/Logging/LoggingStream.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	// Inline function to determine the current time
	inline String Time()
	{
		char buffer[16];
		time_t curTime = time(nullptr);
		size_t ret = strftime(buffer, sizeof(buffer) / sizeof(char), "%H:%M:%S", localtime(&curTime));

		assert(ret != 0); // Only happens if the time didn't fit into the buffer, which should never happen.

		return buffer;
	}

	// -------------------------------------------------------------------------
	inline String LogSeverityToString(int sev)
	{
		static const Char* strVal[] =
		{
			"DEBUG",
			"INFO",
			"MESSAGE",
			"WARNING",
			"ERROR",
			"CRITICAL",
			"FATAL"
		};

		return strVal[sev];
	}

	// -------------------------------------------------------------------------
	LoggingStream::LoggingStream(ILogging* callback, int severity):
		severity(severity),
		callback(callback)
	{

	}

	// -------------------------------------------------------------------------
	LoggingStream::~LoggingStream()
	{
		if(callback)
		{
			os << std::endl;
			callback->LogMessage(os.str(), severity);
		}
		
		else
			os.clear();
	}

	// -------------------------------------------------------------------------
	void LoggingStream::SetCallback(ILogging* callback)
	{
		this->callback = callback;
	}

	// -------------------------------------------------------------------------
	StringStream& LoggingStream::Get()
	{
		if(callback != 0)
			os << "[" << Time() << "] " << (severity != LOGSEV_NORMAL? LogSeverityToString(severity) : "") << (severity != LOGSEV_NORMAL? ": " : "");

		return os;
	}

	// -------------------------------------------------------------------------
	StringStream& LoggingStream::Get(const char* file, long line, const char* function)
	{
		if(callback)
		{
			os << "[" << Time() << "] " << (severity != LOGSEV_NORMAL? LogSeverityToString(severity) : "") << (severity != LOGSEV_NORMAL? ": " : "");

			if(severity >= LOGSEV_WARNING)
				os << "[" << file << "(" << line << "): " << function << "] ";
		}

		return os;
	}
}
