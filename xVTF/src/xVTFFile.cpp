#include "xVTF/xVTFFile.h"

XVTF_NS::ImageFile::VTFFile::VTFFile(std::unique_ptr<VTF::VTFFileHeader_r> header)
{
	this->_headerRaw = std::move(header);
}

std::shared_ptr<XVTF_NS::ImageFile::VTF::VTFFileHeader> XVTF_NS::ImageFile::VTFFile::GetHeader()
{
	if (!this->_headerAligned)
	{
		this->_headerAligned = std::static_pointer_cast<XVTF_NS::ImageFile::VTF::VTFFileHeader>(this->_headerRaw);
	}

	return this->_headerAligned;
}