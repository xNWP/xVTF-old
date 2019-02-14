#ifndef XVTF_CODECS_H__
#define XVTF_CODECS_H__

#include "xExports.h"

#include <memory>

namespace XVTF_NS
{
	namespace Codec
	{
		struct RGB888
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
		};

		struct RGBA8888 : public RGB888
		{
			unsigned char A;
		};

		//----------------------------------------------------------------------------------------------------
		/// Decompresses a DXT1 (no alpha) file and returns a pointer to an array of RGB values.
		/// @param[in] buffer						The buffer of compressed data to decompress.
		/// @param[in] offset						Where in the file to start reading the data from.
		/// @param[in] width						Width of the image in pixels.
		/// @param[in] height						Height of the image in pixels.
		/// @return std::unique_ptr<RGBA8888[]>		The decompressed RGBA8888 values as an array of size width * height.
		//----------------------------------------------------------------------------------------------------
		void* DecompressDXT1(void* buffer, const unsigned int& offset, const unsigned int& width, const unsigned int& height);
	}
}

#endif