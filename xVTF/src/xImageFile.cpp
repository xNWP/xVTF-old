#include "xVTF/xImageFile.h"

#include <cmath>
#include <stdexcept>

class xvtf::Bitmap::BitmapImage::__BitmapImageImpl
{
public:
	__BitmapImageImpl(void* data, const unsigned int size, const unsigned short pixel_size);
	virtual ~__BitmapImageImpl();

	void* operator[](unsigned int index);
	unsigned int GetPixelCount() const;
	unsigned short GetPixelSize() const;

private:
	void* _data;
	unsigned int _size;
	unsigned short _psize;
};

xvtf::Bitmap::BitmapImage::__BitmapImageImpl::__BitmapImageImpl(void* data, const unsigned int size, const unsigned short pixel_size)
{
	this->_data = data;
	this->_size = size;
	this->_psize = pixel_size;
}

xvtf::Bitmap::BitmapImage::__BitmapImageImpl::~__BitmapImageImpl()
{
	delete[] this->_data;
}

void* xvtf::Bitmap::BitmapImage::__BitmapImageImpl::operator[](unsigned int index)
{
	if (index >= _size)
		return nullptr;

	return (void*)((char*)_data + (unsigned int)std::round(index * _psize));
}

unsigned int xvtf::Bitmap::BitmapImage::__BitmapImageImpl::GetPixelCount() const
{
	return this->_size;
}

unsigned short xvtf::Bitmap::BitmapImage::__BitmapImageImpl::GetPixelSize() const
{
	return this->_psize;
}

xvtf::Bitmap::BitmapImage* xvtf::Bitmap::BitmapImage::Alloc(void* buffer, const unsigned int size, const unsigned short pixel_size)
{
	BitmapImage* r = new BitmapImage();
	r->_impl = new __BitmapImageImpl(buffer, size, pixel_size);
	return r;
}

void xvtf::Bitmap::BitmapImage::Free(BitmapImage*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}

void* xvtf::Bitmap::BitmapImage::operator[](unsigned int index)
{
	return (*this->_impl)[index];
}

void* xvtf::Bitmap::BitmapImage::at(unsigned int index)
{
	return (*this->_impl)[index];
}

unsigned int xvtf::Bitmap::BitmapImage::GetPixelCount() const
{
	return this->_impl->GetPixelCount();
}

unsigned short xvtf::Bitmap::BitmapImage::GetPixelSize() const
{
	return this->_impl->GetPixelSize();
}