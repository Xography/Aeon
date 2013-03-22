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
#ifndef Platform_h__
#define Platform_h__

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32)
#	define AEON_WINDOWS			1
#elif defined(__APPLE__) || defined(__MACH__)
#	define AEON_MACOS			1
#else
#	define AEON_LINUX			1
#endif // Platform

#endif // Platform_h__
