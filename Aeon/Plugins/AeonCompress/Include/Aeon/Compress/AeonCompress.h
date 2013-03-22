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
#ifndef AeonCompress_h__
#define AeonCompress_h__

#include <string>
#include <vector>

#include <Aeon/Compress/Export.h>
#include <Aeon/Compress/ZipFile.h>

namespace Aeon {
	namespace Compress {
		/**
		 * Compression plug-in for Aeon.
		 */
		class AEON_COMPRESS_EXPORT AeonCompress
		{
		public:
			static ZipFile* CreateZipFile(const String& file, ZipMode mode = ZIP_CACHE_ALLOCATE_FULL);
		};

	}
}

#endif // AeonCompress_h__
