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
#ifndef ILogging_h__
#define ILogging_h__

#include <Aeon/AeonTypes.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * Log message severity indicator.
	 */
	enum LogSeverity
	{
		LOGSEV_DEBUG = 0,
		LOGSEV_INFO = 1,
		LOGSEV_NORMAL = 2,
		LOGSEV_WARNING = 3,
		LOGSEV_ERROR = 4,
		LOGSEV_CRITICAL = 5,
		LOGSEV_FATAL = 6
	};

	/**
	 * @brief Interface for logging subsystems.
	 */
	class AEON_EXPORT ILogging
	{
	public:
		/**
		 * Sets the level of the log, if a message's severity is smaller than the level, it will not be recorded. This can be used to ignore less significant messages.
		 */
		virtual void SetLogLevel(int level) = 0;

		/**
		 * Adds a new message to the log with the specified severity.
		 */
		virtual void LogMessage(const String& msg, int severity) = 0;

		/**
		 * Request the logging system to shut down.
		 */
		virtual void Shutdown() = 0;
	};
}

#endif // ILogging_h__
