//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file luts.h
/// @brief provides look-up-tables to increase calculation speed for codecs.
//==============================================================================

#ifndef XVTF_LUTS_H__
#define XVTF_LUTS_H__

namespace xvtf
{
	namespace LUT
	{
#pragma warning( push )
#pragma warning( disable : 4838 4309)
		//----------------------------------------------------------------------------------------------------
		/// Provides the Bytes Per Pixel values for each of the ImageFormat (s).
		/// Compressed values stay compressed.
		//----------------------------------------------------------------------------------------------------
		constexpr float ImageFormatBPP[] =
		{ 
			4.0f,				// RGBA8888
			4.0f,				// ABGR8888
			3.0f,				// RGB888
			3.0f,				// BGR888
			2.0f,				// RGB565
			1.0f,				// I8
			2.0f,				// IA88
			1.0f,				// P8
			1.0f,				// A8
			3.0f,				// RGB888_BLUESCREEN
			3.0f,				// BGR888_BLUESCREEN
			4.0f,				// ARGB8888
			4.0f,				// BGRA8888
			0.5f,				// DXT1
			1.0f,				// DXT3
			1.0f,				// DXT5
			4.0f,				// BGRX8888
			2.0f,				// BGR565
			2.0f,				// BGRX5551
			2.0f,				// BGRA4444
			0.5f,				// DXT1_ONEBITALPHA
			2.0f,				// BGRA5551
			2.0f,				// UV88
			4.0f,				// UVWQ8888
			8.0f,				// RGBA16161616F
			8.0f,				// RGBA16161616
			4.0f				// UVLX8888
		};

		//----------------------------------------------------------------------------------------------------
		/// Provides the Bytes Per Pixel values for each of the ImageFormat (s).
		/// Compressed values given in uncompressed size.
		//----------------------------------------------------------------------------------------------------
		constexpr uchar ImageFormatBPPU[] =
		{
			4,					// RGBA8888
			4,					// ABGR8888
			3,					// RGB888
			3,					// BGR888
			2,					// RGB565
			1,					// I8
			2,					// IA88
			1,					// P8
			1,					// A8
			3,					// RGB888_BLUESCREEN
			3,					// BGR888_BLUESCREEN
			4,					// ARGB8888
			4,					// BGRA8888
			3,					// DXT1
			4,					// DXT3
			4,					// DXT5
			4,					// BGRX8888
			2,					// BGR565
			2,					// BGRX5551
			2,					// BGRA4444
			4,					// DXT1_ONEBITALPHA
			2,					// BGRA5551
			2,					// UV88
			4,					// UVWQ8888
			8,					// RGBA16161616F
			8,					// RGBA16161616
			4					// UVLX8888
		};

		// LUT's for quick conversion to RGB888
		constexpr uchar LUT5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
							 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };
		constexpr uchar LUT6[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 45, 49, 53, 57, 61, 65, 69, 73,
						 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 130, 134, 138,
						 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198,
						 202, 206, 210, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255 };
		constexpr uchar LUT4[] = { 0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
#pragma warning( pop )
	}
}

#endif // !XVTF_LUTS_H__
