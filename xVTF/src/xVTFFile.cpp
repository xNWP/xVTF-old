#include "xVTF/xVTFFile.h"

#include "xVTF/xCodecs.h"
#include "xVTF/xLUTs.h"
#include "xVTF/xVTFError.h"
#include "xVTF/xVTFHeaders.h"
#include "xVTF/xVTFStructs.h"

#include <memory>
#include <stdexcept>
#include <vector>

class xvtf::Bitmap::VTFFile::__VTFFileImpl
{
public:
	__VTFFileImpl(const char* FilePath, const bool HeaderOnly, unsigned int * const & xvtferrno);
	virtual ~__VTFFileImpl();

	bool GetResourceIndex(const unsigned int index, unsigned int& value) const;
	bool GetResourceType(const unsigned int type, unsigned int& value) const;
	bool GetImage(BitmapImage*& bmp, unsigned int * const & xvtferrno, const unsigned int MipLevel, const unsigned int Frame, const unsigned int Face, const unsigned int zLevel);
	bool GetResolution(Resolution* const & res, const unsigned int index, unsigned int * const & xvtferrno) const;

private:
	VTFFileHeader _header;
	std::vector<Resolution> _mipMapResolutions;
	void* _highResData;
	void* _lowResData;
	float _texelSize;
	std::vector<BitmapImage*> _storedImages;
};

xvtf::Bitmap::VTFFile::__VTFFileImpl::__VTFFileImpl(const char* FilePath, const bool HeaderOnly, unsigned int * const & xvtferrno)
{
	auto File = fopen(FilePath, "rb");
	if (File == nullptr)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::SYSTEM_FILE_ERROR);
		return;
	}

	/* Check that the Four Character Code is in fact VTF\0 */
	char FourCC[4] = { 0 };
	fread(FourCC, 1, 4, File);

	if (strcmp(FourCC, "VTF"))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NOT_A_VTF_FILE);
		return;
	}

	/* Get Version */
	unsigned int Version[2];
	fread(Version, sizeof(unsigned int), 2, File);

	if (Version[0] > XVTF_VTF_MAX_VERSION_MAJOR ||
		Version[1] > XVTF_VTF_MAX_VERSION_MINOR)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::UNSUPPORTED_FILE_VERSION);
		return;
	}

	/* Get Header Size */
	unsigned int HeaderSize;
	fread(&HeaderSize, sizeof(unsigned int), 1, File);

	/* Feed all of this information into the VTF header struct */
	_fseeki64(File, 0, SEEK_SET);

	VTFFileHeader_r RawHeader;
	if (HeaderSize > sizeof(RawHeader))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::HEADER_SIZE_TOO_LARGE);
		return;
	}

	fread(&RawHeader, 1, HeaderSize, File);
	this->_header = *reinterpret_cast<VTFFileHeader*>(&RawHeader);

	/* Determine Texel Size (Bytes Per Pixel) */
	this->_texelSize = Tools::LUT::ImageFormatBPP[static_cast<unsigned int>(this->_header.imageFormat)];

	/* Determine LowResImage start (thumbnail) */
	unsigned int LowResStart;
	if (this->_header.lowResImageFormat == VTF::ImageFormat::NONE)
	{
		LowResStart = -1;
	}
	else if (this->_header.version[1] < 3)
	{
		LowResStart = this->_header.headerSize;
	}
	else if (!GetResourceType((unsigned int)VTF::StockResourceTypes::LOW_RES_IMAGE, LowResStart))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::MISSING_IMAGE_DATA_MARKER);
		return;
	}

	/* Determine HighResImage start */
	unsigned int HighResStart;
	if (this->_header.version[1] < 3)
	{
		HighResStart = static_cast<unsigned int>(this->_header.headerSize +
			(this->_header.lowResImageFormat == VTF::ImageFormat::NONE ? 0 : this->_header.lowResImageWidth * this->_header.lowResImageHeight * 0.5f));
	}
	else if (!GetResourceType((unsigned int)VTF::StockResourceTypes::HIGH_RES_IMAGE, HighResStart))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::MISSING_IMAGE_DATA_MARKER);
		return;
	}

	/* Fill MipLevel Resolutions */
	this->_mipMapResolutions.resize(this->_header.numMipLevels);
	for (auto i = this->_header.numMipLevels; i > 0; --i)
	{
		unsigned int w = static_cast<unsigned int>(this->_header.width / std::pow(2, i - 1));
		unsigned int h = static_cast<unsigned int>(this->_header.height / std::pow(2, i - 1));
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
		auto size = static_cast<unsigned int>((this->_header.lowResImageWidth < 4 ? 4 : this->_header.lowResImageWidth)
			* (this->_header.lowResImageHeight < 4 ? 4 : this->_header.lowResImageHeight) * 0.5f);
		this->_lowResData = new char[size];
		_fseeki64(File, LowResStart, 0);
		fread(this->_lowResData, 1, size, File);
	}

	/* Read all the high res data */
	{
		unsigned int size = 0;
		const unsigned int FACTOR = this->_header.depth * this->_header.numFrames
			* ((static_cast<unsigned int>(this->_header.flags) & static_cast<unsigned int>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0 ? 6 : 1);

		for (unsigned int i = 0; i < this->_header.numMipLevels; i++)
		{
			auto RES = (this->_mipMapResolutions[i].Width < 4 ? 4 : this->_mipMapResolutions[i].Width)
				* (this->_mipMapResolutions[i].Height < 4 ? 4 : this->_mipMapResolutions[i].Height);
			size += static_cast<unsigned int>(RES * FACTOR * this->_texelSize);
		}

		this->_highResData = new char[size];
		_fseeki64(File, HighResStart, 0);
		fread(this->_highResData, 1, size, File);
	}

	fclose(File);
}

