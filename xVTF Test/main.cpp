#include <iostream>
#include "xVTF/xVTF.h"

int main(unsigned int argc, char** argv)
{
	auto test = xvtf::Bitmap::VTFFile::Alloc("C:\\Users\\AORUS\\Desktop\\CSMATS\\materials\\brick\\brick_ext_03.vtf");

	test->Alloc("blah");

	return 0;
}