/*
	Provides the definitions for the CLI wrapper for the xVTF library.
	This file (along with the compiled DLL) allows you to use the library
	with CLI/CLR applications.
*/

#ifndef XVTF_CLI_H__
#define XVTF_CLI_H__

#include "xVTF/xVTF.h"

#define XVTF_NS xvtf

namespace XVTF_NS
{
	namespace CLI
	{
		namespace ImageFile
		{
			namespace VTF
			{
				//----------------------------------------------------------------------------------------------------
				/// Image formats supported by the VTF format
				//----------------------------------------------------------------------------------------------------
				public enum class ImageFormat
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
				public enum class ImageFlags
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
				public enum class StockResourceTypes
				{
					LOW_RES_IMAGE = 0x01,
					HIGH_RES_IMAGE = 0x30,
					RESOURCE_SHEET = 0x10
				};

				public ref class VTFResource
				{
				public:
					VTFResource(const XVTF_NS::ImageFile::VTF::VTFResource_r &res);
					~VTFResource();
					!VTFResource();

					unsigned int GetIntType();
					System::String^ GetCharCode();
					unsigned int GetResData();

				private:
					XVTF_NS::ImageFile::VTF::VTFResource_r*_raw;
				};
			}

			public ref class VTFFile
			{
			public:
				//----------------------------------------------------------------------------------------------------
				/// Generates a VTFFile given the path to a VTF file.
				/// @param[in] filename				The path to the file to load.
				/// @param[in] HeaderOnly			Whether or not to only load the header (useful for probing).
				//----------------------------------------------------------------------------------------------------
				VTFFile(const System::String^ filename, const System::Boolean HeaderOnly);

				~VTFFile();
				!VTFFile();

				array<unsigned int>^ GetVersion();
				unsigned short GetWidth();
				unsigned short GetHeight();
				unsigned int GetFlags();
				unsigned short GetNumberOfFrames();
				unsigned short GetStartFrame();
				array<float>^ GetReflectivity();
				float GetBumpScale();
				unsigned int GetFormat();
				unsigned short GetNumberOfMipLevels();
				unsigned int GetLowResFormat();
				unsigned short GetLowResWidth();
				unsigned short GetLowResHeight();
				unsigned short GetDepth();
				unsigned short GetNumberOfResources();
				VTF::VTFResource^ GetResource(const int index);

			private:
				XVTF_NS::ImageFile::VTF::VTFFileHeader* _header;
				XVTF_NS::ImageFile::VTFFile* _impl;
			};
		}
	}
}

#endif