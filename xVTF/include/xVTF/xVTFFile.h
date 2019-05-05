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
			/// @return VTFFile*				A pointer to the VTFFile. Caller owns object.
			/// @throws std::runtime_error		If the file could not be opened for some reason, or is corrupted in some way.
			/// @throws std::invalid_argument	If the file is a version that this library does not support.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI static VTFFile* Alloc(const char* FilePath, const bool& HeaderOnly = false);

			//----------------------------------------------------------------------------------------------------
			/// Frees the VTFFile object from memory.
			/// @param[in] obj					The object to free.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI static void Free(VTFFile*& obj);

			//----------------------------------------------------------------------------------------------------
			/// Returns a resource by its index.
			/// @param[in] index						The index of the resource.
			/// @return bool							True if the value exists.
			/// @throws std::out_of_range				If the index is larger than the number of resources.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI VTFResource GetResourceIndex(const unsigned int& index);

			//----------------------------------------------------------------------------------------------------
			/// Returns a resource by its integer type.
			/// @param[in] type							The type of the resource to return (hint: use the StockResourceTypes).
			/// @param[out] value						Stores the returned value.
			/// @return bool							True if the value exists.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI bool GetResourceType(const VTF::StockResourceTypes& type, unsigned int& value);

			//----------------------------------------------------------------------------------------------------
			/// Returns the image data. Uncompressed formats are first decoded.
			/// @param[in] MipLevel						The mip to load, 0 being the largest.
			/// @param[in] Frame						The frame to load, 0 being the first frame.
			/// @param[in] Face							The face to load (if the image contains any), 0 being the first face.
			/// @param[in] zLevel						The z-depth/slice to load (if any), 0 being the first slice.
			/// @return BitmapImage						The BitmapImage.
			/// @throws std::out_of_range				If requested image does not exist for this file.
			/// @throws std::runtime_error				If the requested image could not be returned for any other reason.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI BitmapImage& GetImage(const unsigned int& MipLevel = 0, const unsigned int& Frame = 0,
				const unsigned int& Face = 0, const unsigned int& zLevel = 0);

			//----------------------------------------------------------------------------------------------------
			/// Returns the resolution at the specified Mipmap level.
			/// @param[in] MipLevel		The Mipmap level to load.
			/// @return Resolution		The resolution.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI Resolution GetResolution(const unsigned int& MipLevel) const;







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
