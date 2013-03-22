/*******************************************************************************
 *
 * This file is part of the Aeon Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012- The Aeon Development Team.
 * 
 * See LICENSE.txt for licensing details.
 * 
 ******************************************************************************/
#include "AeonTestUtil.h"

#include <Aeon/Platform/Platform.h>

#ifdef AEON_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif


void AeonTestUtil::Sleep(int msec) {
#if AEON_WINDOWS
	::Sleep(msec);
#else
	usleep(msec*1000);
#endif
}