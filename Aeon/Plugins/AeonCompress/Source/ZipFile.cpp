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
#include <Aeon/Compress/Export.h>
#include <Aeon/Platform/Platform.h>

#include <iterator>
#include <algorithm>

#include <cstring>
#include <unzip.h>

#include <Aeon/Compress/ZipFile.h>
#include <Aeon/Compress/IMemoryStrategy.h>
#include <Aeon/Compress/ICompressionAlgorithm.h>

#define CASESENSITIVITY (0)
#define WRITEBUFFERSIZE (8192)
#define MAXFILENAME (256)

#if (!defined(AEON_WINDOWS) && !defined(AEON_MACOS))
#	ifndef __USE_FILE_OFFSET64
#		define __USE_FILE_OFFSET64
#	endif

#	ifndef __USE_LARGEFILE64
#		define __USE_LARGEFILE64
#	endif

#	ifndef _LARGEFILE64_SOURCE
#		define _LARGEFILE64_SOURCE
#	endif

#	ifndef _FILE_OFFSET_BIT
#		define _FILE_OFFSET_BIT 64
#	endif
#endif

#ifdef AEON_MACOS
	// In darwin and perhaps other BSD variants off_t is a 64 bit value, hence no need for specific 64 bit functions
#	define FOPEN_FUNC(filename, mode) fopen(filename, mode)
#	define FTELLO_FUNC(stream) ftello(stream)
#	define FSEEKO_FUNC(stream, offset, origin) fseeko(stream, offset, origin)
#else
#	define FOPEN_FUNC(filename, mode) fopen64(filename, mode)
#	define FTELLO_FUNC(stream) ftello64(stream)
#	define FSEEKO_FUNC(stream, offset, origin) fseeko64(stream, offset, origin)
#endif // AEON_MACOS

#ifdef AEON_WINDOWS
#	include <direct.h>
#	include <io.h>
#
#	define USEWIN32IOAPI
#	include <iowin32.h>
#else
#	include <unistd.h>
#	include <utime.h>
#endif // AEON_WINDOWS


namespace Aeon {
	namespace Compress {
	// -------------------------------------------------------------------------
	/**
	 * Describes a compressed file.
	 */
	struct CompressedFile
	{
		char filename[256 + 1];
		uint32_t uncompressedSize;
		uint32_t offset;
	};


	typedef std::map<const String, CompressedFile> Dictionary;

	// -------------------------------------------------------------------------
	class ZlibCompressedFile : public ICompressionAlgorithm
	{
	public:
		// ---------------------------------------------------------------------
		ZlibCompressedFile():
			errCode(Compress::ZIP_NONE),
			mode(ZIP_CACHE_ALLOCATE_FULL),
			zip(0),			
			rawData(nullptr)
		{

		}

		// ---------------------------------------------------------------------
		~ZlibCompressedFile()
		{
			if(zip)
				Close();

			FreeRawData();
		}

		// ---------------------------------------------------------------------
		void Open(const char* fname)
		{
			zip = unzOpen(fname);
		}

		// ---------------------------------------------------------------------
		void ReadDictionary(const String& file, ZipMode mode)
		{
			this->mode = mode;
			ReadFile(file, mode == ZIP_CACHE_ALLOCATE_FULL);
		}

