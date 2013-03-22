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
#ifndef LoggingStream_h__
#define LoggingStream_h__

#include <Aeon/Logging/NullStream.h>

#include <Aeon/Logging/Logging.h>

#define DEBUG					LOGSEV_DEBUG
#define	INFO					LOGSEV_INFO
#define NORMAL					LOGSEV_NORMAL
#define WARNING					LOGSEV_WARNING
#define ERR						LOGSEV_ERROR
#define CRITICAL				LOGSEV_CRITICAL
#define FATAL					LOGSEV_FATAL

// These Aeon:: prefixes are required to make the macro's work outside the Aeon namespace without having to issue "using namespace Aeon" in that source.
#define AEON_LOG(__SEV__)		Aeon::LoggingStream(Aeon::Logging::GetInstance(), Aeon::__SEV__).Get(__FILE__, __LINE__, __FUNCTION__)

#define LOG_SEV(severity)		LOG_##severity
#define LOG_INFO				AEON_LOG(INFO)
#define LOG_NORMAL				AEON_LOG(NORMAL)
#define LOG_WARNING				AEON_LOG(WARNING)
#define LOG_ERROR				AEON_LOG(ERR)
#define LOG_CRITICAL			AEON_LOG(CRITICAL)
#define LOG_FATAL				AEON_LOG(FATAL)

#define LOG						LOG_NORMAL

#if _DEBUG
#	define LOG_DEBUG			AEON_LOG(DEBUG)
#else
#	define LOG_DEBUG			Aeon::NullStream()
#endif

namespace Aeon {
	class ILogging;

	/**
	 * @brief A simple logging stream to allow easy logging via redirecting messages to a LOG object using the insertion operator.
	 * 
	 * @note Do not use this class directly, use the LOG macros instead.
	 */
	class AEON_EXPORT LoggingStream
	{
	public:
		/**
		 * Constructor.
		 *
		 * @param callback The callback to use when something needs to be logged.
		 * @param severity The severity of the logging stream.
		 */
		LoggingStream(ILogging* callback = 0, int severity = 
			#ifdef _DEBUG 
				LOGSEV_DEBUG 
			#else 
				LOGSEV_INFO
			#endif // _DEBUG
	);

	private:
		/**
		 * Copy constructor.
		 */
		LoggingStream(const LoggingStream&);
		
		/**
		 * Assignment operator.
		 */
		LoggingStream& operator =(const LoggingStream&);

	public:
		/**
		 * Destructor.
		 */
		~LoggingStream();

	public:
		/**
		 * Retrieves a string stream fit for the specified file, line and function.
		 */
		StringStream& Get(const char* file, long line, const char* function);

		/**
		 * Just returns the string stream in its current state.
		 */
		StringStream& Get();

		/**
		 * Sets the callback to use for this stream.
		 */
		void SetCallback(ILogging* callback);

	protected:
		/**
		 * The stream used for the logging stream.
		 */
		StringStream os;

		/**
		 * The severity of the stream.
		 */
		int severity;

		/**
		 * The callback to use.
		 */
		ILogging* callback;
	};
}

#endif // LoggingStream_h__
