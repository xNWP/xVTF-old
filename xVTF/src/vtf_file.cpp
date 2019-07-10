//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file vtf_file.cpp
/// @brief implements the VTFFile class.
//==============================================================================

#include "xVTF/vtf_file.h"

#include "xVTF/error.h"
#include "xVTF/flags.h"

#include "codecs.h"
#include "error_macro.h"
#include "luts.h"
#include "vtfheader.h"

#include <cmath>
#include <stdio.h>
#include <string.h>
#include <vector>

class xvtf::VTFFile::__VTFFileImpl
{
public:
	__VTFFileImpl(const char* FilePath, bool HeaderOnly, ERRORCODE* xvtferrno);
	virtual ~__VTFFileImpl();

	bool GetResourceIndex(uint32 index, uint32& value) const;
	bool GetResourceType(uint32 type, uint32& value) const;
	BitmapImage* GetImage(uchar MipLevel, uint16 Frame, uchar Face, uint16 zLevel, ERRORCODE* xvtferrno);
	bool GetResolution(Resolution& res, uchar index, ERRORCODE* xvtferrno) const;
	void GetVersion(uint32 version[2]) const;
	VTF::ImageFlags GetFlags() const;
	uint16 GetFrameCount() const;
	uint16 GetStartFrame() const;
	void GetReflectivity(float reflectivity[3]) const;
	float GetBumpmapScale() const;
	VTF::ImageFormat GetImageFormat() const;
	uchar GetMipCount() const;
	VTF::ImageFormat GetLowResImageFormat() const;
	bool GetLowResImageResolution(Resolution& res) const;
	uint16 GetDepth() const;
	uint32 GetResourceCount() const;

private:
	VTF::VTFFileHeader _header;
	std::vector<Resolution> _mipMapResolutions;
	void* _highResData;
	void* _lowResData;
	float _texelSize;
};

