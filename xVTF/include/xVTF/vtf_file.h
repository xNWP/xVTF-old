//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file vtf_file.h
/// @brief defines the VTFFile class, the main workhorse of the library that allows
/// for the manipulation of VTF files.
//==============================================================================

#ifndef XVTF_VTF_FILE_H__
#define XVTF_VTF_FILE_H__

#include "xVTF/bitmapimage.h"
#include "xVTF/exports.h"
#include "xVTF/error.h"
#include "xVTF/flags.h"
#include "xVTF/types.h"

#define XVTF_VTF_MAX_RESOURCES	32
#define XVTF_VTF_MAX_VERSION_MAJOR	7
#define XVTF_VTF_MAX_VERSION_MINOR	5

namespace xvtf
{
	//----------------------------------------------------------------------------------------------------
	/// Generic VTFFile, provides several functions to use VTF files :-)
	//----------------------------------------------------------------------------------------------------
	class VTFFile
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Creates a VTFFile from the path to a valid vtf file. Ensure that you call Free() when done with the object.
		/// @param[in] FilePath				The complete path to the VTFFile.
		/// @param[in] HeaderOnly			Whether or not to only load the header.
		/// @param[out] xvtferrno			The error number if an error occured, can be nullptr.
		/// @return VTFFile*				A pointer to the VTFFile, or nullptr if an error occured. Caller owns object.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI static VTFFile* Alloc(const char* FilePath, bool HeaderOnly = false, ERRORCODE* xvtferrno = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Frees the VTFFile object from memory.
		/// @param[in] obj					The object to free.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI static void Free(VTFFile*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Returns a resource by its index.
		/// @param[in] index						The index of the resource.
		/// @param[out] value						Stores the returned value.
		/// @return bool							True if the value exists.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI bool GetResourceIndex(uint32 index, uint32& value) const;

		//----------------------------------------------------------------------------------------------------
		/// Returns a resource by its integer type.
		/// @param[in] type							The type of the resource to return (hint: use the StockResourceTypes).
		/// @param[out] value						Stores the returned value.
		/// @return bool							True if the value exists.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI bool GetResourceType(uint32 type, uint32& value) const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the image data. Uncompressed formats are first decoded.
		/// @param[in] MipLevel						The mip to load, 0 being the largest.
		/// @param[in] Frame						The frame to load, 0 being the first frame.
		/// @param[in] Face							The face to load (if the image contains any), 0 being the first face.
		/// @param[in] zLevel						The z-depth/slice to load (if any), 0 being the first slice.
		/// @param[out] xvtferrno					The error number if an error occured, can be nullptr.
		/// @return BitmapImage						The returned image. (ensure Free() is called once done with the image)
		//----------------------------------------------------------------------------------------------------
		XVTFAPI BitmapImage* GetImage(uchar MipLevel = 0, uint16 Frame = 0,
			uchar Face = 0, uint16 zLevel = 0, ERRORCODE * xvtferrno = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the resolution at the specified Mipmap level.
		/// @param[out] Res			The returned resolution.
		/// @param[in] MipLevel		The Mipmap level to load.
		/// @param[out] xvtferrno	The error number if an error occured, can be nullptr.
		/// @return bool			True if the resolution was successfully grabbed.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI bool GetResolution(Resolution& res, uchar MipLevel = 0, ERRORCODE* xvtferrno = nullptr) const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the version of the file as an array of size 2. version[0].version[1]
		/// @param[out] version		The version of the VTF file.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI void GetVersion(uint32 version[2]) const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the flags of the file.
		/// @return ImageFlags		The files flags.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI VTF::ImageFlags GetFlags() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the number of frames in the file.
		/// @return uint16			The number of frames the file has.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uint16 GetFrameCount() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the starting frame of the file.
		/// @return uint16			The starting frame of the file.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uint16 GetStartFrame() const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the reflectivity vector (as an array of size 3) of the file.
		/// @param[out] float[3]		The reflectivity of the image.
		/// @return void
		//----------------------------------------------------------------------------------------------------
		XVTFAPI void GetReflectivity(float reflectivity[3]) const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the bumpmap scale.
		/// @return float			The bumpmap scale of the image.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI float GetBumpmapScale() const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the image format.
		/// @return ImageFormat		The image format.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI VTF::ImageFormat GetImageFormat() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the number of mip maps in the file.
		/// @return uchar			The number of mips.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uchar GetMipCount() const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the low resolution image format.
		/// @return ImageFormat			The image format.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI VTF::ImageFormat GetLowResImageFormat() const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the low resolution image resolution.
		/// @param[out] res			The grabbed resolution.
		/// @return bool			True if the res could be grabbed (false if the image doesn't contain a low res image).
		//----------------------------------------------------------------------------------------------------
		XVTFAPI bool GetLowResImageResolution(Resolution& res) const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the depth (# of slices) of the image.
		/// @return uint16			The depth of the image.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uint16 GetDepth() const;

		//----------------------------------------------------------------------------------------------------
		/// Get's the number of resource entries the image has.
		/// @return uint32		The number of resource entries.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uint32 GetResourceCount() const;

		 /////////////////////////////////////////////////////////////////
		//        ____ _   __ ______ ______ ____   _   __ ___     __     //
		//		 /  _// | / //_  __// ____// __ \ / | / //   |   / /     //
		//       / / /  |/ /  / /  / __/  / /_/ //  |/ // /| |  / /      //
		//     _/ / / /|  /  / /  / /___ / _, _// /|  // ___ | / /___    //
		//    /___//_/ |_/  /_/  /_____//_/ |_|/_/ |_//_/  |_|/_____/    //
		//																 //
		 /////////////////////////////////////////////////////////////////

	private:
		VTFFile() = default;
		virtual ~VTFFile() = default;
		VTFFile(const VTFFile&) = delete;
		VTFFile(const VTFFile&&) = delete;
		VTFFile& operator=(const VTFFile&) = delete;
		VTFFile& operator=(const VTFFile&&) = delete;

		class __VTFFileImpl;
		__VTFFileImpl* _impl;
	};
}


#endif // !XVTF_VTF_FILE_H__
