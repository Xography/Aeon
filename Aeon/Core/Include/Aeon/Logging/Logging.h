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
#ifndef Logging_h__
#define Logging_h__

#include <fstream>

#include <Aeon/Multicore/Types.h>
#include <Aeon/CmdArgs.h>
#include <Aeon/Logging/ILogging.h>
#include <Aeon/Logging/ILogProxy.h>
#include <Aeon/Logging/LoggingStream.h>

namespace Aeon {
	/**
	 * @brief Actual implementation of the Logging interface, which should be registered as a subsystem to the Subsystem Locator structure
	 * and not being used stand-alone.
	 * 
	 * @see ILogging
	 */
	class AEON_EXPORT Logging : public ILogging
	{
	public:
		/**
		 * 
		 */
		Logging();

		/**
		 * 
		 */
		~Logging();
	
	public:
		/**
		 * Creates a new log linked to the specified file and returns it.
		 */
		static void Create(const WString& file);

		/**
		 * Creates a logging stream without a file, but with a proxy attached to it. All messages will be redirected to this proxy.
		 */
		static void Create(ILogProxy* target);

		/**
		 * Destroys the given logging implementation.
		 */
		static void Destroy(ILogging* logging);


		/**
		 * Returns a Logging instance. If nothing was initialised via Create() before, a null logger will be returned. 
		 */
		static ILogging* GetInstance();
	
	public:

	private:
		/**
		 * Internal init function
		 */
		void Init();

		/**
		 * Sets the file that is used to write the log to.
		 */
		void SetLogFile(const WString& file);

		/**
		 * Sets the level of the log, which is used to suppress messages of certain severities.
		 */
		void SetLogLevel(int level);

		/**
		 * Adds the specified message with the specified severity to the log.
		 */
		void LogMessage(const String& msg, int severity = LOGSEV_INFO);
	
		/**
		 * Shuts the logger down.
		 */
		void Shutdown() override;
	private:
		/**
		 * The path to the file used for logging.
		 */
		WString file;

		/**
		 * The file stream used for logging.
		 */
		std::wofstream fileStream;

		/**
		 * Lock used for logging messages to (file) streams when multiple threads are used.
		 */
		Mutex writeLock;

		/**
		 * The level of the object, used to suppress messages of certain severity levels.
		 */
		int level;

		/**
		 * Logging Flags
		 */
		bool toConsole, toStdErr, forceDebug;

		/**
		 * Instance
		 */
		static ILogging* instance;

		/**
		 * Logging proxy.
		 */
		ILogProxy* proxy;
	};
}

#endif // Logging_h__
