#ifndef XVTF_LUTS_H__
#define XVTF_LUTS_H__

#include "xVTF/xMacros.h"

namespace XVTF_NS
{
	namespace Tools
	{
		namespace LUT
		{
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
		}
	}
}

#endif // !XVTF_LUTS_H__
