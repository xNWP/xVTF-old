#ifndef XVTF_STRUCTS_H__
#define XVTF_STRUCTS_H__

#define XVTF_VTF_MAX_RESOURCES	32
#define XVTF_VTF_MAX_VERSION_MAJOR	7
#define XVTF_VTF_MAX_VERSION_MINOR	5

namespace xvtf
{
	namespace Bitmap
	{
		namespace VTF
		{
			//----------------------------------------------------------------------------------------------------
			/// Image formats supported by the VTF format
			//----------------------------------------------------------------------------------------------------
			enum class ImageFormat
			{
				NONE = -1,
				RGBA8888 = 0,
				ABGR8888,
				RGB888,
				BGR888,
				RGB565,
				I8,
				IA88,
				P8,
				A8,
				RGB888_BLUESCREEN,
				BGR888_BLUESCREEN,
				ARGB8888,
				BGRA8888,
				DXT1,
				DXT3,
				DXT5,
				BGRX8888,
				BGR565,
				BGRX5551,
				BGRA4444,
				DXT1_ONEBITALPHA,
				BGRA5551,
				UV88,
				UVWQ8888,
				RGBA16161616F,
				RGBA16161616,
				UVLX8888
			};

			//----------------------------------------------------------------------------------------------------
			/// Various flags that a VTF image may provide
			//----------------------------------------------------------------------------------------------------
			enum class ImageFlags
			{
				POINTSAMPLING = 0x0001,
				TRILINEARSAMPLING = 0x0002,
				CLAMPS = 0x0004,
				CLAMPT = 0x0008,
				ANISOTROPICSAMPLING = 0x0010,
				HINTDXT5 = 0x0020,
				NOCOMPRESS = 0x0040,
				NORMALMAP = 0x0080,
				NOMIPMAPS = 0x0100,
				NOLOD = 0x0200,
				NOMINIMUMMIPMAP = 0x0400,
				PROCEDURAL = 0x0800,
				ONEBITALPHA = 0x1000,
				EIGHTBITALHPA = 0x2000,
				ENVIRONMENTMAP = 0x4000,
				RENDERTARGET = 0x8000,
				DEPTHRENDERTARGET = 0x10000,
				NODEBUGOVERRIDE = 0x20000,
				SINGLECOPY = 0x40000,
				PRESRGB = 0x80000,
				NODEPTHBUFFER = 0x800000,
				NICEFILTERED = 0x1000000,
				CLAMPU = 0x2000000,
				VERTEXTEXTURE = 0x4000000,
				SSBUMP = 0x8000000,
				BORDER = 0x20000000
			};

			//----------------------------------------------------------------------------------------------------
			/// Stock Resource Types
			//----------------------------------------------------------------------------------------------------
			enum class StockResourceTypes
			{
				LOW_RES_IMAGE = 0x01,
				HIGH_RES_IMAGE = 0x30,
				RESOURCE_SHEET = 0x10,
				CRC = 0x02435243
			};
		}

		//----------------------------------------------------------------------------------------------------
		/// General VTF Resource Struct
		//----------------------------------------------------------------------------------------------------
		struct VTFResource
		{
			union
			{
				unsigned int eType;
				unsigned char chTypeBytes[4];
			};
			unsigned int resData;
		};
	}
}

#endif // !XVTF_STRUCTS_H__