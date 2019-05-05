#include "xVTF/xCLI.h"

xvtf::CLI::ImageFile::VTF::VTFResource::VTFResource(const xvtf::Bitmap::VTF::VTFResource_r &res)
{
	this->_raw = new xvtf::Bitmap::VTF::VTFResource_r;
	*this->_raw = res;
}

xvtf::CLI::ImageFile::VTF::VTFResource::~VTFResource()
{
	this->!VTFResource();
}

xvtf::CLI::ImageFile::VTF::VTFResource::!VTFResource()
{
	delete this->_raw;
}

unsigned int xvtf::CLI::ImageFile::VTF::VTFResource::GetIntType()
{
	return this->_raw->eType;
}

System::String^ xvtf::CLI::ImageFile::VTF::VTFResource::GetCharCode()
{
	return gcnew System::String(reinterpret_cast<const char*>(this->_raw->chTypeBytes), 0, 4);
}

unsigned int xvtf::CLI::ImageFile::VTF::VTFResource::GetResData()
{
	return this->_raw->resData;
}

xvtf::CLI::ImageFile::VTFFile::VTFFile(const System::String^ filename, const System::Boolean HeaderOnly)
{
	using namespace System::Runtime::InteropServices;
	System::String^ loc = const_cast<System::String^>(filename);
	System::IntPtr PtrToNative = Marshal::StringToHGlobalAnsi(loc);

	this->_impl =  xvtf::Bitmap::VTFFile::Alloc(static_cast<const char*>(PtrToNative.ToPointer()), HeaderOnly);
	this->_header = this->_impl->GetHeader();
	Marshal::FreeHGlobal(PtrToNative);
}

xvtf::CLI::ImageFile::VTFFile::~VTFFile()
{
	this->!VTFFile();
}

xvtf::CLI::ImageFile::VTFFile::!VTFFile()
{
	pin_ptr<xvtf::Bitmap::VTFFile*> ptr = &this->_impl;
	auto rawPtr = (xvtf::Bitmap::VTFFile*)ptr;
	xvtf::Bitmap::VTFFile::Free(rawPtr);
}

array<unsigned int>^ xvtf::CLI::ImageFile::VTFFile::GetVersion()
{
	array<unsigned int>^ rval = gcnew array<unsigned int>(2);
	rval[0] = this->_header->version[0];
	rval[1] = this->_header->version[1];
	return rval;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetWidth()
{
	return this->_header->width;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetHeight()
{
	return this->_header->height;
}

unsigned int xvtf::CLI::ImageFile::VTFFile::GetFlags()
{
	return static_cast<unsigned int>(this->_header->flags);
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetNumberOfFrames()
{
	return this->_header->numFrames;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetStartFrame()
{
	return this->_header->startFrame;
}

array<float>^ xvtf::CLI::ImageFile::VTFFile::GetReflectivity()
{
	array<float>^ rval = gcnew array<float>(3);
	rval[0] = this->_header->reflectivity[0];
	rval[1] = this->_header->reflectivity[1];
	rval[2] = this->_header->reflectivity[2];
	return rval;
}

float xvtf::CLI::ImageFile::VTFFile::GetBumpScale()
{
	return this->_header->bumpScale;
}

xvtf::CLI::ImageFile::VTF::ImageFormat xvtf::CLI::ImageFile::VTFFile::GetFormat()
{
	return static_cast<xvtf::CLI::ImageFile::VTF::ImageFormat>(this->_header->imageFormat);
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetNumberOfMipLevels()
{
	return this->_header->numMipLevels;
}

xvtf::CLI::ImageFile::VTF::ImageFormat xvtf::CLI::ImageFile::VTFFile::GetLowResFormat()
{
	return static_cast<xvtf::CLI::ImageFile::VTF::ImageFormat>(this->_header->lowResImageFormat);
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetLowResWidth()
{
	return this->_header->lowResImageWidth;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetLowResHeight()
{
	return this->_header->lowResImageHeight;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetDepth()
{
	return this->_header->depth;
}

unsigned short xvtf::CLI::ImageFile::VTFFile::GetNumberOfResources()
{
	return this->_header->numResources;
}

xvtf::CLI::ImageFile::VTF::VTFResource^ xvtf::CLI::ImageFile::VTFFile::GetResource(const int index)
{
	// Out of range case
	if (index > this->GetNumberOfResources())
		return nullptr;

	return gcnew xvtf::CLI::ImageFile::VTF::VTFResource(this->_header->resources[index]);
}

unsigned int xvtf::CLI::ImageFile::VTFFile::GetCRC()
{
	auto CRC = this->_impl->GetResourceType(static_cast<unsigned int>(xvtf::CLI::ImageFile::VTF::StockResourceTypes::CRC));
	return CRC == nullptr ? 0 : CRC->resData;
}

xvtf::CLI::ImageFile::VTF::VTFResource^ xvtf::CLI::ImageFile::VTFFile::GetResourceType(const unsigned int type)
{
	for (unsigned int i = 0; i < this->GetNumberOfResources(); i++)
	{
		if (this->GetResource(i)->GetIntType() == type)
			return this->GetResource(i);
	}
	// null case
	return nullptr;
}

array<xvtf::CLI::Codec::RGB888>^ xvtf::CLI::ImageFile::VTFFile::GetImageRGB888(const unsigned int MipLevel, const unsigned int Frame,
	const unsigned int Face, const unsigned int zLevel)
{
	auto NativeData = this->_impl->GetImage(MipLevel, Frame, Face, zLevel);
	auto RES = GetResolutions()[MipLevel];
	array<Codec::RGB888>^ rval = gcnew array<Codec::RGB888>(RES.Width * RES.Height);

	for (unsigned int i = 0; i < RES.Width * RES.Height; i++)
	{
		void* pixel = (*NativeData)[i];
		rval[i].R = *((char*)pixel);
		rval[i].G = *((char*)pixel + 1);
		rval[i].B = *((char*)pixel + 2);
	}

	xvtf::Bitmap::BitmapImage::Free(NativeData);

	return rval;
}

array<xvtf::CLI::Codec::RGBA8888>^ xvtf::CLI::ImageFile::VTFFile::GetImageRGBA8888(const unsigned int MipLevel, const unsigned int Frame,
	const unsigned int Face, const unsigned int zLevel)
{
	void* NativeData = this->_impl->GetImage(MipLevel, Frame, Face, zLevel);
	auto RES = GetResolutions()[MipLevel];
	array<Codec::RGBA8888>^ rval = gcnew array<Codec::RGBA8888>(RES.Width * RES.Height);

	for (unsigned int i = 0; i < RES.Width * RES.Height; i++)
	{
		rval[i].R = *((char*)NativeData);
		rval[i].G = *((char*)NativeData + 1);
		rval[i].B = *((char*)NativeData + 2);
		rval[i].A = *((char*)NativeData + 3);
		NativeData = (void*)((char*)NativeData + 4);
	}

	return rval;
}

array<xvtf::CLI::ImageFile::Resolution>^ xvtf::CLI::ImageFile::VTFFile::GetResolutions()
{
	const xvtf::Bitmap::Resolution* RES = this->_impl->GetResolutions();
	array<xvtf::CLI::ImageFile::Resolution>^ RVAL = gcnew array<xvtf::CLI::ImageFile::Resolution>(_header->numMipLevels);

	for (unsigned int i = 0; i < this->_header->numMipLevels; i++)
	{
		RVAL[i].Width = (RES + i)->Width;
		RVAL[i].Height = (RES + i)->Height;
	}

	return RVAL;
}