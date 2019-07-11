//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file pixelformats.h
/// @brief Defines structs for each pixel format the vtf format supports.
//==============================================================================

#ifndef XVTF_PIXELFORMATS_H__
#define XVTF_PIXELFORMATS_H__

#include "xVTF/types.h"

#pragma pack(push, 1)

namespace xvtf
{
	namespace PixelFormats
	{
		struct RGB888
		{
			uchar R;
			uchar G;
			uchar B;
		};

		struct RGBA8888 : public RGB888
		{
			uchar A;
		};

		struct ABGR8888
		{
			uchar A;
			uchar B;
			uchar G;
			uchar R;
		};

		struct BGR888
		{
			uchar B;
			uchar G;
			uchar R;
		};

		struct RGB565
		{
			uint16 R : 5;
			uint16 G : 6;
			uint16 B : 5;
		};

		struct RGBA5651 : RGB565
		{
			uchar A : 1;
		};

		struct RGBA5654 : RGB565
		{
			uchar A : 4;
		};

		struct RGBA5658 : RGB565
		{
			uchar A;
		};

		struct BGR565
		{
			uint16 B : 5;
			uint16 G : 6;
			uint16 R : 5;
		};

		struct A8
		{
			uchar A;
		};

		struct UV88
		{
			uchar U;
			uchar V;
		};

		struct ARGB8888
		{
			uchar A;
			uchar R;
			uchar G;
			uchar B;
		};

		struct BGRA4444
		{
			uint16 B : 4;
			uint16 G : 4;
			uint16 R : 4;
			uint16 A : 4;
		};

		struct BGRA5551
		{
			uint16 B : 5;
			uint16 G : 5;
			uint16 R : 5;
			uint16 A : 1;
		};

		struct BGRA8888
		{
			uchar B;
			uchar G;
			uchar R;
			uchar A;
		};

		struct BGRX5551
		{
			uint16 B : 5;
			uint16 G : 5;
			uint16 R : 5;
			uint16 X : 1;
		};

		struct BGRX8888
		{
			uchar B;
			uchar G;
			uchar R;
			uchar X;
		};

		struct RGBA8881
		{
			uchar R;
			uchar G;
			uchar B;
			uchar A : 1;
		};

		struct I8
		{
			uchar I;
		};

		struct IA88 : public I8
		{
			uchar A;
		};

		struct P8
		{
			uchar P;
		};

		struct RGBA16161616
		{
			uint16 R;
			uint16 G;
			uint16 B;
			uint16 A;
		};

		struct RGBA16161616F
		{
			// todo
		};

		struct UVLX8888
		{
			uchar U;
			uchar V;
			uchar L;
			uchar X;
		};

		struct UVWQ8888
		{
			uchar U;
			uchar V;
			uchar W;
			uchar Q;
		};
	}
}

#pragma pack(pop)

#endif // !XVTF_PIXELFORMATS_H__