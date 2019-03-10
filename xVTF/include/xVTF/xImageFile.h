#ifndef XVTF_IMAGEFILE_H__
#define XVTF_IMAGEFILE_H__

#include "xVTF/xMacros.h"

#include <stdexcept>

namespace XVTF_NS
{
	namespace Bitmap
	{
		struct Resolution
		{
			unsigned int Width, Height;
		};

		//----------------------------------------------------------------------------------------------------
		/// A generic ImageFile class. Whenever using this class it is important
		/// to call Free() whenever you are done with it to avoid memory leaks.
		//----------------------------------------------------------------------------------------------------
		class BitmapImage
		{
		private:
			BitmapImage();
			BitmapImage(const BitmapImage&);

		public:
			//----------------------------------------------------------------------------------------------------
			/// Construct an BitmapImage from existing data.
			/// @param[in] data			A pointer to the start of the image data.
			/// @param[in] size			The number of pixels in the image data.
			/// @param[in] pixel_size	The size of an individual pixel in bytes.
			/// @param[in] owns_data	If true, the ImageFile object will free the memory pointed to by data during deconstruction.
			//----------------------------------------------------------------------------------------------------
			BitmapImage(void* data, const unsigned int& size, const unsigned short& pixel_size, const bool& owns_data);

			~BitmapImage();

			//----------------------------------------------------------------------------------------------------
			/// Returns a pointer to the pixel at index.
			/// @param[in] index			The index of the pixel to return.
			/// @return void*				The retrieved pixel.
			/// @throws std::out_of_range	when the supplied index exceeds the number of pixels.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI void* operator[](const unsigned int& index);

		private:
			void* _data;
			unsigned int _size;
			unsigned short _psize;
			bool _managed;
		};
	}
}

#endif // !XVTF_IMAGEFILE_H__
