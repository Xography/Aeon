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
#include <algorithm>
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>

#include <sstream>

#include <Aeon/Compress/AeonCompress.h>
#include <Aeon/Compress/ZipFile.h>
#include "CommonTestUtilities.h"

#include <igloo/igloo.h>
using namespace igloo;
using namespace Aeon;

Context (Test_Zip_Full_Cache)
{

	// -------------------------------------------------------------------------
	void SetUp()
	{
		file = AeonCompress::CreateZipFile("data/test.zip");
	}

	// -------------------------------------------------------------------------
	Spec(zip_file_list_size)
	{
		Assert::That(file->GetFileNameList().size(), Equals(105));
	}

	// -------------------------------------------------------------------------
	Spec(zip_file_test_file_not_found) {
		AssertThat(file->UnpackAsRawMemory("BLARGH.xml").size, Equals(0));
	}

	// -------------------------------------------------------------------------
	Spec(zip_file_test_file_not_found_err_code) {
		AssertThat(file->UnpackAsRawMemory("BLARGH.xml").size, Equals(0));
		AssertThat(file->GetLastZipError(),
			Equals(Compress::ZIP_FILE_NOT_FOUND));
	}

	// -------------------------------------------------------------------------
	Spec(zip_file_test_binary_data) {
		RawMemoryBlock mem = file->UnpackAsRawMemory("images/1.png");
		AssertThat(mem.size, IsGreaterThan(0));
		storeDisk(mem.ptr, mem.size);
		AssertThat(getFilesize("000_test_data.bin"),
			Equals(getFilesize("data/test/images/1.png")));
		AssertThat(file->GetLastZipError(), Equals(Compress::ZIP_FILE_OK));
	}

	// -------------------------------------------------------------------------
	void TearDown() {
		remove( "000_test_data.bin" );
	}


	ZipFile* file;
};
