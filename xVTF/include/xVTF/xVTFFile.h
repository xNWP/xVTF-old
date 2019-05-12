#ifndef XVTF_FILE_H__
#define XVTF_FILE_H__

#include "xVTF/xExports.h"
#include "xVTF/xImageFile.h"
#include "xVTF/xVTFStructs.h"

namespace xvtf
{
	namespace Bitmap
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
			XVTFAPI static VTFFile* Alloc(const char* FilePath, const bool HeaderOnly = false, unsigned int * const & xvtferrno = nullptr);

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
			XVTFAPI bool GetResourceIndex(const unsigned int index, unsigned int& value) const;

			//----------------------------------------------------------------------------------------------------
			/// Returns a resource by its integer type.
			/// @param[in] type							The type of the resource to return (hint: use the StockResourceTypes).
			/// @param[out] value						Stores the returned value.
			/// @return bool							True if the value exists.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI bool GetResourceType(const unsigned int type, unsigned int& value) const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the image data. Uncompressed formats are first decoded.
			/// @param[out] bmp							The returned image. (ensure Free() is called once done with the image)
			/// @param[out] xvtferrno					The error number if an error occured, can be nullptr.
			/// @param[in] MipLevel						The mip to load, 0 being the largest.
			/// @param[in] Frame						The frame to load, 0 being the first frame.
			/// @param[in] Face							The face to load (if the image contains any), 0 being the first face.
			/// @param[in] zLevel						The z-depth/slice to load (if any), 0 being the first slice.
			/// @return bool							True if the BitmapImage was grabbed successfully.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI bool GetImage(BitmapImage*& bmp, unsigned int * const & xvtferrno = nullptr,
				const unsigned int MipLevel = 0, const unsigned int Frame = 0,
				const unsigned int Face = 0, const unsigned int zLevel = 0);

			//----------------------------------------------------------------------------------------------------
			/// Returns the resolution at the specified Mipmap level.
			/// @param[out] Res			The returned resolution.
			/// @param[in] MipLevel		The Mipmap level to load.
			/// @param[out] xvtferrno	The error number if an error occured, can be nullptr.
			/// @return bool			True if the resolution was successfully grabbed.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI bool GetResolution(Resolution* const & res, const unsigned int MipLevel = 0, unsigned int * const & xvtferrno = nullptr) const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the version of the file as an array of size 2. version[0].version[1]
			/// @param[out] version		The version of the VTF file.
			/// @return void
			//----------------------------------------------------------------------------------------------------
			XVTFAPI void GetVersion(unsigned int version[2]) const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the flags of the file.
			/// @return unsigned int	The files flags.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned int GetFlags() const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the number of frames in the file.
			/// @return unsigned short			The number of frames the file has.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned short GetFrameCount() const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the starting frame of the file.
			/// @return unsigned short			The starting frame of the file.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned short GetStartFrame() const;

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
			/// @return unsigned int		The image format.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned int GetImageFormat() const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the number of mip maps in the file.
			/// @return unsigned short			The number of mips.
			//----------------------------------------------------------------------------------------------------
			unsigned short GetMipCount() const;

			//----------------------------------------------------------------------------------------------------
			/// Get's the low resolution image format.
			/// @return unsigned int		The image format.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned int GetLowResImageFormat() const;

			//----------------------------------------------------------------------------------------------------
			/// Get's the low resolution image resolution.
			/// @return Resolution			The grabbed resolution.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI Resolution GetLowResImageResolution() const;

			//----------------------------------------------------------------------------------------------------
			/// Get's the depth (# of slices) of the image.
			/// @return unsigned short		The depth of the image.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned short GetDepth() const;

			//----------------------------------------------------------------------------------------------------
			/// Get's the number of resource entries the image has.
			/// @return unsigned int		The number of resource entries.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned int GetResourceCount() const;


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
}


#endif // !XVTF_FILE_H__