xvtf::VTFFile::__VTFFileImpl::__VTFFileImpl(const char* FilePath, bool HeaderOnly, ERRORCODE* xvtferrno)
{
	auto File = fopen(FilePath, "rb");
	if (File == nullptr)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::SYSTEM_FILE_ERROR);
		return;
	}

	/* Check that the Four Character Code is in fact VTF\0 */
	uchar FourCC[4] = { 0 };
	fread(FourCC, 1, 4, File);

	if (strcmp((const char*)FourCC, "VTF"))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NOT_A_VTF_FILE);
		fclose(File);
		return;
	}

	/* Get Version */
	uint32 Version[2];
	fread(Version, sizeof(uint32), 2, File);

	if (Version[0] > XVTF_VTF_MAX_VERSION_MAJOR ||
		Version[1] > XVTF_VTF_MAX_VERSION_MINOR)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::UNSUPPORTED_FILE_VERSION);
		fclose(File);
		return;
	}

	/* Get Header Size */
	uint32 HeaderSize;
	fread(&HeaderSize, sizeof(uint32), 1, File);

	/* Feed all of this information into the VTF header struct */
	fseek(File, 0, SEEK_SET);

	VTF::VTFFileHeader_r RawHeader;
	if (HeaderSize > sizeof(RawHeader))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::HEADER_SIZE_TOO_LARGE);
		fclose(File);
		return;
	}

	fread(&RawHeader, 1, HeaderSize, File);
	this->_header = *reinterpret_cast<VTF::VTFFileHeader*>(&RawHeader);

	/* Determine Texel Size (Bytes Per Pixel) */
	this->_texelSize = LUT::ImageFormatBPP[static_cast<uint32>(this->_header.imageFormat)];

	/* Determine LowResImage start (thumbnail) */
	uint32 LowResStart;
	if (this->_header.lowResImageFormat == VTF::ImageFormat::NONE)
	{
		LowResStart = -1;
	}
	else if (this->_header.version[1] < 3)
	{
		LowResStart = this->_header.headerSize;
	}
	else if (!GetResourceType((uint32)VTF::StockResourceTypes::LOW_RES_IMAGE, LowResStart))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::MISSING_IMAGE_DATA_MARKER);
		fclose(File);
		return;
	}

	/* Determine HighResImage start */
	uint32 HighResStart;
	if (this->_header.version[1] < 3)
	{
		HighResStart = static_cast<uint32>(this->_header.headerSize +
			(this->_header.lowResImageFormat == VTF::ImageFormat::NONE ? 0 : this->_header.lowResImageWidth * this->_header.lowResImageHeight * 0.5f));
	}
	else if (!GetResourceType((uint32)VTF::StockResourceTypes::HIGH_RES_IMAGE, HighResStart))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::MISSING_IMAGE_DATA_MARKER);
		fclose(File);
		return;
	}

	/* Fill MipLevel Resolutions */
	this->_mipMapResolutions.resize(this->_header.numMipLevels);
	for (auto i = this->_header.numMipLevels; i > 0; --i)
	{
		uint16 w = static_cast<uint16>(this->_header.width / std::pow(2, i - 1));
		uint16 h = static_cast<uint16>(this->_header.height / std::pow(2, i - 1));
		w == 0 ? w = 1 : w = w;
		h == 0 ? h = 1 : h = h;
		this->_mipMapResolutions[i - 1] = { w, h };
	}

	/* Normalize Header Values */
	if (this->_header.depth == 0 || (this->_header.version[1] < 2 && this->_header.version[0] == 7)) this->_header.depth = 1;
	if (this->_header.numFrames == 0) this->_header.numFrames = 1;
	if (this->_header.numMipLevels == 0) this->_header.numMipLevels = 1;
	if (this->_header.startFrame == 0xFFFF) this->_header.startFrame = 0;
	if (this->_header.version[1] < 3 && this->_header.version[0] == 7) this->_header.numResources = 0;

	/* Finish here if HeaderOnly */
	/* NOTE TO SELF: Stop commenting obvious shit */
	/* NOTE TO SELF: Stop breaking the fourth wall ;) */
	if (HeaderOnly)
	{
		fclose(File);
		return;
	}

	/* Read all the low res data */
	if (this->_header.lowResImageFormat != VTF::ImageFormat::NONE)
	{
		auto size = static_cast<uint32>((this->_header.lowResImageWidth < 4 ? 4 : this->_header.lowResImageWidth)
			* (this->_header.lowResImageHeight < 4 ? 4 : this->_header.lowResImageHeight) * 0.5f);
		this->_lowResData = malloc(sizeof(uchar) * size);
		fseek(File, LowResStart, SEEK_SET);
		fread(this->_lowResData, 1, size, File);
	}
	else
	{
		this->_lowResData = nullptr;
	}

	/* Read all the high res data */
	{
		bool COMP = this->_header.imageFormat == VTF::ImageFormat::DXT1 ||
			this->_header.imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA ||
			this->_header.imageFormat == VTF::ImageFormat::DXT3 ||
			this->_header.imageFormat == VTF::ImageFormat::DXT5;

		uint32 size = 0;
		uint32 FACTOR = this->_header.depth * this->_header.numFrames
			* ((static_cast<uint32>(this->_header.flags) & static_cast<uint32>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0 ? 6 : 1);

		for (uchar i = 0; i < this->_header.numMipLevels; i++)
		{
			uint32 RES;
			if (COMP)
			{
				RES = (this->_mipMapResolutions[i].Width < 4 ? 4 : this->_mipMapResolutions[i].Width)
					* (this->_mipMapResolutions[i].Height < 4 ? 4 : this->_mipMapResolutions[i].Height);
			}
			else
			{
				RES = this->_mipMapResolutions[i].Width * this->_mipMapResolutions[i].Height;
			}
			size += static_cast<uint32>(RES * FACTOR * this->_texelSize);
		}

		this->_highResData = malloc(sizeof(uchar) * size);
		fseek(File, HighResStart, SEEK_SET);
		fread(this->_highResData, 1, size, File);
	}

	XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);
	fclose(File);
}

xvtf::VTFFile::__VTFFileImpl::~__VTFFileImpl()
{
	if (this->_highResData != nullptr) free(this->_highResData);
	if (this->_lowResData != nullptr) free(this->_lowResData);
}

bool xvtf::VTFFile::__VTFFileImpl::GetResourceIndex(uint32 index, uint32& value) const
{
	if (index >= this->_header.numResources)
	{
		return false;
	}

	value = this->_header.resources[index].resData;
	return true;
}

bool xvtf::VTFFile::__VTFFileImpl::GetResourceType(uint32 type, uint32& value) const
{
	for (uint32 i = 0; i < this->_header.numResources; i++)
	{
		if (this->_header.resources[i].eType == static_cast<uint32>(type))
		{
			auto get = this->_header.resources[i];
			value = get.resData;

			return true;
		}
	}
	return false;
}

