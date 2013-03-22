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
#ifndef CmdArgs_h__
#define CmdArgs_h__

#include <map>
#include <Aeon/AeonTypes.h>
#include <Aeon/Export.h>

namespace Aeon {
	/**
	 * @brief A command line argument interpreter, matches args of the following format: --key=val.
	 */
	class AEON_EXPORT CmdArgs
	{
	public:
		/**
		 * @brief Raw arguments wrapper.
		 */
		struct RawArgs
		{
			int argc;
			char** argv;
		};	

	private:
		/**
		 * Constructor.
		 */
		CmdArgs(int argc, char** argv);

		/**
		 * Destructor.
		 */
		~CmdArgs();

	public:
		/**
		 * Check whether a given key was passed on the command line.
		 * 
		 * @param argKey the arg key (--key=val), without preceding dashes.
		 */
		bool Has(const String& argKey);

		/**
		 * Get the value of the given argument key, if it exists.
		 * 
		 * @param argKey argument key.
		 *
		 * @return the corresponding argument value, else empty string "".
		 */
		const String& Get(const String& argKey);

		/**
		 * Get the name of the executable (argv[0]).
		 *
		 * @return executable name.
		 */
		const String& GetExecutableName();

		/**
		 * Extracts the absolute path to the directory of the executable from argv[0], does not contain a trailing separator.
		 *
		 * @return the path to the executable.
		 */
		const String& GetExecutablePath();
		
		/**
		 * Get raw args for further processing.
		 * 
		 * @return RawArgs Container (contains raw argc, argv).
		 */
		const RawArgs& GetRawArguments();

	public:
		/**
		 * Factory method. Creates a new CmdArgs Instance. Use Destroy() to 
		 * delete instance.
		 * 
		 * @param argc arg counter
		 * @param argv arg values
		 * @return CmdArgs Instance
		 */
		static CmdArgs* Create(int argc, char** argv);

		/**
		 * Destroys the given instance and sets the given pointer to 0.
		 * 
		 * @param ptr pointer of the instance to destroy.
		 * 
		 */
		static void Destroy(CmdArgs* ptr);

	private:
		/**
		 * A map of keys to values for command line arguments for easy access.
		 */
		std::map<String, String/*, Allocator */> args;

		/**
		 * The command line arguments in their raw form.
		 */
		RawArgs rawArgs;

		/**
		 * The name of the executable and the path to its containing directory.
		 */
		String executableName, executablePath;
	};
}
#endif // CmdArgs_h__
