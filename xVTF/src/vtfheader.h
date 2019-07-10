//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file vtfheader.h
/// @brief defines various structs contained within VTF files.
//==============================================================================

#ifndef XVTF_VTF_HEADER_H__
#define XVTF_VTF_HEADER_H__

#include "xVTF/types.h"

namespace xvtf
{
	namespace VTF
	{
		//----------------------------------------------------------------------------------------------------
		/// General VTF Resource Struct
		//----------------------------------------------------------------------------------------------------
		struct VTFResource
		{
			union
			{
				uint32 eType;
				uchar chTypeBytes[4];
			};
			uint32 resData;
		};

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
			uint32 version[2];	// version[0].version[1]
			uint32 headerSize;
			uint16 width;
			uint16 height;
			ImageFlags flags;
			uint16 numFrames;
			uint16 startFrame;
		private:
			uchar pad1[4];
		public:
			float reflectivity[3];
		private:
			uchar pad2[4];
		public:
			float bumpScale;
			ImageFormat imageFormat;
			uchar numMipLevels;
			ImageFormat lowResImageFormat;
			uchar lowResImageWidth;
			uchar lowResImageHeight;
		};

		//----------------------------------------------------------------------------------------------------
		/// Version 7.2 Header Extensions (Unaligned)
		//----------------------------------------------------------------------------------------------------
		struct VTFFileHeader_7_2_r : public VTFFileBaseHeader_r
		{
		public:
			uint16 depth;
		};

		//----------------------------------------------------------------------------------------------------
		/// Version 7.3 Header Extensions (Unaligned)
		//----------------------------------------------------------------------------------------------------
		struct VTFFileHeader_7_3_r : public VTFFileHeader_7_2_r
		{
		private:
			uchar pad4[3];
		public:
			uint32 numResources;
		private:
			uchar pad5[8]; // Pad to 80 Bytes (16 byte aligned)
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

		//----------------------------------------------------------------------------------------------------
		/// General VTF File Header Struct (Unaligned)
		//----------------------------------------------------------------------------------------------------
		struct VTFFileHeader : public VTFFileHeader_r
		{
			// No Extensions
		};

#pragma pack(pop)
	}
}

#endif // !XVTF_VTF_HEADER_H__
