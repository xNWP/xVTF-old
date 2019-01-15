#ifndef XVTF_FILE_H__
#define XVTF_FILE_H__

#include "xVTF/xExports.h"

#include <memory>

#define XVTF_VTF_MAX_RESOURCES	32
#define XVTF_VTF_MAX_VERSION_MAJOR	7
#define XVTF_VTF_MAX_VERSION_MINOR	5

namespace XVTF_NS
{
	namespace ImageFile
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
				POINTSAMPLING			= 0x0001,
				TRILINEARSAMPLING		= 0x0002,
				CLAMPS					= 0x0004,
				CLAMPT					= 0x0008,
				ANISOTROPICSAMPLING		= 0x0010,
				HINTDXT5				= 0x0020,
				NOCOMPRESS				= 0x0040,
				NORMALMAP				= 0x0080,
				NOMIPMAPS				= 0x0100,
				NOLOD					= 0x0200,
				NOMINIMUMMIPMAP			= 0x0400,
				PROCEDURAL				= 0x0800,
				ONEBITALPHA				= 0x1000,
				EIGHTBITALHPA			= 0x2000,
				ENVIRONMENTMAP			= 0x4000,
				RENDERTARGET			= 0x8000,
				DEPTHRENDERTARGET		= 0x10000,
				NODEBUGOVERRIDE			= 0x20000,
				SINGLECOPY				= 0x40000,
				PRESRGB					= 0x80000,
				NODEPTHBUFFER			= 0x800000,
				NICEFILTERED			= 0x1000000,
				CLAMPU					= 0x2000000,
				VERTEXTEXTURE			= 0x4000000,
				SSBUMP					= 0x8000000,
				BORDER					= 0x20000000
			};

			//----------------------------------------------------------------------------------------------------
			/// Stock Resource Types
			//----------------------------------------------------------------------------------------------------
			enum class StockResourceTypes
			{
				LOW_RES_IMAGE	= 0x01,
				HIGH_RES_IMAGE	= 0x30,
				RESOURCE_SHEET	= 0x10
			};

			/* Disable alignment packing on these structs */
#pragma pack(push, 1)

			//----------------------------------------------------------------------------------------------------
			/// Header Resource type (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFResource_r
			{
			public:
				union
				{
					unsigned int eType;
					unsigned char chTypeBytes[4];
				};
				unsigned int resData;
			};

			//----------------------------------------------------------------------------------------------------
			/// Base header for a VTF file (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileBaseHeader_r
			{
			private:
				char FourCC[4]; // VTF\0
			public:
				int version[2];	// version[0].version[1]
				int headerSize;
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.1 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_1_r : public VTFFileBaseHeader_r
			{
			public:
				unsigned short width;
				unsigned short height;
				ImageFlags flags;
				unsigned short numFrames;
				unsigned short startFrame;
			private:
				char pad1[4];
			public:
				float reflectivity[3];
			private:
				char pad2[4];
			public:
				float bumpScale;
				ImageFormat imageFormat;
				unsigned char numMipLevels;
				ImageFormat lowResImageFormat;
				unsigned char lowResImageWidth;
				unsigned char lowResImageHeight;
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.2 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_2_r : public VTFFileHeader_7_1_r
			{
			public:
				unsigned short depth;
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.3 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_3_r : public VTFFileHeader_7_2_r
			{
			private:
				char pad4[3];
			public:
				unsigned int numResources;
			private:
				char pad5[8]; // Pad to 80 Bytes (16 byte aligned)
			public:
				VTFResource_r resources[XVTF_VTF_MAX_RESOURCES];
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.4 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_4_r : public VTFFileHeader_7_3_r
			{
				// No Extensions
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.5 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_5_r : public VTFFileHeader_7_4_r
			{
				// No Extensions
			};

			//----------------------------------------------------------------------------------------------------
			/// General VTF File Header Struct (Unaligned)
			//----------------------------------------------------------------------------------------------------
			typedef VTFFileHeader_7_5_r VTFFileHeader_r;

#pragma pack(pop)

			//----------------------------------------------------------------------------------------------------
			/// General VTF File Header Struct (Aligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader : public VTFFileHeader_r
			{
				// No Extensions
			};

		}

		class VTFFile
		{
		public:
			//----------------------------------------------------------------------------------------------------
			/// Create a VTFFile from a raw unaligned header.
			/// @param[in] std::unique_ptr<VTF::VTFFileHeader_r> header				The unaligned header to create
			///																		the VTFFile from.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI VTFFile(std::unique_ptr<VTF::VTFFileHeader_r> header);

			//----------------------------------------------------------------------------------------------------
			/// Returns the header for the VTF file.
			/// @return std::shared_ptr<VTFFileHeader>				The aligned header struct.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI std::shared_ptr<VTF::VTFFileHeader> GetHeader();

		private:
			std::shared_ptr<VTF::VTFFileHeader_r> _headerRaw;
			std::shared_ptr<VTF::VTFFileHeader> _headerAligned;
		};
	}
}


#endif // !XVTF_FILE_H__
