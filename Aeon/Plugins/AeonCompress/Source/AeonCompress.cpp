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
#include <Aeon/Compress/AeonCompress.h>

namespace Aeon {
	// -------------------------------------------------------------------------
	Compress::ZipFile* Compress::AeonCompress::CreateZipFile(const String& file, Compress::ZipMode mode /*= ZipMode::ZIP_CACHE_ALLOCATE_FULL*/)
	{
		return Compress::ZipFile::Create(file, mode);
	}
}
