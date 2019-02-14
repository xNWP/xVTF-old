#ifndef XVTF_FILE_H__
#define XVTF_FILE_H__

#include "xVTF/xCodecs.h"
#include "xVTF/xExports.h"

#include <stdexcept>
#include <memory>

#define XVTF_VTF_MAX_RESOURCES	32
#define XVTF_VTF_MAX_VERSION_MAJOR	7
#define XVTF_VTF_MAX_VERSION_MINOR	5

namespace XVTF_NS
{
	namespace ImageFile
	{
		struct Resolution
		{
			unsigned int Width, Height;
		};

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
			/// Provides the Bytes Per Pixel values for each of the ImageFormat (s).
			//----------------------------------------------------------------------------------------------------
			const float ImageFormatBPP[] =
			{ 4.0f, 4.0f, 3.0f, 3.0f, 2.0f, 1.0f, 2.0f,
			  1.0f, 1.0f, 3.0f, 3.0f, 4.0f, 4.0f, 0.5f,
			  1.0f, 1.0f, 4.0f, 2.0f, 2.0f, 2.0f, 0.5f,
			  2.0f, 2.0f, 4.0f, 8.0f, 8.0f, 4.0f };

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
				RESOURCE_SHEET	= 0x10,
				CRC				= 0x02435243
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
			struct VTFFileHeader_7_2_r : public VTFFileBaseHeader_r
			{
			public:
				VTFFileHeader_7_2_r() : depth(1) { }
				unsigned short depth;
			};

			//----------------------------------------------------------------------------------------------------
			/// Version 7.3 Header Extensions (Unaligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFFileHeader_7_3_r : public VTFFileHeader_7_2_r
			{
			public:
				VTFFileHeader_7_3_r() : numResources(0) { }
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

			//----------------------------------------------------------------------------------------------------
			/// General VTF Resource Struct (Aligned)
			//----------------------------------------------------------------------------------------------------
			struct VTFResource : public VTFResource_r
			{
				VTFResource(const VTFResource_r &rhs)
				{
					this->eType = rhs.eType;
					this->resData = rhs.resData;
				}
			};
		}

		class VTFFile
		{
		public:
			//----------------------------------------------------------------------------------------------------
			/// Creates a VTFFile from the path to a valid VTFFile.
			/// @param[in] FilePath				The complete path to the VTFFile.
			/// @param[in] HeaderOnly			Whether or not to only load the header.
			/// @throws std::runtime_error		If the file could not be open for some reason, or is corrupted in some way.
			/// @throws std::invalid_argument	If the file is a version that this library does not support.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI VTFFile(const char* const FilePath, const bool& HeaderOnly);

			XVTFAPI ~VTFFile();

			//----------------------------------------------------------------------------------------------------
			/// Returns the header for the VTF file.
			/// @return std::shared_ptr<VTFFileHeader>				The aligned header struct.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI std::shared_ptr<VTF::VTFFileHeader> GetHeader();

			//----------------------------------------------------------------------------------------------------
			/// Returns a resource by its integer type.
			/// @param[in] type							The type of the resource to return (hint: use the StockResourceTypes).
			/// @return VTFResource						A unique pointer to the grabbed resource, or nullptr if it doesn't exist.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI std::unique_ptr<XVTF_NS::ImageFile::VTF::VTFResource> GetResourceType(const unsigned int& type);

			//----------------------------------------------------------------------------------------------------
			/// Returns the decompressed image data.
			/// @param[in] MipLevel						The mip to load, 0 being the largest.
			/// @param[in] Frame						The frame to load, 0 being the first frame.
			/// @param[in] Face							The face to load (if the image contains any).
			/// @param[in] zLevel						The z-depth/slice to load (if any).
			/// @returns T*								A pointer to the array of data.
			/// @throws std::out_of_range				If requested image does not exist for this file.
			//----------------------------------------------------------------------------------------------------
			template <typename T> T* GetImage(const unsigned int& MipLevel = 0, const unsigned int& Frame = 0,
				const unsigned int& Face = 0, const unsigned int& zLevel = 0);

			//----------------------------------------------------------------------------------------------------
			/// Returns an array of all the resolutions contained in the VTF : 0 being the largest MipMap.
			/// @return Resolution*		The resolution array.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI const Resolution* GetResolutions() const;

		private:
			std::shared_ptr<VTF::VTFFileHeader_r> _headerRaw;
			std::shared_ptr<VTF::VTFFileHeader> _headerAligned;
			Resolution* _mipMapResolutions;
			void* _highResData;
			void* _lowResData;
			float _texelSize;
			void** _decodedHighResData;
		};

		template XVTFAPI void* VTFFile::GetImage(const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
		template XVTFAPI Codec::RGB888* VTFFile::GetImage(const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
		template XVTFAPI Codec::RGBA8888* VTFFile::GetImage(const unsigned int&, const unsigned int&, const unsigned int&, const unsigned int&);
	}
}


#endif // !XVTF_FILE_H__
