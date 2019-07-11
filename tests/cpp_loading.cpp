//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file cpp_loading.cpp
/// @brief verifies that the library can load VTF images and that the data is still intact.
//==============================================================================

#define _CRT_SECURE_NO_WARNINGS
#include "xVTF/xVTF.h"

#include "assert_equal.h"
#include "file.h"
#include "library_error.h"

using namespace xvtf;
using namespace xvtf::PixelFormats;

int TestDXT1()
{
	std::string s = DATASETDIR;
	s += "/32x32_DXT1.raw";

	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	RGB888* TempBuffer = new RGB888[32 * 32];
	fread(TempBuffer, sizeof(RGB888), 32 * 32, input);
	fclose(input);

	RGB565* iBuffer = new RGB565[32 * 32];

	// Truncate from 8-bit input to 5/6/5 input
	for (uint16 i = 0; i < 32 * 32; ++i)
	{
		iBuffer[i].R = ((TempBuffer[i].R & 0b11111000) >> 3);
		iBuffer[i].G = ((TempBuffer[i].G & 0b11111100) >> 2);
		iBuffer[i].B = ((TempBuffer[i].B & 0b11111000) >> 3);
	}

	delete[] TempBuffer;

	std::string s2 = DATASETDIR;
	s2 += "/32x32_DXT1.vtf";

	ERRORCODE err;

	auto vtf = VTFFile::Alloc(s2.c_str(), false, &err);

	ASSERT_ERRORCODE_NONE(err);

	auto BMP = vtf->GetImage(0, 0, 0, 0, &err);
	VTFFile::Free(vtf);

	ASSERT_ERRORCODE_NONE(err);

	for (addressable i = 0; i < 32 * 32; ++i)
	{
		auto val = *(RGB565*)BMP->at(i);
		ASSERT_EQUAL_EPSILON_INT(val.R, iBuffer[i].R, 1);
		ASSERT_EQUAL_EPSILON_INT(val.G, iBuffer[i].G, 1);
		ASSERT_EQUAL_EPSILON_INT(val.B, iBuffer[i].B, 1);
	}

	delete[] iBuffer;
	BitmapImage::Free(BMP);

	return 0;
}

int TestDXT1_ONEBITALPHA()
{
	// CURRENTLY UNTESTABLE -- No Way To Make Valid DXT1_ONEBITALPHA Files.
	std::string s = DATASETDIR;
	s += "/32x32_DXT1_OBA.raw";

	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	RGBA8888* TempBuffer = new RGBA8888[32 * 32];
	fread(TempBuffer, sizeof(RGBA8888), 32 * 32, input);
	fclose(input);

	RGBA5651* iBuffer = new RGBA5651[32 * 32];

	// Truncate from 8-bit input to 5/6/5/1 input
	for (uint16 i = 0; i < 32 * 32; ++i)
	{
		iBuffer[i].R = ((TempBuffer[i].R & 0b11111000) >> 3);
		iBuffer[i].G = ((TempBuffer[i].G & 0b11111100) >> 2);
		iBuffer[i].B = ((TempBuffer[i].B & 0b11111000) >> 3);
		iBuffer[i].A = ((TempBuffer[i].A & 0b10000000) >> 7);
	}

	delete[] TempBuffer;

	std::string s2 = DATASETDIR;
	s2 += "/32x32_DXT1_OBA.vtf";

	ERRORCODE err;

	auto vtf = VTFFile::Alloc(s2.c_str(), false, &err);

	ASSERT_ERRORCODE_NONE(err);

	auto BMP = vtf->GetImage(0, 0, 0, 0, &err);
	VTFFile::Free(vtf);

	ASSERT_ERRORCODE_NONE(err);

	for (addressable i = 0; i < 32 * 32; ++i)
	{
		auto val = *(RGBA5651*)BMP->at(i);
		ASSERT_EQUAL_EPSILON_INT(val.R, iBuffer[i].R, 1);
		ASSERT_EQUAL_EPSILON_INT(val.G, iBuffer[i].G, 1);
		ASSERT_EQUAL_EPSILON_INT(val.B, iBuffer[i].B, 1);
		ASSERT_EQUAL(val.A, iBuffer[i].A);
	}

	delete[] iBuffer;
	BitmapImage::Free(BMP);

	return 0;
}

