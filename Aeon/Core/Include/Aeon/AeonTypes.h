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
#ifndef AeonTypes_h__
#define AeonTypes_h__

#include <string>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <cstdint>
#include <sstream>
#include <iostream>

#include <Aeon/Export.h>

#define WSTR(x) L##x
#define AEON_INTERFACE class

#ifndef AEON_EXPORT
	#define AEON_API AEON_EXPORT
#else
	#define AEON_API static inline
#endif
// This will cause this disabled warning to become propagated to all files, which is the intent.
// We don't want Microsoft's crappy "secure" alternatives to the stdlib, since they aren't portable.

// TODO: This should better be replaced by a compile switch placed in generated VS project files in
// CMake, the switch is /wd"4996". That should hopefully also take care of these warnings originating
// from the Boost library.
#ifdef _MSC_VER
#	pragma warning(disable : 4996)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif // _MSC_VER

namespace Aeon {
	typedef wchar_t WChar;
	typedef std::wstring WString;
	typedef std::wofstream WFileStream;
	typedef std::wostringstream WStringStream;
	typedef std::wostream WBaseStream;

	typedef char Char;
	typedef std::string String;
	typedef std::ofstream FileStream;
	typedef std::ostringstream StringStream;
	typedef std::ostream BaseStream;

	typedef float Real;
	
	#define ATTR(text) Aeon::AttributeKey(text)

	/**
	 * @brief A class wrapping a stdlib vector.
	 * 
	 * TODO: This could be replaced with a C++11 template typedef, once it is supported.
	 */
	template<typename T> struct Vector
	{
		typedef std::vector<T> type;
	};

	/**
	 * Converts the specified ASCII-string to a wide UTF-encoded string and returns it.
	 */
	AEON_API WString ToWString(const String& str) 
	{
		std::wstringstream sstr;
		sstr << str.c_str();
		return sstr.str();
	}
	
	/**
	 * Converts a wide string back to an ASCII string, note that it's not always safe to do so!
	 */
	AEON_API String ToString(const WString& str)
	{
		char* tmp = new char[str.length()];
		wcstombs(tmp, str.c_str(), str.length());
		String s(tmp);
		delete[] tmp;
		return s;
	}
}
/*
// Required for zlib backwards comaptibility
#if defined(_M_X64) || defined(_WIN64) || defined(__MINGW64__) || defined(_LP64) || defined(__LP64__) || defined(__ia64__) || defined(__x86_64__)
	typedef unsigned long uint;
#else
	typedef unsigned int uint;
#endif // Platforms
*/

#endif // AeonTypes_h__
