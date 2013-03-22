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
#ifndef ICompressedFile_h__
#define ICompressedFile_h__

#include <Aeon/Compress/AeonCompress.h>
#include <Aeon/Compress/Export.h>

namespace Aeon {
	namespace Compress {
		/**
		 * Describes an interface for compressed files.
		 */
		class AEON_COMPRESS_EXPORT ICompressedFile
		{
		public:
			/**
			 * Mounts the specified file for compression.
			 */
			virtual void MountFile(const String& filename) = 0;

			/**
			 * Mounts the specified raw data for compression.
			 */
			virtual void MountRawData(void* data, ulong64_t length) = 0;
		
			/**
			 * Retrieves a list of files from the header.
			 */
			virtual const List& GetFileList() = 0;

			/**
			 * Retrieves the raw data read from the specified file.
			 */
			virtual RawData GetRawData(const String& localFilename) = 0;
		};
	}
}

#endif // ICompressedFile_h__
