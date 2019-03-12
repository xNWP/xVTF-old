#ifndef XVTF_PIXELFORMATS_H__
#define XVTF_PIXELFORMATS_H__

#include "xVTF/xMacros.h"

namespace XVTF_NS
{
	namespace Bitmap
	{
		namespace PixelFormats
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

			struct ABGR8888
			{
				unsigned char A;
				unsigned char B;
				unsigned char G;
				unsigned char R;
			};

			struct BGR888
			{
				unsigned char B;
				unsigned char G;
				unsigned char R;
			};

			struct RGB565
			{
				unsigned char R : 5;
				unsigned char G : 6;
				unsigned char B : 5;
			};

			struct BGR565
			{
				unsigned char B : 5;
				unsigned char G : 6;
				unsigned char R : 5;
			};

			struct A8
			{
				unsigned char A;
			};

			struct UV88
			{
				unsigned char U;
				unsigned char V;
			};

			struct ARGB8888
			{
				unsigned char A;
				unsigned char R;
				unsigned char G;
				unsigned char B;
			};

			struct BGRA4444
			{
				unsigned char B : 4;
				unsigned char G : 4;
				unsigned char R : 4;
				unsigned char A : 4;
			};

			struct BGRA5551
			{
				unsigned char B : 5;
				unsigned char G : 5;
				unsigned char R : 5;
				unsigned char A : 1;
			};

			struct BGRA8888
			{
				unsigned char B;
				unsigned char G;
				unsigned char R;
				unsigned char A;
			};

			struct BGRX5551
			{
				unsigned char B : 5;
				unsigned char G : 5;
				unsigned char R : 5;
				unsigned char X : 1;
			};

			struct BGRX8888
			{
				unsigned char B;
				unsigned char G;
				unsigned char R;
				unsigned char X;
			};

			struct RGBA8881
			{
				unsigned char R;
				unsigned char G;
				unsigned char B;
				unsigned char A : 1;
			};

			struct I8
			{
				unsigned char I;
			};

			struct IA88 : public I8
			{
				unsigned char A;
			};

			struct P8
			{
				unsigned char P;
			};

			struct RGBA16161616
			{
				unsigned short int R;
				unsigned short int G;
				unsigned short int B;
				unsigned short int A;
			};

			struct UVLX8888
			{
				unsigned char U;
				unsigned char V;
				unsigned char L;
				unsigned char X;
			};

			struct UVWQ8888
			{
				unsigned char U;
				unsigned char V;
				unsigned char W;
				unsigned char Q;
			};
		}
	}
}

#endif // !XVTF_PIXELFORMATS_H__