//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file bitmapimage.cpp
/// @brief Implements the bitmap image class which is used to handle image buffers.
//==============================================================================

#include "xVTF/bitmapimage.h"

#include <cmath>
#include <string.h>

class xvtf::BitmapImage::__BitmapImageImpl
{
public:
	__BitmapImageImpl(const void* data, addressable size, uchar pixel_size);
	virtual ~__BitmapImageImpl();

	void* operator[](addressable index);
	addressable GetPixelCount() const;
	uchar GetPixelSize() const;

private:
	void* _data;
	addressable _size;
	uchar _psize;
};

xvtf::BitmapImage::__BitmapImageImpl::__BitmapImageImpl(const void* data, addressable size, uchar pixel_size)
{
	this->_data = malloc(pixel_size * size);
	memcpy(this->_data, data, pixel_size * size);
	this->_size = size;
	this->_psize = pixel_size;
}

xvtf::BitmapImage::__BitmapImageImpl::~__BitmapImageImpl()
{
	free(this->_data);
}

void* xvtf::BitmapImage::__BitmapImageImpl::operator[](addressable index)
{
	if (index >= _size)
		return nullptr;

	return (void*)((uchar*)_data + index * _psize);
}

addressable xvtf::BitmapImage::__BitmapImageImpl::GetPixelCount() const
{
	return this->_size;
}

uchar xvtf::BitmapImage::__BitmapImageImpl::GetPixelSize() const
{
	return this->_psize;
}

xvtf::BitmapImage* xvtf::BitmapImage::Alloc(const void* buffer, addressable size, uchar pixel_size)
{
	BitmapImage* r = new BitmapImage();
	r->_impl = new __BitmapImageImpl(buffer, size, pixel_size);
	return r;
}

void xvtf::BitmapImage::Free(BitmapImage*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}

void* xvtf::BitmapImage::operator[](addressable index)
{
	return (*this->_impl)[index];
}

void* xvtf::BitmapImage::at(addressable index)
{
	return (*this->_impl)[index];
}

addressable xvtf::BitmapImage::GetPixelCount() const
{
	return this->_impl->GetPixelCount();
}

uchar xvtf::BitmapImage::GetPixelSize() const
{
	return this->_impl->GetPixelSize();
}