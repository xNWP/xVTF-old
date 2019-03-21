#ifndef XVTF_LUTS_H__
#define XVTF_LUTS_H__

#include "xVTF/xMacros.h"

namespace XVTF_NS
{
	namespace Tools
	{
		namespace LUT
		{
#pragma warning( push )
#pragma warning( disable : 4838 4309)
			//----------------------------------------------------------------------------------------------------
			/// Provides the Bytes Per Pixel values for each of the ImageFormat (s).
			/// Compressed values stay compressed.
			//----------------------------------------------------------------------------------------------------
			const float ImageFormatBPP[] =
			{ 4.0f, 4.0f, 3.0f, 3.0f, 2.0f, 1.0f, 2.0f,
			  1.0f, 1.0f, 3.0f, 3.0f, 4.0f, 4.0f, 0.5f,
			  1.0f, 1.0f, 4.0f, 2.0f, 2.0f, 2.0f, 0.5f,
			  2.0f, 2.0f, 4.0f, 8.0f, 8.0f, 4.0f };

			//----------------------------------------------------------------------------------------------------
			/// Provides the Bytes Per Pixel values for each of the ImageFormat (s).
			/// Compressed values given in uncompressed size.
			//----------------------------------------------------------------------------------------------------
			const unsigned char ImageFormatBPPU[] =
			{ 4, 4, 3, 3, 3, 1, 2, 1, 1, 3, 3, 4, 4, 3, 4, 4, 4, 3, 4, 4, 4, 4, 2, 4, 8, 8, 4 };

			// LUT's for quick conversion to RGB888
			const char LUT5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
								 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };
			const char LUT6[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 45, 49, 53, 57, 61, 65, 69, 73,
							 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 130, 134, 138,
							 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198,
							 202, 206, 210, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255 };
			const char LUT4[] = { 0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
#pragma warning( pop )
		}
	}
}

#endif // !XVTF_LUTS_H__
