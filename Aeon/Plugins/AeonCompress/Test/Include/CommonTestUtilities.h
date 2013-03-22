/*******************************************************************************
 *
 * This file is part of the AEON Framework.
 *
 * -----------------------------------------------------------------------------
 * Copyright (C) 2012 The Aeon Development Team.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 ******************************************************************************/
#ifndef CommonTestUtilities_h__
#define CommonTestUtilities_h__
#include <algorithm>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>


#define BOOST_STATIC_LIBS 1

#include <Aeon/Logging/Logging.h>

using namespace Aeon;
using namespace Aeon::Compress;

// -----------------------------------------------------------------------------
namespace Aeon{
 static void FreeRawMemoryBlock(Compress::RawMemoryBlock* ptr)
 {
	free(ptr->ptr);
 }
}

// -----------------------------------------------------------------------------
static inline String loadDisk(const String& filename)
{
	std::ifstream t(filename.c_str());
	std::stringstream buffer;
	buffer << t.rdbuf();

	return buffer.str();
}

// -----------------------------------------------------------------------------
static inline  void storeDisk(void* mem, uint32_t len)
{
	std::ofstream f("000_test_data.bin", std::ios::out | std::ios::binary);
	f.write(reinterpret_cast<char*>(mem), len);
	f.close();
}

// -----------------------------------------------------------------------------
static inline uint32_t getFilesize(const String& filename)
{
	FILE* pFile = fopen(filename.c_str(), "rb" );
	fseek( pFile, 0, SEEK_END );
	uint32_t Size = ftell( pFile );
	fclose( pFile );
	return Size;
}

#endif // CommonTestUtilities_h__
