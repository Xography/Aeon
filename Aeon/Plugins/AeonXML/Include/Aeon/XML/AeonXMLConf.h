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
#ifndef AeonXMLConf_h__
#define AeonXMLConf_h__

#include <Aeon/Platform/Platform.h>

// Disable expats export/import symbols since we're using it standalone.
#define XMLCALL
#define XMLIMPORT

// We'll disable the Microsoft warning, that some C functions are unsafe, since they're proven rather safe every day.
#ifdef _MSC_VER
#	pragma warning(disable : 4996)
#endif // _MSC_VER

/* we will assume all platforms are little endian */
#define BYTEORDER 1234

/* Windows has memmove() available. */
#define HAVE_MEMMOVE 1

#define XML_CONTEXT_BYTES 1024

#define EXPAT_CHUNK_SIZE 10240

#ifdef XML_UNICODE_WCHAR_T
#	define XML_UNICODE
#endif

#ifdef XML_UNICODE     /* Information is UTF-16 encoded. */
#	ifdef XML_UNICODE_WCHAR_T
		typedef wchar_t XML_Char;
		typedef wchar_t XML_LChar;
#	else
		typedef unsigned short XML_Char;
		typedef char XML_LChar;
#	endif /* XML_UNICODE_WCHAR_T */
#else                  /* Information is UTF-8 encoded. */
	typedef char XML_Char;
	typedef char XML_LChar;
#endif /* XML_UNICODE */

#endif // AeonXMLConf_h__
