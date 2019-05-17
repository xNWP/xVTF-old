#ifndef XVTF_IMAGEFILE_H__
#define XVTF_IMAGEFILE_H__

#include "xVTF/xExports.h"

namespace xvtf
{
	namespace Bitmap
	{
		struct Resolution
		{
			unsigned int Width, Height;
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
			XVTFAPI static BitmapImage* Alloc(void* buffer, const unsigned int size, const unsigned short pixel_size);

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
			XVTFAPI void* operator[](unsigned int index);

			//----------------------------------------------------------------------------------------------------
			/// Identical to operator[]. Provided to give clearer syntax with pointers. Returns a pointer to the pixel at index.
			/// @param[in] index			The index of the pixel to return.
			/// @return void*				The retrieved pixel, or nullptr if the index is out of range.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI void* at(unsigned int index);

			//----------------------------------------------------------------------------------------------------
			/// Returns the number of pixels contained in the image.
			/// @return unsigned int		The number of pixels in the image.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned int GetPixelCount() const;

			//----------------------------------------------------------------------------------------------------
			/// Returns the size in bytes that each pixel occupies.
			/// @return unsigned short		The size in bytes of each pixel.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned short GetPixelSize() const;


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
}

#endif // !XVTF_IMAGEFILE_H__
