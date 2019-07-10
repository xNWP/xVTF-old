//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file bitmapimage.h
/// @brief defines the bitmapimage class which is used for returning image buffers.
//==============================================================================

#ifndef XVTF_BITMAPIMAGE_H__
#define XVTF_BITMAPIMAGE_H__

#include "xVTF/exports.h"
#include "xVTF/types.h"

namespace xvtf
{
	struct Resolution
	{
		uint16 Width, Height;
	};

	//----------------------------------------------------------------------------------------------------
	/// A generic BitmapImage class.
	//----------------------------------------------------------------------------------------------------
	class BitmapImage
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Allocates a new BitmapImage. Ensure that you call BitmapImage::Free() when you are done with the object.
		/// @param[in] buffer		A pointer to the buffer that contains the image data.
		/// @param[in] size			The number of pixels that this buffer contains.
		/// @param[in] pixel_size	The size in bytes that each pixel occupies.
		/// @return BitmapImage*	A pointer to the created object.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI static BitmapImage* Alloc(const void* buffer, addressable size, uchar pixel_size);

		//----------------------------------------------------------------------------------------------------
		/// Frees the supplied BitmapImage object.
		/// @param[in] obj			The object to free.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI static void Free(BitmapImage*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Returns a pointer to the pixel at index.
		/// @param[in] index			The index of the pixel to return.
		/// @return void*				The retrieved pixel, or nullptr if the index is out of range.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI void* operator[](addressable index);

		//----------------------------------------------------------------------------------------------------
		/// Identical to operator[]. Provided to give clearer syntax with pointers. Returns a pointer to the pixel at index.
		/// @param[in] index			The index of the pixel to return.
		/// @return void*				The retrieved pixel, or nullptr if the index is out of range.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI void* at(addressable index);

		//----------------------------------------------------------------------------------------------------
		/// Returns the number of pixels contained in the image.
		/// @return unsigned int		The number of pixels in the image.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI addressable GetPixelCount() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the size in bytes that each pixel occupies.
		/// @return unsigned short		The size in bytes of each pixel.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI uchar GetPixelSize() const;


		 /////////////////////////////////////////////////////////////////
		//        ____ _   __ ______ ______ ____   _   __ ___     __     //
		//		 /  _// | / //_  __// ____// __ \ / | / //   |   / /     //
		//       / / /  |/ /  / /  / __/  / /_/ //  |/ // /| |  / /      //
		//     _/ / / /|  /  / /  / /___ / _, _// /|  // ___ | / /___    //
		//    /___//_/ |_/  /_/  /_____//_/ |_|/_/ |_//_/  |_|/_____/    //
		//																 //
		 /////////////////////////////////////////////////////////////////

	private:
		BitmapImage() = default;
		virtual ~BitmapImage() = default;
		BitmapImage(const BitmapImage&) = delete;
		BitmapImage(const BitmapImage&&) = delete;
		BitmapImage& operator=(const BitmapImage&) = delete;
		BitmapImage& operator=(const BitmapImage&&) = delete;

		class __BitmapImageImpl;
		__BitmapImageImpl* _impl;
	};
}

#endif // !XVTF_BITMAPIMAGE_H__
