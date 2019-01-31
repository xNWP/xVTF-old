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

	this->_impl = XVTF_NS::IO::VTFReader::Open(static_cast<char*>(PtrToNative.ToPointer()), HeaderOnly).release();
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
	return gcnew XVTF_NS::CLI::ImageFile::VTF::VTFResource(this->_header->resources[index]);
}