#include <iostream>
#include "xVTF/xVTF.h"

int main(unsigned int argc, char** argv)
{
	auto test = xvtf::Bitmap::VTFFile::Alloc("C:\\Users\\AORUS\\Desktop\\CSMATS\\materials\\brick\\brick_ext_03.vtf");

	auto res = test->GetResolutions()[0];

	std::cout << res.Width << ' ' << res.Height;

	auto BI = test->GetImage();

	auto pix = (*BI)[0];

	xvtf::Bitmap::PixelFormats::RGB888 data = *(xvtf::Bitmap::PixelFormats::RGB888*)pix;

	std::cout << ' ' << (unsigned int)data.R << ' ' << (unsigned int)data.G << ' ' << (unsigned int)data.B;

	xvtf::Bitmap::VTFFile::Free(test);

	return 0;
}