//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file codecs.h
/// @brief provides several functions used internally by the library to perform image operations.
//==============================================================================

#ifndef XVTF_CODECS_H__
#define XVTF_CODECS_H__

#include "xVTF/pixelformats.h"
#include "xVTF/types.h"

namespace xvtf
{
	namespace Codecs
	{
		//----------------------------------------------------------------------------------------------------
		/// Mixes two RGB888 values together.
		/// @param[in] _A			The first colour to mix.
		/// @param[in] _B			The second colour to mix.
		/// @param[in] PartsA		How many parts of _A to mix in.
		/// @param[in] PartsB		How many parts of _B to mix in.
		/// @return RGB888			The blended colour.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGB888 Mix(const PixelFormats::RGB888& _A, const PixelFormats::RGB888& _B, uint32 PartsA, uint32 PartsB);

		//----------------------------------------------------------------------------------------------------
		/// Mixes two RGBA8888 values together.
		/// @param[in] _A			The first colour to mix.
		/// @param[in] _B			The second colour to mix.
		/// @param[in] PartsA		How many parts of _A to mix in.
		/// @param[in] PartsB		How many parts of _B to mix in.
		/// @return RGBA8888		The blended colour.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGBA8888 Mix(const PixelFormats::RGBA8888& _A, const PixelFormats::RGBA8888& _B, uint32 PartsA, uint32 PartsB);

		//----------------------------------------------------------------------------------------------------
		/// Mixes two RGB565 values together.
		/// @param[in] _A			The first colour to mix.
		/// @param[in] _B			The second colour to mix.
		/// @param[in] PartsA		How many parts of _A to mix in.
		/// @param[in] PartsB		How many parts of _B to mix in.
		/// @return RGB565			The blended colour.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGB565 Mix(const PixelFormats::RGB565& _A, const PixelFormats::RGB565& _B, uint32 PartsA, uint32 PartsB);

		//----------------------------------------------------------------------------------------------------
		/// Mixes two RGBA5651 values together.
		/// @param[in] _A			The first colour to mix.
		/// @param[in] _B			The second colour to mix.
		/// @param[in] PartsA		How many parts of _A to mix in.
		/// @param[in] PartsB		How many parts of _B to mix in.
		/// @return RGBA5651		The blended colour.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGBA5651 Mix(const PixelFormats::RGBA5651& _A, const PixelFormats::RGBA5651& _B, uint32 PartsA, uint32 PartsB);

		//----------------------------------------------------------------------------------------------------
		/// Decompresses a DXT1 (no alpha) file and returns a pointer to an array of RGB values.
		/// @param[in] buffer						The buffer of compressed data to decompress.
		/// @param[in] offset						Where in the buffer to start reading the data from.
		/// @param[in] width						Width of the image in pixels.
		/// @param[in] height						Height of the image in pixels.
		/// @return RGB565*							The decompressed RGB565 values as an array of size width * height.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGB565* DecompressDXT1(const void* buffer, addressable offset, uint16 width, uint16 height);

		//----------------------------------------------------------------------------------------------------
		/// Decompresses a DXT1 (1-bit alpha) file and returns a pointer to an array of RGBA values.
		/// @param[in] buffer						The buffer of compressed data to decompress.
		/// @param[in] offset						Where in the buffer to start reading the data from.
		/// @param[in] width						Width of the image in pixels.
		/// @param[in] height						Height of the image in pixels.
		/// @return RGBA5651*						The decompressed RGBA5651 values as an array of size width * height.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGBA5651* DecompressDXT1_ONEBITALPHA(const void* buffer, addressable offset, uint16 width, uint16 height);

		//----------------------------------------------------------------------------------------------------
		/// Decompresses a DXT3 (4-bit alpha) file and returns a pointer to an array of RGBA values.
		/// @param[in] buffer						The buffer of compressed data to decompress.
		/// @param[in] offset						Where in the buffer to start reading the data from.
		/// @param[in] width						Width of the image in pixels.
		/// @param[in] height						Height of the image in pixels.
		/// @return RGBA5654*						The decompressed RGBA5654 values as an array of size width * height.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGBA5654* DecompressDXT3(const void* buffer, addressable offset, uint16 width, uint16 height);

		//----------------------------------------------------------------------------------------------------
		/// Decompresses a DXT5 (paletted alpha) file and returns a pointer to an array of RGBA values.
		/// @param[in] buffer						The buffer of compressed data to decompress.
		/// @param[in] offset						Where in the buffer to start reading the data from.
		/// @param[in] width						Width of the image in pixels.
		/// @param[in] height						Height of the image in pixels.
		/// @return RGBA5654*						The decompressed RGBA5654 values as an array of size width * height.
		//----------------------------------------------------------------------------------------------------
		PixelFormats::RGBA5658* DecompressDXT5(const void* buffer, addressable offset, uint16 width, uint16 height);
	}
}

#endif // !XVTF_CODECS_H__