xvtf::Bitmap::VTFFile::__VTFFileImpl::~__VTFFileImpl()
{
	/* Delete stored images */
	for (auto&& i : this->_storedImages)
		BitmapImage::Free(i);

	if (this->_highResData != nullptr) delete[] this->_highResData;
	if (this->_lowResData != nullptr) delete[] this->_lowResData;
}

bool xvtf::Bitmap::VTFFile::__VTFFileImpl::GetResourceIndex(const unsigned int index, unsigned int& value) const
{
	if (index >= this->_header.numResources)
	{
		return false;
	}

	value = this->_header.resources[index].resData;
	return true;
}

bool xvtf::Bitmap::VTFFile::__VTFFileImpl::GetResourceType(const unsigned int type, unsigned int& value) const
{
	for (unsigned int i = 0; i < this->_header.numResources; i++)
	{
		if (this->_header.resources[i].eType == static_cast<unsigned int>(type))
		{
			auto get = this->_header.resources[i];
			value = get.resData;

			return true;
		}
	}
	return false;
}

bool xvtf::Bitmap::VTFFile::__VTFFileImpl::GetImage(BitmapImage*& bmp, unsigned int * const & xvtferrno,
	const unsigned int MipLevel, const unsigned int Frame,
	const unsigned int Face, const unsigned int zLevel)
{
	/* assume no error */
	XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);

	const bool EnvMap = (static_cast<unsigned int>(this->_header.flags) & static_cast<unsigned int>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0;

	/* Catch Bad Indices */
	if ((MipLevel >= this->_header.numMipLevels) ||
		(Frame >= this->_header.numFrames) ||
		(Face >= 6 || (!EnvMap && Face > 0)) ||
		(zLevel >= this->_header.depth))
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::REQUESTED_IMAGE_DNE);
		return false;
	}

	/* Find Appropriate Start To Data */
	unsigned int START = 0;

	const unsigned int SLICE_FACTOR = this->_header.depth;
	const unsigned int FACE_FACTOR = SLICE_FACTOR * (EnvMap ? 6 : 1);
	const unsigned int FRAME_FACTOR = FACE_FACTOR * this->_header.numFrames;

	const bool COMP = this->_header.imageFormat == VTF::ImageFormat::DXT1 ||
		this->_header.imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA ||
		this->_header.imageFormat == VTF::ImageFormat::DXT3 ||
		this->_header.imageFormat == VTF::ImageFormat::DXT5;

	unsigned short BytesPerPixel = Tools::LUT::ImageFormatBPPU[static_cast<unsigned int>(this->_header.imageFormat)];

	auto RES = this->_mipMapResolutions[MipLevel];
	unsigned int RES_FACTOR;
	
	if (!COMP)
		RES_FACTOR = RES.Width * RES.Height;
	else
		RES_FACTOR = (RES.Width < 4 ? 4 : RES.Width) * (RES.Height < 4 ? 4 : RES.Height);

	/* Scope to the correct Mip */
	if (COMP)
	{
		for (unsigned int MIP = this->_header.numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = this->_mipMapResolutions[MIP];
			START += static_cast<unsigned int>
				((res.Width < 4 ? 4 : res.Width) * (res.Height < 4 ? 4 : res.Width)
					* FRAME_FACTOR * this->_texelSize);
		}
	}
	else
	{
		for (unsigned int MIP = this->_header.numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = this->_mipMapResolutions[MIP];
			START += static_cast<unsigned int>
				(res.Width * res.Height * FRAME_FACTOR * this->_texelSize);
		}
	}

	/* Scope to the correct frame */
	START += static_cast<unsigned int>(Frame * RES_FACTOR * FACE_FACTOR * this->_texelSize);

	/* And the correct face... */
	START += static_cast<unsigned int>(Face * RES_FACTOR * SLICE_FACTOR * this->_texelSize);

	/* And finally the correct slice */
	START += static_cast<unsigned int>(zLevel * RES_FACTOR * this->_texelSize);

	/* And let the decoder take it from here (for compressed formats) */
	if (!COMP)
	{
		_storedImages.push_back(BitmapImage::Alloc(((void*)((char*)this->_highResData + START)), RES_FACTOR, BytesPerPixel, false));
		bmp = _storedImages[_storedImages.size()];
		return true;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT1)
	{
		_storedImages.push_back(BitmapImage::Alloc(Tools::Codecs::DecompressDXT1(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true));
		bmp = _storedImages[_storedImages.size()];
		return true;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA)
	{
		_storedImages.push_back(BitmapImage::Alloc(Tools::Codecs::DecompressDXT1_ONEBITALPHA(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true));
		bmp = _storedImages[_storedImages.size()];
		return true;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT3)
	{
		_storedImages.push_back(BitmapImage::Alloc(Tools::Codecs::DecompressDXT3(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true));
		bmp = _storedImages[_storedImages.size()];
		return true;
	}
	else if (this->_header.imageFormat == VTF::ImageFormat::DXT5)
	{
		_storedImages.push_back(BitmapImage::Alloc(Tools::Codecs::DecompressDXT5(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true));
		bmp = _storedImages[_storedImages.size()];
		return true;
	}

	XVTF_SETERROR(xvtferrno, ERRORCODE::UNKNOWN);
	return false;
}

bool xvtf::Bitmap::VTFFile::__VTFFileImpl::GetResolution(Resolution* const & res, const unsigned int index, unsigned int * const & xvtferrno) const
{
	/* catch null return value */
	if (res == nullptr)
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
		*res = this->_mipMapResolutions[index];
		XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);
	}

	return true;
}

xvtf::Bitmap::VTFFile* xvtf::Bitmap::VTFFile::Alloc(const char* FilePath, const bool HeaderOnly, unsigned int * const & xvtferrno)
{
	VTFFile* r = nullptr;
	auto impl = new __VTFFileImpl(FilePath, HeaderOnly, xvtferrno);

	if (xvtferrno != (unsigned int)ERRORCODE::NONE)
	{
		delete impl;
		return r;
	}

	r = new VTFFile();
	r->_impl = impl;
	return r;
}

void xvtf::Bitmap::VTFFile::Free(VTFFile*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}

bool xvtf::Bitmap::VTFFile::GetResourceIndex(const unsigned int index, unsigned int& value) const
{
	return this->_impl->GetResourceIndex(index, value);
}

bool xvtf::Bitmap::VTFFile::GetResourceType(const unsigned int type, unsigned int& value) const
{
	return this->_impl->GetResourceType(type, value);
}

bool xvtf::Bitmap::VTFFile::GetImage(BitmapImage*& bmp, unsigned int * const & xvtferrno,
	const unsigned int MipLevel, const unsigned int Frame,
	const unsigned int Face, const unsigned int zLevel)
{
	return this->_impl->GetImage(bmp, xvtferrno, MipLevel, Frame, Face, zLevel);
}

bool xvtf::Bitmap::VTFFile::GetResolution(Resolution* const & res, const unsigned int MipLevel, unsigned int * const & xvtferrno) const
{
	return this->_impl->GetResolution(res, MipLevel, xvtferrno);
}