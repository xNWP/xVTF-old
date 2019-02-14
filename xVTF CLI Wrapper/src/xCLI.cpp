#include "xVTF/xCLI.h"

XVTF_NS::CLI::ImageFile::VTF::VTFResource::VTFResource(const XVTF_NS::ImageFile::VTF::VTFResource_r &res)
{
	this->_raw = new XVTF_NS::ImageFile::VTF::VTFResource_r;
	*this->_raw = res;
}

XVTF_NS::CLI::ImageFile::VTF::VTFResource::~VTFResource()
{
	this->!VTFResource();
}

XVTF_NS::CLI::ImageFile::VTF::VTFResource::!VTFResource()
{
	delete this->_raw;
}

unsigned int XVTF_NS::CLI::ImageFile::VTF::VTFResource::GetIntType()
{
	return this->_raw->eType;
}

System::String^ XVTF_NS::CLI::ImageFile::VTF::VTFResource::GetCharCode()
{
	return gcnew System::String(reinterpret_cast<const char*>(this->_raw->chTypeBytes), 0, 4);
}

unsigned int XVTF_NS::CLI::ImageFile::VTF::VTFResource::GetResData()
{
	return this->_raw->resData;
}

XVTF_NS::CLI::ImageFile::VTFFile::VTFFile(const System::String^ filename, const System::Boolean HeaderOnly)
{
	using namespace System::Runtime::InteropServices;
	System::String^ loc = const_cast<System::String^>(filename);
	System::IntPtr PtrToNative = Marshal::StringToHGlobalAnsi(loc);

	this->_impl = new XVTF_NS::ImageFile::VTFFile(static_cast<const char*>(PtrToNative.ToPointer()), HeaderOnly);
	this->_header = this->_impl->GetHeader().get();
	Marshal::FreeHGlobal(PtrToNative);
}

XVTF_NS::CLI::ImageFile::VTFFile::~VTFFile()
{
	this->!VTFFile();
}

XVTF_NS::CLI::ImageFile::VTFFile::!VTFFile()
{
	delete this->_impl;
}

array<unsigned int>^ XVTF_NS::CLI::ImageFile::VTFFile::GetVersion()
{
	array<unsigned int>^ rval = gcnew array<unsigned int>(2);
	rval[0] = this->_header->version[0];
	rval[1] = this->_header->version[1];
	return rval;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetWidth()
{
	return this->_header->width;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetHeight()
{
	return this->_header->height;
}

unsigned int XVTF_NS::CLI::ImageFile::VTFFile::GetFlags()
{
	return static_cast<unsigned int>(this->_header->flags);
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetNumberOfFrames()
{
	return this->_header->numFrames;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetStartFrame()
{
	return this->_header->startFrame;
}

array<float>^ XVTF_NS::CLI::ImageFile::VTFFile::GetReflectivity()
{
	array<float>^ rval = gcnew array<float>(3);
	rval[0] = this->_header->reflectivity[0];
	rval[1] = this->_header->reflectivity[1];
	rval[2] = this->_header->reflectivity[2];
	return rval;
}

float XVTF_NS::CLI::ImageFile::VTFFile::GetBumpScale()
{
	return this->_header->bumpScale;
}

unsigned int XVTF_NS::CLI::ImageFile::VTFFile::GetFormat()
{
	return static_cast<unsigned int>(this->_header->imageFormat);
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetNumberOfMipLevels()
{
	return this->_header->numMipLevels;
}

unsigned int XVTF_NS::CLI::ImageFile::VTFFile::GetLowResFormat()
{
	return static_cast<unsigned int>(this->_header->lowResImageFormat);
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetLowResWidth()
{
	return this->_header->lowResImageWidth;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetLowResHeight()
{
	return this->_header->lowResImageHeight;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetDepth()
{
	return this->_header->depth;
}

unsigned short XVTF_NS::CLI::ImageFile::VTFFile::GetNumberOfResources()
{
	return this->_header->numResources;
}

XVTF_NS::CLI::ImageFile::VTF::VTFResource^ XVTF_NS::CLI::ImageFile::VTFFile::GetResource(const int index)
{
	// Out of range case
	if (index > this->GetNumberOfResources())
		return nullptr;

	return gcnew XVTF_NS::CLI::ImageFile::VTF::VTFResource(this->_header->resources[index]);
}

unsigned int XVTF_NS::CLI::ImageFile::VTFFile::GetCRC()
{
	auto CRC = this->_impl->GetResourceType(static_cast<unsigned int>(XVTF_NS::CLI::ImageFile::VTF::StockResourceTypes::CRC));
	return CRC == nullptr ? 0 : CRC->resData;
}

XVTF_NS::CLI::ImageFile::VTF::VTFResource^ XVTF_NS::CLI::ImageFile::VTFFile::GetResourceType(const unsigned int type)
{
	for (unsigned int i = 0; i < this->GetNumberOfResources(); i++)
	{
		if (this->GetResource(i)->GetIntType() == type)
			return this->GetResource(i);
	}
	// null case
	return nullptr;
}

array<XVTF_NS::CLI::Codec::RGB888>^ XVTF_NS::CLI::ImageFile::VTFFile::GetImageRGB888(const unsigned int MipLevel, const unsigned int Frame,
	const unsigned int Face, const unsigned int zLevel)
{
	void* NativeData = this->_impl->GetImage<void>(MipLevel, Frame, Face, zLevel);
	auto RES = GetResolutions()[MipLevel];
	array<Codec::RGB888>^ rval = gcnew array<Codec::RGB888>(RES.Width * RES.Height);

	for (unsigned int i = 0; i < RES.Width * RES.Height; i++)
	{
		rval[i].R = *((char*)NativeData);
		rval[i].G = *((char*)NativeData + 1);
		rval[i].B = *((char*)NativeData + 2);
		NativeData = (void*)((char*)NativeData + 3);
	}

	return rval;
}

array<XVTF_NS::CLI::ImageFile::Resolution>^ XVTF_NS::CLI::ImageFile::VTFFile::GetResolutions()
{
	const XVTF_NS::ImageFile::Resolution* RES = this->_impl->GetResolutions();
	array<XVTF_NS::CLI::ImageFile::Resolution>^ RVAL = gcnew array<XVTF_NS::CLI::ImageFile::Resolution>(_header->numMipLevels);
	
	for (unsigned int i = 0; i < this->_header->numMipLevels; i++)
	{
		RVAL[i].Width = (RES + i)->Width;
		RVAL[i].Height = (RES + i)->Height;
	}

	return RVAL;
}