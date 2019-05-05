#ifndef XVTF_VTF_STRUCTS_H__
#define XVTF_VTF_STRUCTS_H__

#include "xVTF/xVTFStructs.h"

namespace xvtf
{
	namespace Bitmap
	{
		/* Disable alignment packing on these structs */
#pragma pack(push, 1)
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
			VTF::ImageFlags flags;
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
			VTF::ImageFormat imageFormat;
			unsigned char numMipLevels;
			VTF::ImageFormat lowResImageFormat;
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
			VTFResource resources[XVTF_VTF_MAX_RESOURCES];
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
}

#endif // !XVTF_VTF_STRUCTS_H__