xvtf::BitmapImage* xvtf::VTFFile::__VTFFileImpl::GetImage(uchar MipLevel, uint16 Frame,
	uchar Face, uint16 zLevel, ERRORCODE* xvtferrno)
{
	/* assume no error */
	XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);

	bool EnvMap = (static_cast<uint32>(this->_header.flags) & static_cast<uint32>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0;

	/* Catch Bad Indices */
	if ((MipLevel >= this->_header.numMipLevels) ||
		(Frame >= this->_header.numFrames) ||
		(Face >= 6 || (!EnvMap && Face > 0)) ||
		(zLevel >= this->_header.depth))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::REQUESTED_IMAGE_DNE);
		return nullptr;
	}

	/* Find Appropriate Start To Data */
	uint32 START = 0;

	uint32 SLICE_FACTOR = this->_header.depth;
	uint32 FACE_FACTOR = SLICE_FACTOR * (EnvMap ? 6 : 1);
	uint32 FRAME_FACTOR = FACE_FACTOR * this->_header.numFrames;

	bool COMP = this->_header.imageFormat == VTF::ImageFormat::DXT1 ||
		this->_header.imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA ||
		this->_header.imageFormat == VTF::ImageFormat::DXT3 ||
		this->_header.imageFormat == VTF::ImageFormat::DXT5;

	uchar BytesPerPixel = LUT::ImageFormatBPPU[static_cast<uint32>(this->_header.imageFormat)];

	auto RES = this->_mipMapResolutions[MipLevel];
	uint32 RES_FACTOR;
	
	if (!COMP)
		RES_FACTOR = (uint32)RES.Width * RES.Height;
	else
		RES_FACTOR = (RES.Width < 4 ? 4 : (uint32)RES.Width) * (RES.Height < 4 ? 4 : (uint32)RES.Height);

	/* Scope to the correct Mip */
	if (COMP)
	{
		for (uchar MIP = this->_header.numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = this->_mipMapResolutions[MIP];
			START += static_cast<uint32>((res.Width < 4 ? 4 : (uint32)res.Width) * (res.Height < 4 ? 4 : (uint32)res.Width)
					* FRAME_FACTOR * this->_texelSize);
		}
	}
	else
	{
		for (uchar MIP = this->_header.numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = this->_mipMapResolutions[MIP];
			START += static_cast<uint32>
				((uint32)res.Width * res.Height * FRAME_FACTOR * this->_texelSize);
		}
	}

	/* Scope to the correct frame */
	START += static_cast<uint32>(Frame * RES_FACTOR * FACE_FACTOR * this->_texelSize);

	/* And the correct face... */
	START += static_cast<uint32>(Face * RES_FACTOR * SLICE_FACTOR * this->_texelSize);

	/* And finally the correct slice */
	START += static_cast<uint32>(zLevel * RES_FACTOR * this->_texelSize);

	/* And let the decoder take it from here (for compressed formats) */
	if (!COMP)
	{
		// Create a copy for the returned BitmapImage
		uchar* buffer = (uchar*)malloc(sizeof(uchar) * RES_FACTOR * BytesPerPixel);
		for (unsigned int i = 0; i < RES_FACTOR * BytesPerPixel; ++i)
		{
			*(buffer + i) = *((char*)this->_highResData + START + i);
		}

		auto rval = BitmapImage::Alloc((void*)buffer, RES_FACTOR, BytesPerPixel);
		free(buffer);
		return rval;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT1)
	{
		auto rval = BitmapImage::Alloc(Codecs::DecompressDXT1(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel);
		return rval;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA)
	{
		auto rval = BitmapImage::Alloc(Codecs::DecompressDXT1_ONEBITALPHA(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel);
		return rval;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT3)
	{
		auto rval = BitmapImage::Alloc(Codecs::DecompressDXT3(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel);
		return rval;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT5)
	{
		auto rval = BitmapImage::Alloc(Codecs::DecompressDXT5(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel);
		return rval;
	}

	XVTF_SETERROR(xvtferrno, ERRORCODE::UNKNOWN);
	return nullptr;
}

bool xvtf::VTFFile::__VTFFileImpl::GetResolution(Resolution& res, uchar index, ERRORCODE* xvtferrno) const
{
	/* catch null return value */
	if (&res == nullptr)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::REQUIRED_PARAMETER_NULL);
		return false;
	}

	if (index >= this->_header.numMipLevels)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::REQUESTED_IMAGE_DNE);
		return false;
	}
	else
	{
		res = this->_mipMapResolutions[index];
		XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);
	}

	return true;
}

void xvtf::VTFFile::__VTFFileImpl::GetVersion(uint32 version[2]) const
{
	version[0] = this->_header.version[0];
	version[1] = this->_header.version[1];
}

xvtf::VTF::ImageFlags xvtf::VTFFile::__VTFFileImpl::GetFlags() const
{
	return this->_header.flags;
}

uint16 xvtf::VTFFile::__VTFFileImpl::GetFrameCount() const
{
	return this->_header.numFrames;
}

