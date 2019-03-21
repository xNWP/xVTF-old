#ifndef XVTF_IMAGEFILE_H__
#define XVTF_IMAGEFILE_H__

#include "xVTF/xExports.h"
#include "xVTF/xMacros.h"

namespace XVTF_NS
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
			/// @param[in] buffer		A pointer the the buffer that contains the image data.
			/// @param[in] size			The number of pixels that this buffer contains.
			/// @param[in] pixel_size	The size in bytes that each pixel occupies.
			/// @param[in] owns_data	If true, the image data for this object will be freed when Free() is called.
			/// @return BitmapImage*	A pointer to the created object.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI static BitmapImage* Alloc(void* buffer, const unsigned int& size,
				const unsigned short& pixel_size, const bool& owns_data);

			//----------------------------------------------------------------------------------------------------
			/// Frees the supplied BitmapImage object.
			/// @param[in] obj			The object to free.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI static void Free(BitmapImage*& obj);

			//----------------------------------------------------------------------------------------------------
			/// Returns a pointer to the pixel at index.
			/// @param[in] index			The index of the pixel to return.
			/// @return void*				The retrieved pixel.
			/// @throws std::out_of_range	when the supplied index exceeds the number of pixels.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI void* operator[](const unsigned int& index);

			//----------------------------------------------------------------------------------------------------
			/// Returns the bytes per pixel of the given image.
			/// @return unsigned short		The bytes per pixel.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI unsigned short GetBytesPerPixel() const;





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
