#include "xVTF/xCodecs.h"

XVTF_NS::Codec::RGB888 operator*(const XVTF_NS::Codec::RGB888& lhs, const double& rhs)
{
	XVTF_NS::Codec::RGB888 rval = lhs;
	rval.R = static_cast<unsigned char>(static_cast<double>(rval.R) * rhs);
	rval.G = static_cast<unsigned char>(static_cast<double>(rval.G) * rhs);
	rval.B = static_cast<unsigned char>(static_cast<double>(rval.B) * rhs);

	return rval;
}
XVTF_NS::Codec::RGB888 operator*(const double& lhs, const XVTF_NS::Codec::RGB888& rhs) { return rhs * lhs; }

XVTF_NS::Codec::RGB888 operator+(const XVTF_NS::Codec::RGB888& lhs, const XVTF_NS::Codec::RGB888& rhs)
{
	XVTF_NS::Codec::RGB888 rval = lhs;
	rval.R += rhs.R;
	rval.G += rhs.G;
	rval.B += rhs.B;
	return rval;
}

void* XVTF_NS::Codec::DecompressDXT1(void* buffer, const unsigned int& offset, const unsigned int& width, const unsigned int& height)
{
#pragma warning( push )
#pragma warning( disable : 4838 4309)
	// LUT's for quick conversion to RGB888
	const char LUT5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
		                 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };
	const char LUT6[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 45, 49, 53, 57, 61, 65, 69, 73,
		                 77, 81, 85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 130, 134, 138,
		                 142, 146, 150, 154, 158, 162, 166, 170, 174, 178, 182, 186, 190, 194, 198,
		                 202, 206, 210, 215, 219, 223, 227, 231, 235, 239, 243, 247, 251, 255 };
#pragma warning( pop )

	const unsigned int WIDTH = width < 4 ? 4 : width;
	const unsigned int HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGB888* rBuffer = new RGB888[WIDTH * HEIGHT];
	char* bPtr = (char*)(buffer) + offset;

	// For Each Block
	for (unsigned int b = 0; b < (WIDTH * HEIGHT/ 16); b++)
	{
		unsigned short* c_0 = (unsigned short*)bPtr; bPtr += 2;
		unsigned short* c_1 = (unsigned short*)bPtr; bPtr += 2;

		unsigned char c_0_r = (*c_0 >> 11) & 0x1F;
		unsigned char c_0_g = (*c_0 >> 5) & 0x3F;
		unsigned char c_0_b = *c_0 & 0x1F;

		unsigned char c_1_r = (*c_1 >> 11) & 0x1F;
		unsigned char c_1_g = (*c_1 >> 5) & 0x3F;
		unsigned char c_1_b = *c_1 & 0x1F;

		RGB888 Colour0;
		Colour0.R = LUT5[c_0_r];
		Colour0.G = LUT6[c_0_g];
		Colour0.B = LUT5[c_0_b];

		RGB888 Colour1;
		Colour1.R = LUT5[c_1_r];
		Colour1.G = LUT6[c_1_g];
		Colour1.B = LUT5[c_1_b];

		RGB888 Colour2 = (2.0/3) * Colour0 + (1.0/3) * Colour1;
		RGB888 Colour3 = (1.0/3) * Colour0 + (2.0/3) * Colour1;

		unsigned int Row = (b / (WIDTH / 4)) * 4;
		unsigned int Col = (b % (WIDTH / 4)) * 4;

		// For Each Row (in block)
		for (unsigned int bR = 0; bR < 4; bR++)
		{
			unsigned char* cData = (unsigned char*)bPtr; bPtr += 1;

			// For Each Column (in block row)
			for (unsigned int bC = 0; bC < 4; bC++)
			{
				unsigned char Pix = (*cData & (0b11000000 >> (bC * 2))) >> ((3 - bC) * 2);
				
				RGB888 Colour;
				if (Pix == 0b00)
					Colour = Colour0;
				else if (Pix == 0b01)
					Colour = Colour1;
				else if (Pix == 0b10)
					Colour = Colour2;
				else
					Colour = Colour3;

				*(rBuffer + ((Row + bR) * WIDTH) + (Col + (3 - bC))) = Colour;
			}
		}
	}

	return (void*)rBuffer;
}