		// ---------------------------------------------------------------------
		void ReadFile(const String& file, bool cacheAll = false)
		{
			Close();

			FreeRawData();

			Open(file.c_str());

			unz_global_info info;
			if(unzGetGlobalInfo(zip, &info) != UNZ_OK)
			{
				errCode = Compress::ZIP_FILE_READ_ERROR;
				return;
			}

			if(info.number_entry == 0)
			{
				errCode = Compress::ZIP_NO_FILES;
				return;
			}

			uint32_t uncompressed_size = 0;

			if(cacheAll)
			{
				uncompressed_size = getUncompressedBlockSize();

				if(Close() != UNZ_OK)
				{
					errCode = Compress::ZIP_FILE_ERROR;
					return;
				}

				Open(file.c_str());

				if(uncompressed_size <= 0)
				{
					errCode = Compress::ZIP_FILE_READ_ERROR;
					return;
				}

				rawData = new LinearMemoryBlock(uncompressed_size);

			}

			do
			{
				unz_file_info file_info;
				char filename[256+1];

				if(unzGetCurrentFileInfo(zip, &file_info, filename, sizeof(filename), nullptr, 0, nullptr, 0) != UNZ_OK)
				{
					errCode = Compress::ZIP_FILE_READ_ERROR;
					return;
				}

				if(file_info.uncompressed_size > 0)
				{
					CompressedFile entry = {{0}, 0, 0};

					#ifdef _MSC_VER
						strncpy_s(entry.filename, filename, 256);
					#else
						strncpy(entry.filename, filename, 256);
					#endif // _MSC_VER
					
					filename[256]  = '\0';

					if(cacheAll)
					{
						entry.uncompressedSize = file_info.uncompressed_size;

						if(!unpackToRawMemory(&entry, file_info.uncompressed_size))
						{
							Close();
							FreeRawData();
							errCode = Compress::ZIP_HEAP_ERROR;
							return;
						}
					}

					else
						entry.uncompressedSize = 0;

					dictionary[entry.filename] = entry;
				}
			}
			while(unzGoToNextFile(zip) == UNZ_OK);

			if(cacheAll)
				Close();
		}

		// ---------------------------------------------------------------------
		RawMemoryBlock GetRawMem(const String& filename)
		{
			RawMemoryBlock mem;

			if(dictionary[filename].uncompressedSize > 0)
			{
				mem.ptr = this->rawData->data+dictionary[filename]. offset;
				mem.size = dictionary[filename].uncompressedSize;
			}

			else if(mode != ZIP_CACHE_ALLOCATE_FULL)
				mem  = SeekFileOnDisk(filename.c_str());
			
			else
			{
				errCode = Compress::ZIP_FILE_NOT_FOUND;
				return mem;
			}

			if(mem.size > 0)
				errCode = Compress::ZIP_FILE_OK;

			return mem;
		}

		// ---------------------------------------------------------------------
		RawMemoryBlock SeekFileOnDisk(const char* filename)
		{
			RawMemoryBlock mem;

			if(!zip)
			{
				errCode = Compress::ZIP_FILE_ERROR;
				return mem;
			}

			if(unzLocateFile(zip, filename, 2) != UNZ_OK)
			{
				errCode = Compress::ZIP_FILE_NOT_FOUND;
				return mem;
			}

			unz_file_info file_info;

			if(unzGetCurrentFileInfo(zip, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK)
			{
				errCode = Compress::ZIP_FILE_READ_ERROR;
				return mem;
			}

			/*
			if(mode == ZIP_CACHE_ON_DEMAND) {
				dictionary[filename].uncompressed_size = file_info.uncompressed_size;
			}
			*/

			mem.size = file_info.uncompressed_size;
			mem.ptr = malloc(file_info.uncompressed_size);

			if(!unzipRawMem(mem.ptr, mem.size))
			{
				Close();
				FreeRawData();
				mem.size = 0;
				free(mem.ptr);
				mem.ptr = 0;
				errCode = Compress::ZIP_HEAP_ERROR;
			}

			return mem;

		}
		// ---------------------------------------------------------------------
		int Close()
		{
			int ret = 0;

			if(zip != 0)
			{
				ret = unzClose(zip);
				zip = 0;

				errCode = Compress::ZIP_NONE;
			}

			return ret;
		}

		// ---------------------------------------------------------------------
		const std::map<const String, CompressedFile>& GetDictionary() const
		{
			return dictionary;
		}

		// ---------------------------------------------------------------------
		void FreeRawData()
		{
			if(rawData != 0)
			{
				free(rawData->data);
				rawData->offset = 0;
				rawData->size = 0;
				delete rawData;
				rawData = 0;

				errCode = Compress::ZIP_NONE;
			}
		}

		// ---------------------------------------------------------------------
		int GetError() const
		{
			return errCode;
		}

	private:
		int errCode;
		ZipMode mode;
		unzFile zip;
		LinearMemoryBlock* rawData;
		std::map<const String, CompressedFile> dictionary;

		// ---------------------------------------------------------------------
		void* alloc(uint32_t size)
		{
			if(size + rawData->offset > rawData->size || rawData==0)
				return 0;

			void* mem = rawData->data + rawData->offset;
			rawData->offset += size;
			return mem;
		}

		// ---------------------------------------------------------------------
		bool unpackToRawMemory(CompressedFile* entry, uint32_t uncompressed_size)
		{
			entry->offset = rawData->offset;
			void* mem = alloc(uncompressed_size);

			if(!mem)
			{
				errCode = Compress::ZIP_HEAP_ERROR;
				throw std::bad_alloc();
			}

			return unzipRawMem(mem, uncompressed_size);

		}
		// ---------------------------------------------------------------------
		bool unzipRawMem(void* ptr, uint32_t size)
		{
			if(unzOpenCurrentFile(zip) != UNZ_OK)
			{
				errCode = Compress::ZIP_FILE_READ_ERROR;
				return false;
			}

			if(unzReadCurrentFile(zip, ptr, size) != static_cast<int>(size))
			{
				errCode = Compress::ZIP_DECOMPRESSION_FAILED;
				return false;
			}

			if(unzCloseCurrentFile(zip) != UNZ_OK)
			{
				errCode = Compress::ZIP_FILE_ERROR;
				return false;
			}

			return true;
		}
		// ---------------------------------------------------------------------
		uint32_t getUncompressedBlockSize()
		{
			if(unzGoToFirstFile(zip) != UNZ_OK)
				return 0;

			uint32_t sum = 0;

			do
			{
				unz_file_info file_info;

				if(unzGetCurrentFileInfo(zip, &file_info, nullptr, 0, nullptr, 0, nullptr, 0) != UNZ_OK)
				{
					errCode = Compress::ZIP_FILE_ERROR;
					return 0;
				}

				if(file_info.uncompressed_size > 0)
				{
					sum += file_info.uncompressed_size;
				}
			}
			while(unzGoToNextFile(zip) == UNZ_OK);

			return sum;
		}

	};

