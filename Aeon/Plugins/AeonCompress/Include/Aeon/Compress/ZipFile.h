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
#ifndef ZipFile_h__
#define ZipFile_h__

#include <map>
#include <vector>
#include <string>

#include <Aeon/Logging/Logging.h>

#include <Aeon/Compress/Export.h>

namespace Aeon {
	namespace Compress {
		/**
		 * Denotes the status of the zip file.
		 */
		enum ZipFileStatus
		{
			ZIP_NONE = 0,
			ZIP_FILE_OK ,
			ZIP_FILE_ERROR,
			ZIP_FILE_READ_ERROR,
			ZIP_HEAP_ERROR,
			ZIP_DECOMPRESSION_FAILED,
			ZIP_FILE_NOT_FOUND,
			ZIP_NO_FILES
		};


		/**
		 * The zip mode.
		 */
		enum ZipMode
		{
			ZIP_CACHE_ALLOCATE_FULL = 0,
			ZIP_CACHE_ON_DEMAND,
			ZIP_CACHE_NONE
		};

		/**
		 * Describes a linear memory block.
		 */
		struct LinearMemoryBlock
		{
			/**
			 * Constructor.
			 */
			explicit LinearMemoryBlock(size_t size):
				size(size),
				offset(0)
			{
				data = reinterpret_cast<uint8_t*>(malloc(size));
			}

			/**
			 * Destructor.
			 */
			/*~LinearMemoryBlock()
			{
				if(data != 0)
				{
					free(data);
					data = 0;
				}
			}*/

			/**
			 * Pointer to the memory region of this block.
			 */
			uint8_t* data;

			/**
			 * The size of this block.
			 */
			size_t size;

			/**
			 * The offset is this block.
			 */
			size_t offset;
		};

		/**
		 * Describes a raw memory block.
		 */
		struct RawMemoryBlock
		{
			/**
			 * Constructor.
			 */
			explicit RawMemoryBlock():
				ptr(nullptr),
				size(0)
			{

			}

			/**
			 * A pointer to the location of this raw memory block.
			 */
			void* ptr;

			/**
			 * The size of this block.
			 */
			size_t size;
		};

		// -------------------------------------------------------------------------
		class AEON_COMPRESS_EXPORT ICompressionAlgorithm;
		class AEON_COMPRESS_EXPORT IMemoryStrategy;
		class AEON_COMPRESS_EXPORT ZlibCompressedFile;

		typedef std::vector<String> FileList;

		/**
		 * Represents a zip file.
		 */
		class AEON_COMPRESS_EXPORT ZipFile
		{
		private:
			/**
			 * Constructor.
			 */
			ZipFile(const String& file, IMemoryStrategy* strategy, ZipMode mode);

			/**
			 * Copy constructor.
			 */
			ZipFile(const ZipFile& /*other*/)
			{
			
			}

			/**
			 * Assignment operator.
			 */
			ZipFile& operator=(const ZipFile& /*other*/)
			{
				return (*this);
			}

		public:
			/**
			 * Destructor.
			 */
			~ZipFile();

		public:
			/**
			 * Returns the current state (or last error) of this zip file. Use this to determine whether unpacking was OK. In this case, the
			 * error is ZIP_FILE_OK
			 */
			int GetLastZipError() const;

			/**
			 * Returns a reduced copy of the internal dictionary containing only filenames. The list is generated on the fly when
			 * calling this function, use sparingly.
			 */
			const FileList GetFileNameList() const;

		public:
			/**
			 * Returns an information container containing the raw memory pointer and length of the requested file for further processing.
			 * 
			 * @note If the requested file was not found, the pointer and length are 0, the call has to check this manually!
			 */
			const RawMemoryBlock UnpackAsRawMemory(const String& file) const;

			/**
			 * Returns a copy of the requested file as String. If the requested file was not found, an empty string is returned.
			 */
			const String UnpackAsString(const String& file) const;

		public:
			/**
			 * Creates a new zip file from the specified file and in the specified mode.
			 */
			static ZipFile* Create(const String& file, ZipMode mode);

		private:
			/**
			 * The implementation of the ZIP library.
			 */
			ZlibCompressedFile* zipImpl;

			/**
			 * The memory strategy to use.
			 */
			IMemoryStrategy* strategy;
		};
	}
}

#endif // ZipFile_h__
