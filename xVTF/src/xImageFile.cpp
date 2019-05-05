#include "xVTF/xImageFile.h"

#include <stdexcept>

class xvtf::Bitmap::BitmapImage::__BitmapImageImpl
{
public:
	__BitmapImageImpl(void* data, const unsigned int& size, const unsigned short& pixel_size, const bool& owns_data);

	~__BitmapImageImpl();

	void* operator[](const unsigned int& index);

	unsigned short GetBytesPerPixel() const;

private:
	void* _data;
	unsigned int _size;
	unsigned short _psize;
	bool _managed;
};

xvtf::Bitmap::BitmapImage::__BitmapImageImpl::__BitmapImageImpl(void* data, const unsigned int& size, const unsigned short& pixel_size, const bool& owns_data)
{
	this->_data = data;
	this->_size = size;
	this->_psize = pixel_size;
	this->_managed = owns_data;
}

xvtf::Bitmap::BitmapImage::__BitmapImageImpl::~__BitmapImageImpl()
{
	if (_managed)
	{
		delete[] _data;
	}
}

void* xvtf::Bitmap::BitmapImage::__BitmapImageImpl::operator[](const unsigned int& index)
{
	if (index >= _size)
		throw std::out_of_range("Index out of range.");

	return (void*)((char*)_data + index * _psize);
}

unsigned short xvtf::Bitmap::BitmapImage::__BitmapImageImpl::GetBytesPerPixel() const
{
	return this->_psize;
}

void* xvtf::Bitmap::BitmapImage::operator[](const unsigned int& index)
{
	return (*this->_impl)[index];
}

unsigned short xvtf::Bitmap::BitmapImage::GetBytesPerPixel() const
{
	return this->_impl->GetBytesPerPixel();
}

xvtf::Bitmap::BitmapImage* xvtf::Bitmap::BitmapImage::Alloc(void* buffer, const unsigned int& size,
	const unsigned short& pixel_size, const bool& owns_data)
{
	BitmapImage* r = new BitmapImage();
	r->_impl = new __BitmapImageImpl(buffer, size, pixel_size, owns_data);
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