	class MinimalAllocationStrategy : public IMemoryStrategy
	{

	};

	class OnDemandAllocationStrategy : public IMemoryStrategy
	{

	};

	class FullAllocationStrategy : public IMemoryStrategy
	{

	};

	// -------------------------------------------------------------------------
	struct CompressedFileKey
	{
		template<typename T> typename T::first_type operator()(T keyValuePair) const
		{
			return keyValuePair.first;
		}
	};

	// -------------------------------------------------------------------------
	ZipFile::ZipFile(const String& file, IMemoryStrategy* strategy, ZipMode mode):
		zipImpl(0), 
		strategy(strategy)
	{
		zipImpl = new ZlibCompressedFile();
		zipImpl->ReadDictionary(file, mode);
	}

	// -------------------------------------------------------------------------
	ZipFile::~ZipFile()
	{
		delete zipImpl;
		delete strategy;
	}

	// -------------------------------------------------------------------------
	int ZipFile::GetLastZipError() const
	{
		return zipImpl->GetError();
	}

	// -------------------------------------------------------------------------
	const FileList ZipFile::GetFileNameList() const
	{
		std::vector<std::string> files;

		Dictionary dict = zipImpl->GetDictionary();
		std::transform(dict.begin(), dict.end(), std::back_inserter(files), CompressedFileKey());
		return files;
	}

	// -------------------------------------------------------------------------
	const RawMemoryBlock ZipFile::UnpackAsRawMemory(const String& file) const
	{
		return zipImpl->GetRawMem(file);
	}

	// -------------------------------------------------------------------------
	const String ZipFile::UnpackAsString( const String& file ) const
	{
		RawMemoryBlock mem = UnpackAsRawMemory(file);
		return String(reinterpret_cast<char*>(mem.ptr), mem.size).c_str();
	}

	// -------------------------------------------------------------------------
	ZipFile* ZipFile::Create( const String& file, ZipMode mode )
	{
		switch (mode)
		{
		case ZIP_CACHE_NONE:		return new ZipFile(file,new MinimalAllocationStrategy(),mode);
		case ZIP_CACHE_ON_DEMAND:	return new ZipFile(file, new OnDemandAllocationStrategy(),mode);
		default:					return new ZipFile(file,new FullAllocationStrategy(),mode);
		}
	}

}}
