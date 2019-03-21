#include "xVTF/xVTFFile.h"

#include "xVTF/xCodecs.h"
#include "xVTF/xLUTs.h"

#include <stdexcept>
#include <memory>

class XVTF_NS::Bitmap::VTFFile::__VTFFileImpl
{
public:
	__VTFFileImpl(const char* FilePath, const bool& HeaderOnly);
	virtual ~__VTFFileImpl();

	VTF::VTFFileHeader* GetHeader();
	VTF::VTFResource* GetResourceType(const unsigned int& type);
	BitmapImage* GetImage(const unsigned int& MipLevel = 0, const unsigned int& Frame = 0, const unsigned int& Face = 0, const unsigned int& zLevel = 0);
	const Resolution* GetResolutions() const;

private:
	std::shared_ptr<VTF::VTFFileHeader_r> _headerRaw;
	std::shared_ptr<VTF::VTFFileHeader> _headerAligned;
	Resolution* _mipMapResolutions;
	void* _highResData;
	void* _lowResData;
	float _texelSize;
};

XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::__VTFFileImpl(const char* FilePath, const bool& HeaderOnly)
{
	auto File = fopen(FilePath, "rb");
	if (File == nullptr)
	{
		std::string err = "Error opening file '";
		err += FilePath; err += "': ";
		err += strerror(errno);
		throw std::runtime_error(err);
	}

	/* Check that the Four Character Code is in fact VTF\0 */
	char FourCC[4] = { 0 };
	fread(FourCC, 1, 4, File);

	if (strcmp(FourCC, "VTF"))
	{
		std::string err = "Error opening file '";
		err += FilePath; err += "': Not a valid VTF file. FourCC was not VTF\\0.";
		throw std::runtime_error(err);
	}

	/* Get Version */
	unsigned int Version[2];
	fread(Version, sizeof(unsigned int), 2, File);

	if (Version[0] > XVTF_VTF_MAX_VERSION_MAJOR ||
		Version[1] > XVTF_VTF_MAX_VERSION_MINOR)
	{
		std::string err = "Error opening file '";
		err += FilePath; err += "': Version of VTF not supported.";
		throw std::invalid_argument(err);
	}

	/* Get Header Size */
	unsigned int HeaderSize;
	fread(&HeaderSize, sizeof(unsigned int), 1, File);

	/* Feed all of this information into the VTF header struct */
	_fseeki64(File, 0, SEEK_SET);

	this->_headerRaw = std::make_shared<VTF::VTFFileHeader_r>();
	if (HeaderSize > sizeof(*this->_headerRaw))
	{
		std::string err = "Error opening file '";
		err += FilePath; err += "': HeaderSize too large, possibly corrupt file?";
		throw std::runtime_error(err);
	}

	fread(this->_headerRaw.get(), 1, HeaderSize, File);
	this->_headerAligned = std::static_pointer_cast<VTF::VTFFileHeader>(this->_headerRaw);

	auto Header = this->_headerAligned;

	/* Determine Texel Size (Bytes Per Pixel) */
	this->_texelSize = Tools::LUT::ImageFormatBPP[static_cast<unsigned int>(Header->imageFormat)];

	/* Determine LowResImage start (thumbnail) */
	unsigned int LowResStart;
	if (Header->lowResImageFormat == VTF::ImageFormat::NONE)
	{
		LowResStart = -1;
	}
	else if (Header->version[1] < 3)
	{
		LowResStart = Header->headerSize;
	}
	else
	{
		LowResStart = GetResourceType(static_cast<unsigned int>(VTF::StockResourceTypes::LOW_RES_IMAGE))->resData;
	}

	/* Determine HighResImage start */
	unsigned int HighResStart;
	if (Header->version[1] < 3)
	{
		HighResStart = static_cast<unsigned int>(Header->headerSize +
			(Header->lowResImageFormat == VTF::ImageFormat::NONE ? 0 : Header->lowResImageWidth * Header->lowResImageHeight * 0.5f));
	}
	else
	{
		HighResStart = GetResourceType(static_cast<unsigned int>(VTF::StockResourceTypes::HIGH_RES_IMAGE))->resData;
	}

	/* Fill MipLevel Resolutions */
	this->_mipMapResolutions = new Resolution[Header->numMipLevels];
	for (unsigned int i = Header->numMipLevels; i > 0; --i)
	{
		unsigned int w = static_cast<unsigned int>(Header->width / std::pow(2, i - 1));
		unsigned int h = static_cast<unsigned int>(Header->height / std::pow(2, i - 1));
		w == 0 ? w = 1 : w = w;
		h == 0 ? h = 1 : h = h;
		this->_mipMapResolutions[i - 1] = { w, h };
	}

	/* Normalize Header Values */
	if (Header->depth == 0 || (Header->version[1] < 2 && Header->version[0] == 7)) Header->depth = 1;
	if (Header->numFrames == 0) Header->numFrames = 1;
	if (Header->numMipLevels == 0) Header->numMipLevels = 1;
	if (Header->startFrame == 0xFFFF) Header->startFrame = 0;
	if (Header->version[1] < 3 && Header->version[0] == 7) Header->numResources = 0;

	/* Finish here if HeaderOnly */
	/* NOTE TO SELF: Stop commenting obvious shit */
	/* NOTE TO SELF: Stop breaking the fourth wall ;) */
	if (HeaderOnly)
	{
		fclose(File);
		return;
	}

	/* Read all the low res data */
	if (Header->lowResImageFormat != VTF::ImageFormat::NONE)
	{
		auto size = static_cast<unsigned int>((Header->lowResImageWidth < 4 ? 4 : Header->lowResImageWidth)
			* (Header->lowResImageHeight < 4 ? 4 : Header->lowResImageHeight) * 0.5f);
		this->_lowResData = new char[size];
		_fseeki64(File, LowResStart, 0);
		fread(this->_lowResData, 1, size, File);
	}

	/* Read all the high res data */
	{
		unsigned int size = 0;
		const unsigned int FACTOR = Header->depth * Header->numFrames
			* ((static_cast<unsigned int>(Header->flags) & static_cast<unsigned int>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0 ? 6 : 1);

		for (unsigned int i = 0; i < Header->numMipLevels; i++)
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

XVTF_NS::Bitmap::VTF::VTFFileHeader* XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::GetHeader()
{
	return this->_headerAligned.get();
}

XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::~__VTFFileImpl()
{
	delete[] this->_highResData;
	delete[] this->_lowResData;
	delete[] this->_mipMapResolutions;
}

XVTF_NS::Bitmap::VTF::VTFResource* XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::GetResourceType(const unsigned int& type)
{
	for (unsigned int i = 0; i < this->GetHeader()->numResources; i++)
	{
		if (this->GetHeader()->resources[i].eType == type)
		{
			VTF::VTFResource* ptr = static_cast<VTF::VTFResource*>(&this->GetHeader()->resources[i]);
			return ptr;
		}
	}
	return nullptr;
}

XVTF_NS::Bitmap::BitmapImage* XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::GetImage(const unsigned int& MipLevel, const unsigned int& Frame,
	const unsigned int& Face, const unsigned int& zLevel)
{
	auto Header = this->_headerAligned;
	const bool EnvMap = (static_cast<unsigned int>(Header->flags) & static_cast<unsigned int>(VTF::ImageFlags::ENVIRONMENTMAP)) != 0;
	/* Catch Bad Indices */
	if (MipLevel >= this->_headerAligned->numMipLevels)
		throw std::out_of_range("GetImageData: MipLevel out of range.");
	if (Frame >= this->_headerAligned->numFrames)
		throw std::out_of_range("GetImageData: Frame out of range.");
	if (Face >= 6 || (!EnvMap && Face > 0))
		throw std::out_of_range("GetImageData: Face out of range.");
	if (zLevel >= this->_headerAligned->depth)
		throw std::out_of_range("GetImageData: zLevel out of range.");

	/* Find Appropriate Start To Data */
	unsigned int START = 0;

	const unsigned int SLICE_FACTOR = Header->depth;
	const unsigned int FACE_FACTOR = SLICE_FACTOR * (EnvMap ? 6 : 1);
	const unsigned int FRAME_FACTOR = FACE_FACTOR * Header->numFrames;

	const bool COMP = Header->imageFormat == VTF::ImageFormat::DXT1 ||
		Header->imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA ||
		Header->imageFormat == VTF::ImageFormat::DXT3 ||
		Header->imageFormat == VTF::ImageFormat::DXT5;

	unsigned short BytesPerPixel = Tools::LUT::ImageFormatBPPU[static_cast<unsigned int>(Header->imageFormat)];

	auto RES = *(this->_mipMapResolutions + MipLevel);
	unsigned int RES_FACTOR;
	
	if (!COMP)
		RES_FACTOR = RES.Width * RES.Height;
	else
		RES_FACTOR = (RES.Width < 4 ? 4 : RES.Width) * (RES.Height < 4 ? 4 : RES.Height);

	/* Scope to the correct Mip */
	if (COMP)
	{
		for (unsigned int MIP = Header->numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = *(this->_mipMapResolutions + MIP);
			START += static_cast<unsigned int>
				((res.Width < 4 ? 4 : res.Width) * (res.Height < 4 ? 4 : res.Width)
					* FRAME_FACTOR * this->_texelSize);
		}
	}
	else
	{
		for (unsigned int MIP = Header->numMipLevels - 1; MIP > MipLevel; --MIP)
		{
			auto res = *(this->_mipMapResolutions + MIP);
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
		return BitmapImage::Alloc( ((void*)((char*)this->_highResData + START)), RES_FACTOR, BytesPerPixel, false );
	else if (this->_headerAligned->imageFormat == VTF::ImageFormat::DXT1)
		return BitmapImage::Alloc(Tools::Codecs::DecompressDXT1(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true);
	else if (this->_headerAligned->imageFormat == VTF::ImageFormat::DXT1_ONEBITALPHA)
		return BitmapImage::Alloc(Tools::Codecs::DecompressDXT1_ONEBITALPHA(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true);
	else if (this->_headerAligned->imageFormat == VTF::ImageFormat::DXT3)
		return BitmapImage::Alloc(Tools::Codecs::DecompressDXT3(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true);
	else if (this->_headerAligned->imageFormat == VTF::ImageFormat::DXT5)
		return BitmapImage::Alloc(Tools::Codecs::DecompressDXT5(this->_highResData, START, RES.Width, RES.Height), RES_FACTOR, BytesPerPixel, true);

	throw std::runtime_error("Critical Error in XVTF_NS::Bitmap::BitmapImage* XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::GetImage(const unsigned int& MipLevel, const unsigned int& Frame, unsigned int& Face, const unsigned int& zLevel)");
	return nullptr;
}

const XVTF_NS::Bitmap::Resolution* XVTF_NS::Bitmap::VTFFile::__VTFFileImpl::GetResolutions() const
{
	return this->_mipMapResolutions;
}

XVTF_NS::Bitmap::VTFFile* XVTF_NS::Bitmap::VTFFile::Alloc(const char* FilePath, const bool& HeaderOnly)
{
	VTFFile* r = new VTFFile();
	r->_impl = new __VTFFileImpl(FilePath, HeaderOnly);
	return r;
}

void XVTF_NS::Bitmap::VTFFile::Free(VTFFile*& obj)
{
	delete obj->_impl;
	obj->_impl = nullptr;
	delete obj;
	obj = nullptr;
}

XVTF_NS::Bitmap::VTF::VTFFileHeader* XVTF_NS::Bitmap::VTFFile::GetHeader()
{
	return this->_impl->GetHeader();
}

XVTF_NS::Bitmap::VTF::VTFResource* XVTF_NS::Bitmap::VTFFile::GetResourceType(const unsigned int& type)
{
	return this->_impl->GetResourceType(type);
}

XVTF_NS::Bitmap::BitmapImage* XVTF_NS::Bitmap::VTFFile::GetImage(const unsigned int& MipLevel, const unsigned int& Frame,
	const unsigned int& Face, const unsigned int& zLevel)
{
	return this->_impl->GetImage(MipLevel, Frame, Face, zLevel);
}

const XVTF_NS::Bitmap::Resolution* XVTF_NS::Bitmap::VTFFile::GetResolutions() const
{
	return this->_impl->GetResolutions();
}