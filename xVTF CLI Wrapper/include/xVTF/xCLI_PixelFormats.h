#ifndef XVTF_CLI_PIXELFORMATS_H__
#define XVTF_CLI_PIXELFORMATS_H__

namespace xvtf
{
	namespace CLI
	{
		namespace Bitmap
		{
			namespace PixelFormats
			{
				public value struct RGB888
				{
					unsigned char R;
					unsigned char G;
					unsigned char B;
				};

				public value struct RGBA8888
				{
					unsigned char R;
					unsigned char G;
					unsigned char B;
					unsigned char A;
				};

				public value struct ABGR8888
				{
					unsigned char A;
					unsigned char B;
					unsigned char G;
					unsigned char R;
				};

				public value struct BGR888
				{
					unsigned char B;
					unsigned char G;
					unsigned char R;
				};

				public value struct RGB565
				{
					unsigned char R : 5;
					unsigned char G : 6;
					unsigned char B : 5;
				};

				public value struct BGR565
				{
					unsigned char B : 5;
					unsigned char G : 6;
					unsigned char R : 5;
				};

				public value struct A8
				{
					unsigned char A;
				};

				public value struct UV88
				{
					unsigned char U;
					unsigned char V;
				};

				public value struct ARGB8888
				{
					unsigned char A;
					unsigned char R;
					unsigned char G;
					unsigned char B;
				};

				public value struct BGRA4444
				{
					unsigned char B : 4;
					unsigned char G : 4;
					unsigned char R : 4;
					unsigned char A : 4;
				};

				public value struct BGRA5551
				{
					unsigned char B : 5;
					unsigned char G : 5;
					unsigned char R : 5;
					unsigned char A : 1;
				};

				public value struct BGRA8888
				{
					unsigned char B;
					unsigned char G;
					unsigned char R;
					unsigned char A;
				};

				public value struct BGRX5551
				{
					unsigned char B : 5;
					unsigned char G : 5;
					unsigned char R : 5;
					unsigned char X : 1;
				};

				public value struct BGRX8888
				{
					unsigned char B;
					unsigned char G;
					unsigned char R;
					unsigned char X;
				};

				public value struct RGBA8881
				{
					unsigned char R;
					unsigned char G;
					unsigned char B;
					unsigned char A : 1;
				};

				public value struct I8
				{
					unsigned char I;
				};

				public value struct IA88
				{
					unsigned char I;
					unsigned char A;
				};

				public value struct P8
				{
					unsigned char P;
				};

				public value struct RGBA16161616
				{
					unsigned short int R;
					unsigned short int G;
					unsigned short int B;
					unsigned short int A;
				};

				public value struct UVLX8888
				{
					unsigned char U;
					unsigned char V;
					unsigned char L;
					unsigned char X;
				};

				public value struct UVWQ8888
				{
					unsigned char U;
					unsigned char V;
					unsigned char W;
					unsigned char Q;
				};
			}
		}
	}
}

#endif // !XVTF_CLI_PIXELFORMATS_H__