uint16 xvtf::VTFFile::__VTFFileImpl::GetStartFrame() const
{
	return this->_header.startFrame;
}

void xvtf::VTFFile::__VTFFileImpl::GetReflectivity(float reflectivity[3]) const
{
	reflectivity[0] = this->_header.reflectivity[0];
	reflectivity[1] = this->_header.reflectivity[1];
	reflectivity[2] = this->_header.reflectivity[2];
}

float xvtf::VTFFile::__VTFFileImpl::GetBumpmapScale() const
{
	return this->_header.bumpScale;
}

xvtf::VTF::ImageFormat xvtf::VTFFile::__VTFFileImpl::GetImageFormat() const
{
	return this->_header.imageFormat;
}

uchar xvtf::VTFFile::__VTFFileImpl::GetMipCount() const
{
	return this->_header.numMipLevels;
}

xvtf::VTF::ImageFormat xvtf::VTFFile::__VTFFileImpl::GetLowResImageFormat() const
{
	return this->_header.lowResImageFormat;
}

bool xvtf::VTFFile::__VTFFileImpl::GetLowResImageResolution(Resolution& res) const
{
	if (this->_header.imageFormat == VTF::ImageFormat::NONE)
		return false;

	res.Width = (uint16)this->_header.lowResImageWidth;
	res.Height = (uint16)this->_header.lowResImageHeight;

	return true;
}

uint16 xvtf::VTFFile::__VTFFileImpl::GetDepth() const
{
	return this->_header.depth;
}

uint32 xvtf::VTFFile::__VTFFileImpl::GetResourceCount() const
{
	return this->_header.numResources;
}

xvtf::VTFFile* xvtf::VTFFile::Alloc(const char* FilePath, bool HeaderOnly, ERRORCODE* xvtferrno)
{
	ERRORCODE err; // temp err in case xvtferrno is nullptr
	auto impl = new __VTFFileImpl(FilePath, HeaderOnly, &err);

	if (err != ERRORCODE::NONE)
	{
		XVTF_SETERROR(xvtferrno, err);
		delete impl;
		return nullptr;
	}

	XVTF_SETERROR(xvtferrno, err);
	VTFFile* r = new VTFFile();
	r->_impl = impl;
	return r;
}

void xvtf::VTFFile::Free(VTFFile*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}

bool xvtf::VTFFile::GetResourceIndex(uint32 index, uint32& value) const
{
	return this->_impl->GetResourceIndex(index, value);
}

bool xvtf::VTFFile::GetResourceType(uint32 type, uint32& value) const
{
	return this->_impl->GetResourceType(type, value);
}

xvtf::BitmapImage* xvtf::VTFFile::GetImage(uchar MipLevel, uint16 Frame,
	uchar Face, uint16 zLevel, ERRORCODE* xvtferrno)
{
	return this->_impl->GetImage(MipLevel, Frame, Face, zLevel, xvtferrno);
}

bool xvtf::VTFFile::GetResolution(Resolution& res, uchar MipLevel, ERRORCODE* xvtferrno) const
{
	return this->_impl->GetResolution(res, MipLevel, xvtferrno);
}

void xvtf::VTFFile::GetVersion(uint32 version[2]) const
{
	return this->_impl->GetVersion(version);
}

xvtf::VTF::ImageFlags xvtf::VTFFile::GetFlags() const
{
	return this->_impl->GetFlags();
}

uint16 xvtf::VTFFile::GetFrameCount() const
{
	return this->_impl->GetFrameCount();
}

uint16 xvtf::VTFFile::GetStartFrame() const
{
	return this->_impl->GetStartFrame();
}

void xvtf::VTFFile::GetReflectivity(float reflectivity[3]) const
{
	return this->_impl->GetReflectivity(reflectivity);
}

float xvtf::VTFFile::GetBumpmapScale() const
{
	return this->_impl->GetBumpmapScale();
}

xvtf::VTF::ImageFormat xvtf::VTFFile::GetImageFormat() const
{
	return this->_impl->GetImageFormat();
}

uchar xvtf::VTFFile::GetMipCount() const
{
	return this->_impl->GetMipCount();
}

xvtf::VTF::ImageFormat xvtf::VTFFile::GetLowResImageFormat() const
{
	return this->_impl->GetLowResImageFormat();
}

bool xvtf::VTFFile::GetLowResImageResolution(Resolution& res) const
{
	return this->_impl->GetLowResImageResolution(res);
}

uint16 xvtf::VTFFile::GetDepth() const
{
	return this->_impl->GetDepth();
}

uint32 xvtf::VTFFile::GetResourceCount() const
{
	return this->_impl->GetResourceCount();
}