#include <iostream>
#include <fstream>
#include <string>
#include "xVTF/xVTF.h"

int main(unsigned int argc, char** argv)
{
	std::ifstream is(*(argv + 1));
	std::string CurrentFile;
	unsigned int i = 0;

	unsigned int VerUnknown = 0;
	unsigned int VerSevZero = 0;
	unsigned int VerSevOne = 0;
	unsigned int VerSevTwo = 0;
	unsigned int VerSevThree = 0;
	unsigned int VerSevFour = 0;
	unsigned int VerSevFive = 0;

	unsigned int A8 = 0;
	unsigned int ABGR8888 = 0;
	unsigned int ARGB8888 = 0;
	unsigned int BGR565 = 0;
	unsigned int BGR888 = 0;
	unsigned int BGR888_BLUESCREEN = 0;
	unsigned int BGRA4444 = 0;
	unsigned int BGRA5551 = 0;
	unsigned int BGRA8888 = 0;
	unsigned int BGRX5551 = 0;
	unsigned int BGRX8888 = 0;
	unsigned int DXT1 = 0;
	unsigned int DXT1_ONEBITALPHA = 0;
	unsigned int DXT3 = 0;
	unsigned int DXT5 = 0;
	unsigned int I8 = 0;
	unsigned int IA88 = 0;
	unsigned int NONE = 0;
	unsigned int P8 = 0;
	unsigned int RGB565 = 0;
	unsigned int RGB888 = 0;
	unsigned int RGB888_BLUESCREEN = 0;
	unsigned int RGBA16161616 = 0;
	unsigned int RGBA16161616F = 0;
	unsigned int RGBA8888 = 0;
	unsigned int UV88 = 0;
	unsigned int UVLX8888 = 0;
	unsigned int UVWQ8888 = 0;
	unsigned int FormUnk = 0;

	unsigned int POINTSAMPLING		   = 0;
	unsigned int TRILINEARSAMPLING	   = 0;
	unsigned int CLAMPS				   = 0;
	unsigned int CLAMPT				   = 0;
	unsigned int ANISOTROPICSAMPLING   = 0;
	unsigned int HINTDXT5			   = 0;
	unsigned int NOCOMPRESS			   = 0;
	unsigned int NORMALMAP			   = 0;
	unsigned int NOMIPMAPS			   = 0;
	unsigned int NOLOD				   = 0;
	unsigned int NOMINIMUMMIPMAP	   = 0;
	unsigned int PROCEDURAL			   = 0;
	unsigned int ONEBITALPHA		   = 0;
	unsigned int EIGHTBITALHPA		   = 0;
	unsigned int ENVIRONMENTMAP		   = 0;
	unsigned int RENDERTARGET		   = 0;
	unsigned int DEPTHRENDERTARGET	   = 0;
	unsigned int NODEBUGOVERRIDE	   = 0;
	unsigned int SINGLECOPY			   = 0;
	unsigned int PRESRGB			   = 0;
	unsigned int NODEPTHBUFFER		   = 0;
	unsigned int NICEFILTERED		   = 0;
	unsigned int CLAMPU				   = 0;
	unsigned int VERTEXTEXTURE		   = 0;
	unsigned int SSBUMP				   = 0;
	unsigned int BORDER				   = 0;
	unsigned int FlagZero = 0;

	while(std::getline(is, CurrentFile))
	{
		auto File = xvtf::IO::VTFReader::Open(CurrentFile.c_str(), true);
		auto Header = File->GetHeader();
		auto Version = Header->version;
		auto ImageForm = Header->imageFormat;
		auto Flags = Header->flags;

		if (Version[0] != 7)
		{
			VerUnknown++;
		}
		else if (Version[1] == 0 && Version[0] == 7)
			VerSevZero++;
		else if (Version[1] == 1 && Version[0] == 7)
			VerSevOne++;
		else if (Version[1] == 2 && Version[0] == 7)
			VerSevTwo++;
		else if (Version[1] == 3 && Version[0] == 7)
			VerSevThree++;
		else if (Version[1] == 4 && Version[0] == 7)
			VerSevFour++;
		else if (Version[1] == 5 && Version[0] == 7)
			VerSevFive++;
		else
		{
			VerUnknown++;
			std::cout << "Abnormal Version in '" << CurrentFile << "': " << Version[0] << '.' << Version[1] << '\n';
		}

			using namespace xvtf::ImageFile::VTF;

			if (ImageForm == ImageFormat::A8)
				A8++;
			else if (ImageForm == ImageFormat::ABGR8888)
				ABGR8888++;
			else if (ImageForm == ImageFormat::ARGB8888)
				ARGB8888++;
			else if (ImageForm == ImageFormat::BGR565)
				BGR565++;
			else if (ImageForm == ImageFormat::BGR888)
				BGR888++;
			else if (ImageForm == ImageFormat::BGR888_BLUESCREEN)
				BGR888_BLUESCREEN++;
			else if (ImageForm == ImageFormat::BGRA4444)
				BGRA4444++;
			else if (ImageForm == ImageFormat::BGRA5551)
				BGRA5551++;
			else if (ImageForm == ImageFormat::BGRA8888)
				BGRA8888++;
			else if (ImageForm == ImageFormat::BGRX5551)
				BGRX5551++;
			else if (ImageForm == ImageFormat::BGRX8888)
				BGRX8888++;
			else if (ImageForm == ImageFormat::DXT1)
				DXT1++;
			else if (ImageForm == ImageFormat::DXT1_ONEBITALPHA)
				DXT1_ONEBITALPHA++;
			else if (ImageForm == ImageFormat::DXT3)
				DXT3++;
			else if (ImageForm == ImageFormat::DXT5)
				DXT5++;
			else if (ImageForm == ImageFormat::I8)
				I8++;
			else if (ImageForm == ImageFormat::IA88)
				IA88++;
			else if (ImageForm == ImageFormat::NONE)
				NONE++;
			else if (ImageForm == ImageFormat::P8)
				P8++;
			else if (ImageForm == ImageFormat::RGB565)
				RGB565++;
			else if (ImageForm == ImageFormat::RGB888)
				RGB888++;
			else if (ImageForm == ImageFormat::RGB888_BLUESCREEN)
				RGB888_BLUESCREEN++;
			else if (ImageForm == ImageFormat::RGBA16161616)
				RGBA16161616++;
			else if (ImageForm == ImageFormat::RGBA16161616F)
				RGBA16161616F++;
			else if (ImageForm == ImageFormat::RGBA8888)
				RGBA8888++;
			else if (ImageForm == ImageFormat::UV88)
				UV88++;
			else if (ImageForm == ImageFormat::UVLX8888)
				UVLX8888++;
			else if (ImageForm == ImageFormat::UVWQ8888)
				UVWQ8888++;

			if (static_cast<unsigned int>(Flags) == 0)
				FlagZero++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::POINTSAMPLING)) != 0)
				POINTSAMPLING++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::TRILINEARSAMPLING)) != 0)
				TRILINEARSAMPLING++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::CLAMPS)) != 0)
				CLAMPS++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::CLAMPT)) != 0)
				CLAMPT++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::ANISOTROPICSAMPLING)) != 0)
				ANISOTROPICSAMPLING++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::HINTDXT5)) != 0)
				HINTDXT5++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NOCOMPRESS)) != 0)
				NOCOMPRESS++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NORMALMAP)) != 0)
				NORMALMAP++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NOMIPMAPS)) != 0)
				NOMIPMAPS++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NOLOD)) != 0)
				NOLOD++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NOMINIMUMMIPMAP)) != 0)
				NOMINIMUMMIPMAP++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::PROCEDURAL)) != 0)
				PROCEDURAL++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::ONEBITALPHA)) != 0)
				ONEBITALPHA++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::EIGHTBITALHPA)) != 0)
				EIGHTBITALHPA++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::ENVIRONMENTMAP)) != 0)
				ENVIRONMENTMAP++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::RENDERTARGET)) != 0)
				RENDERTARGET++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::DEPTHRENDERTARGET)) != 0)
				DEPTHRENDERTARGET++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NODEBUGOVERRIDE)) != 0)
				NODEBUGOVERRIDE++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::SINGLECOPY)) != 0)
				SINGLECOPY++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::PRESRGB)) != 0)
				PRESRGB++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NODEPTHBUFFER)) != 0)
				NODEPTHBUFFER++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::NICEFILTERED)) != 0)
				NICEFILTERED++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::CLAMPU)) != 0)
				CLAMPU++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::VERTEXTEXTURE)) != 0)
				VERTEXTEXTURE++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::SSBUMP)) != 0)
				SSBUMP++;
			if ((static_cast<unsigned int>(Flags) & static_cast<unsigned int>(ImageFlags::BORDER)) != 0)
				BORDER++;

		i++;
	}

	std::cout << "\nCompleted processing " << i << " files.\n"
		<< "== Version Spread ==\n"
		<< "7.0: " << VerSevZero << '\n'
		<< "7.1: " << VerSevOne << '\n'
		<< "7.2: " << VerSevTwo << '\n'
		<< "7.3: " << VerSevThree << '\n'
		<< "7.4: " << VerSevFour << '\n'
		<< "7.5: " << VerSevFive << '\n'
		<< "Unknown/Other: " << VerUnknown << "\n\n"


		<< "== Format Spread ==\n"
		<< "NONE: " << NONE << '\n'
		<< "RGBA8888: " << RGBA8888 << '\n'
		<< "ABGR8888: " << ABGR8888 << '\n'
		<< "RGB888: " << RGB888 << '\n'
		<< "BGR888: " << BGR888 << '\n'
		<< "RGB565: " << RGB565 << '\n'
		<< "I8: " << I8 << '\n'
		<< "IA88: " << IA88 << '\n'
		<< "P8: " << P8 << '\n'
		<< "A8: " << A8 << '\n'
		<< "RGB888_BLUESCREEN: " << RGB888_BLUESCREEN << '\n'
		<< "BGR888_BLUESCREEN: " << BGR888_BLUESCREEN << '\n'
		<< "ARGB8888: " << ARGB8888 << '\n'
		<< "BGRA8888: " << BGRA8888 << '\n'
		<< "DXT1: " << DXT1 << '\n'
		<< "DXT3: " << DXT3 << '\n'
		<< "DXT5: " << DXT5 << '\n'
		<< "BGRX8888: " << BGRX8888 << '\n'
		<< "BGR565: " << BGR565 << '\n'
		<< "BGRX5551: " << BGRX5551 << '\n'
		<< "BGRA4444: " << BGRA4444 << '\n'
		<< "DXT1_ONEBITALPHA: " << DXT1_ONEBITALPHA << '\n'
		<< "BGRA5551: " << BGRA5551 << '\n'
		<< "UV88: " << UV88 << '\n'
		<< "UVWQ8888: " << UVWQ8888 << '\n'
		<< "RGBA16161616F: " << RGBA16161616F << '\n'
		<< "RGBA16161616: " << RGBA16161616 << '\n'
		<< "UVLX8888: " << UVLX8888 << '\n'
		<< "Unknown/Other: " << FormUnk << "\n\n"

		<< "== Flag Spread ==\n"
		<< "POINTSAMPLING: " << POINTSAMPLING << '\n'
		<< "TRILINEARSAMPLING: " << TRILINEARSAMPLING << '\n'
		<< "CLAMPS: " << CLAMPS << '\n'
		<< "CLAMPT: " << CLAMPT << '\n'
		<< "ANISOTROPICSAMPLING: " << ANISOTROPICSAMPLING << '\n'
		<< "HINTDXT5: " << HINTDXT5 << '\n'
		<< "NOCOMPRESS: " << NOCOMPRESS << '\n'
		<< "NORMALMAP: " << NORMALMAP << '\n'
		<< "NOMIPMAPS: " << NOMIPMAPS << '\n'
		<< "NOLOD: " << NOLOD << '\n'
		<< "NOMINIMUMMIPMAP: " << NOMINIMUMMIPMAP << '\n'
		<< "PROCEDURAL: " << PROCEDURAL << '\n'
		<< "ONEBITALPHA: " << ONEBITALPHA << '\n'
		<< "EIGHTBITALHPA: " << EIGHTBITALHPA << '\n'
		<< "ENVIRONMENTMAP: " << ENVIRONMENTMAP << '\n'
		<< "RENDERTARGET: " << RENDERTARGET << '\n'
		<< "DEPTHRENDERTARGET: " << DEPTHRENDERTARGET << '\n'
		<< "NODEBUGOVERRIDE: " << NODEBUGOVERRIDE << '\n'
		<< "SINGLECOPY: " << SINGLECOPY << '\n'
		<< "PRESRGB: " << PRESRGB << '\n'
		<< "NODEPTHBUFFER: " << NODEPTHBUFFER << '\n'
		<< "NICEFILTERED: " << NICEFILTERED << '\n'
		<< "CLAMPU: " << CLAMPU << '\n'
		<< "VERTEXTEXTURE: " << VERTEXTEXTURE << '\n'
		<< "SSBUMP: " << SSBUMP << '\n'
		<< "BORDER: " << BORDER << '\n'
		<< "None: " << FlagZero << "\n\n";

	return 0;
}