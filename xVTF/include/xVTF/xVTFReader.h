#ifndef XVTF_READER_H__
#define XVTF_READER_H__

#include "xVTF/xExports.h"
#include "xVTF/xVTFFile.h"

#include <memory>
#include <stdexcept>

namespace XVTF_NS
{
	namespace IO
	{
		namespace VTFReader
		{
			//----------------------------------------------------------------------------------------------------
			/// Opens the VTF file specified at filename and returns a VTF object from which you can read/modify its data.
			/// @param[in] filename									Path to the VTF file we want to load.
			/// @param[in] HeaderOnly								If true, only load the header (useful for probing files).
			/// @return std::unique_ptr<VTFFile>					A standard unique pointer to the VTF file object.
			/// @throws std::runtime_error							If file cannot be open.
			/// @throws std::invalid_argument						If the VTF version is not supported.
			//----------------------------------------------------------------------------------------------------
			XVTFAPI std::unique_ptr<XVTF_NS::ImageFile::VTFFile> Open(const char* filename, const bool& HeaderOnly = false);
		}
	}
}

#endif // !XVTF_READER_H__
