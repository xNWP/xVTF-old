#include "xVTF/xImageFile.h"

XVTF_NS::BitmapImage::BitmapImage(void* data, const unsigned int& size, const unsigned short& pixel_size, const bool& owns_data)
{
	this->_data = data;
	this->_size = size;
	this->_psize = pixel_size;
	this->_managed = owns_data;
}

XVTF_NS::BitmapImage::~BitmapImage()
{
	if (_managed)
	{
		delete[] _data;
	}
}

void* XVTF_NS::BitmapImage::operator[](const unsigned int& index)
{
	if (index >= _size)
		throw std::out_of_range("Index out of range.");

	return (void*)((char*)_data + index * _psize);
}