int TestDXT3()
{
	std::string s = DATASETDIR;
	s += "/32x32_DXT3.raw";

	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	RGBA8888* TempBuffer = new RGBA8888[32 * 32];
	fread(TempBuffer, sizeof(RGBA8888), 32 * 32, input);
	fclose(input);

	RGBA5654* iBuffer = new RGBA5654[32 * 32];

	// Truncate from 8-bit input to 5/6/5/1 input
	for (uint16 i = 0; i < 32 * 32; ++i)
	{
		iBuffer[i].R = ((TempBuffer[i].R & 0b11111000) >> 3);
		iBuffer[i].G = ((TempBuffer[i].G & 0b11111100) >> 2);
		iBuffer[i].B = ((TempBuffer[i].B & 0b11111000) >> 3);
		iBuffer[i].A = ((TempBuffer[i].A & 0b11110000) >> 4);
	}

	delete[] TempBuffer;

	std::string s2 = DATASETDIR;
	s2 += "/32x32_DXT3.vtf";

	ERRORCODE err;

	auto vtf = VTFFile::Alloc(s2.c_str(), false, &err);

	ASSERT_ERRORCODE_NONE(err);

	auto BMP = vtf->GetImage(0, 0, 0, 0, &err);
	VTFFile::Free(vtf);

	ASSERT_ERRORCODE_NONE(err);

	for (addressable i = 0; i < 32 * 32; ++i)
	{
		auto val = *(RGBA5654*)BMP->at(i);
		ASSERT_EQUAL_EPSILON_INT(val.R, iBuffer[i].R, 1);
		ASSERT_EQUAL_EPSILON_INT(val.G, iBuffer[i].G, 1);
		ASSERT_EQUAL_EPSILON_INT(val.B, iBuffer[i].B, 1);
		ASSERT_EQUAL_EPSILON_INT(val.A, iBuffer[i].A, 1);
	}

	delete[] iBuffer;

	BitmapImage::Free(BMP);

	return 0;
}

int TestDXT5()
{
	std::string s = DATASETDIR;
	s += "/32x32_DXT5.raw";

	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	RGBA8888* TempBuffer = new RGBA8888[32 * 32];
	fread(TempBuffer, sizeof(RGBA8888), 32 * 32, input);
	fclose(input);

	RGBA5658* iBuffer = new RGBA5658[32 * 32];

	// Truncate from 8-bit input to 5/6/5/1 input
	for (uint16 i = 0; i < 32 * 32; ++i)
	{
		iBuffer[i].R = ((TempBuffer[i].R & 0b11111000) >> 3);
		iBuffer[i].G = ((TempBuffer[i].G & 0b11111100) >> 2);
		iBuffer[i].B = ((TempBuffer[i].B & 0b11111000) >> 3);
		iBuffer[i].A = TempBuffer[i].A;
	}

	delete[] TempBuffer;

	std::string s2 = DATASETDIR;
	s2 += "/32x32_DXT5.vtf";

	ERRORCODE err;

	auto vtf = VTFFile::Alloc(s2.c_str(), false, &err);

	ASSERT_ERRORCODE_NONE(err);

	auto BMP = vtf->GetImage(0, 0, 0, 0, &err);
	VTFFile::Free(vtf);

	ASSERT_ERRORCODE_NONE(err);

	for (addressable i = 0; i < 32 * 32; ++i)
	{
		auto val = *(RGBA5658*)BMP->at(i);
		ASSERT_EQUAL_EPSILON_INT(val.R, iBuffer[i].R, 1);
		ASSERT_EQUAL_EPSILON_INT(val.G, iBuffer[i].G, 1);
		ASSERT_EQUAL_EPSILON_INT(val.B, iBuffer[i].B, 1);
		ASSERT_EQUAL(val.A, iBuffer[i].A);
	}

	delete[] iBuffer;

	BitmapImage::Free(BMP);

	return 0;
}

int main()
{
	return TestDXT1() | TestDXT3() | TestDXT5();
}