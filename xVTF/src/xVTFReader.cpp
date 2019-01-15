#include "xVTF/xVTFReader.h"

std::unique_ptr<XVTF_NS::ImageFile::VTFFile> XVTF_NS::IO::VTFReader::Open(const char* filename, const bool& HeaderOnly)
{
	auto File = fopen(filename, "rb");
	if (File == nullptr)
	{
		std::string err = "Error opening file '";
		err += filename; err += "': ";
		err += strerror(errno);
		throw std::runtime_error(err);
	}

	/* Check that the Four Character Code is in fact VTF\0 */
	char FourCC[4] = { 0 };
	fread(FourCC, 1, 4, File);

	if (strcmp(FourCC, "VTF"))
	{
		std::string err = "Error opening file '";
		err += filename; err += "': Not a valid VTF file. FourCC was not VTF\\0.";
		throw std::runtime_error(err);
	}

	/* Get Version */
	unsigned int Version[2];
	fread(Version, sizeof(unsigned int), 2, File);

	if (Version[0] > XVTF_VTF_MAX_VERSION_MAJOR ||
		Version[1] > XVTF_VTF_MAX_VERSION_MINOR)
	{
		std::string err = "Error opening file '";
		err += filename; err += "': Version of VTF not supported.";
		throw std::invalid_argument(err);
	}

	/* Get Header Size */
	unsigned int HeaderSize;
	fread(&HeaderSize, sizeof(unsigned int), 1, File);

	/* Feed all of this information into the VTF header struct */
	auto VTFHeader = std::make_unique<xvtf::ImageFile::VTF::VTFFileHeader_r>();
	fseek(File, 0, SEEK_SET);

	if (HeaderSize > sizeof(*VTFHeader))
	{
		std::string err = "Error opening file '";
		err += filename; err += "': HeaderSize too large, possibly corrupt file?";
		throw std::runtime_error(err);
	}

	fread(VTFHeader.get(), 1, HeaderSize, File);

	/* Create VTFFile Object */
	auto vtfFile = std::make_unique<xvtf::ImageFile::VTFFile>(std::move(VTFHeader));

	// TODO: Read image data.
	fclose(File);

	return vtfFile;
}