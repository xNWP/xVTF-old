#include "xVTF/xCodecs.h"

XVTF_NS::Codec::RGB888 XVTF_NS::Codec::Mix(const XVTF_NS::Codec::RGB888& _A, const XVTF_NS::Codec::RGB888& _B, const unsigned int& PartsA, const unsigned int& PartsB)
{
	float A_R = static_cast<float>(_A.R);
	float A_G = static_cast<float>(_A.G);
	float A_B = static_cast<float>(_A.B);

	float B_R = static_cast<float>(_B.R);
	float B_G = static_cast<float>(_B.G);
	float B_B = static_cast<float>(_B.B);

	float R_R = ((PartsA * A_R) + (PartsB * B_R)) / (PartsA + PartsB);
	float R_G = ((PartsA * A_G) + (PartsB * B_G)) / (PartsA + PartsB);
	float R_B = ((PartsA * A_B) + (PartsB * B_B)) / (PartsA + PartsB);

	/* Normalize Values */
	R_R = ceilf(R_R);
	R_G = ceilf(R_G);
	R_B = ceilf(R_B);

	if (R_R > 255)
		R_R = 255;
	if (R_G > 255)
		R_G = 255;
	if (R_B > 255)
		R_B = 255;

	XVTF_NS::Codec::RGB888 RVAL;
	RVAL.R = static_cast<unsigned char>(R_R);
	RVAL.G = static_cast<unsigned char>(R_G);
	RVAL.B = static_cast<unsigned char>(R_B);

	return RVAL;
}

XVTF_NS::Codec::RGBA8888 XVTF_NS::Codec::Mix(const XVTF_NS::Codec::RGBA8888& _A, const XVTF_NS::Codec::RGBA8888& _B, const unsigned int& PartsA, const unsigned int& PartsB)
{
	float A_R = static_cast<float>(_A.R);
	float A_G = static_cast<float>(_A.G);
	float A_B = static_cast<float>(_A.B);
	float A_A = static_cast<float>(_A.A);

	float B_R = static_cast<float>(_B.R);
	float B_G = static_cast<float>(_B.G);
	float B_B = static_cast<float>(_B.B);
	float B_A = static_cast<float>(_B.A);

	float R_R = ((PartsA * A_R) + (PartsB * B_R)) / (PartsA + PartsB);
	float R_G = ((PartsA * A_G) + (PartsB * B_G)) / (PartsA + PartsB);
	float R_B = ((PartsA * A_B) + (PartsB * B_B)) / (PartsA + PartsB);
	float R_A = ((PartsA * A_A) + (PartsB * B_A)) / (PartsA + PartsB);

	/* Normalize Values */
	R_R = ceilf(R_R);
	R_G = ceilf(R_G);
	R_B = ceilf(R_B);
	R_A = ceilf(R_A);

	if (R_R > 255)
		R_R = 255;
	if (R_G > 255)
		R_G = 255;
	if (R_B > 255)
		R_B = 255;
	if (R_A > 255)
		R_A = 255;

	XVTF_NS::Codec::RGBA8888 RVAL;
	RVAL.R = static_cast<unsigned char>(R_R);
	RVAL.G = static_cast<unsigned char>(R_G);
	RVAL.B = static_cast<unsigned char>(R_B);
	RVAL.A = static_cast<unsigned char>(R_A);

	return RVAL;
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

		RGB888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGB888 Colour3 = Mix(Colour0, Colour1, 1, 2);

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

void* XVTF_NS::Codec::DecompressDXT1_ONEBITALPHA(void* buffer, const unsigned int& offset, const unsigned int& width, const unsigned int& height)
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
	RGBA8888* rBuffer = new RGBA8888[WIDTH * HEIGHT];
	char* bPtr = (char*)(buffer)+offset;

	// For Each Block
	for (unsigned int b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		unsigned short* c_0 = (unsigned short*)bPtr; bPtr += 2;
		unsigned short* c_1 = (unsigned short*)bPtr; bPtr += 2;

		unsigned char c_0_r = (*c_0 >> 11) & 0x1F;
		unsigned char c_0_g = (*c_0 >> 5) & 0x3F;
		unsigned char c_0_b = *c_0 & 0x1F;

		unsigned char c_1_r = (*c_1 >> 11) & 0x1F;
		unsigned char c_1_g = (*c_1 >> 5) & 0x3F;
		unsigned char c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT5[c_0_r];
		Colour0.G = LUT6[c_0_g];
		Colour0.B = LUT5[c_0_b];
		Colour0.A = 0xFF;

		RGBA8888 Colour1;
		Colour1.R = LUT5[c_1_r];
		Colour1.G = LUT6[c_1_g];
		Colour1.B = LUT5[c_1_b];
		Colour1.A = 0xFF;

		RGBA8888 Colour2, Colour3;

		// 4 Colour Mode (No Transparency)
		if (c_0 > c_1)
		{
			Colour2 = Mix(Colour0, Colour1, 2, 1);
			Colour3 = Mix(Colour0, Colour1, 1, 2);
		}
		else // 3 Colour Mode (1-bit Alpha)
		{
			Colour2 = Mix(Colour0, Colour1, 1, 1);
			Colour3.R = 0; Colour3.G = 0; Colour3.B = 0; Colour3.A = 0;
		}

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

				RGBA8888 Colour;
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

void* XVTF_NS::Codec::DecompressDXT3(void* buffer, const unsigned int& offset, const unsigned int& width, const unsigned int& height)
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
	const char LUT4[] = { 0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255 };
#pragma warning( pop )

	const unsigned int WIDTH = width < 4 ? 4 : width;
	const unsigned int HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGBA8888* rBuffer = new RGBA8888[WIDTH * HEIGHT];
	char* bPtr = (char*)(buffer)+offset;

	// For Each Block
	for (unsigned int b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		unsigned char A[16];
		// Read in Alpha Data
		for (unsigned char i = 0; i < 4; ++i)
		{
			unsigned short* VAL = (unsigned short*)bPtr; bPtr += 2;
			A[i * 4] = LUT4[(*VAL >> 12) & 0xF];
			A[i * 4 + 1] = LUT4[(*VAL >> 8) & 0xF];
			A[i * 4 + 2] = LUT4[(*VAL >> 4) & 0xF];
			A[i * 4 + 3] = LUT4[*VAL & 0xF];
		}

		unsigned short* c_0 = (unsigned short*)bPtr; bPtr += 2;
		unsigned short* c_1 = (unsigned short*)bPtr; bPtr += 2;

		unsigned char c_0_r = (*c_0 >> 11) & 0x1F;
		unsigned char c_0_g = (*c_0 >> 5) & 0x3F;
		unsigned char c_0_b = *c_0 & 0x1F;

		unsigned char c_1_r = (*c_1 >> 11) & 0x1F;
		unsigned char c_1_g = (*c_1 >> 5) & 0x3F;
		unsigned char c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT5[c_0_r];
		Colour0.G = LUT6[c_0_g];
		Colour0.B = LUT5[c_0_b];

		RGBA8888 Colour1;
		Colour1.R = LUT5[c_1_r];
		Colour1.G = LUT6[c_1_g];
		Colour1.B = LUT5[c_1_b];

		RGBA8888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGBA8888 Colour3 = Mix(Colour0, Colour1, 1, 2);

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

				RGBA8888 Colour;
				if (Pix == 0b00)
					Colour = Colour0;
				else if (Pix == 0b01)
					Colour = Colour1;
				else if (Pix == 0b10)
					Colour = Colour2;
				else
					Colour = Colour3;
				Colour.A = A[4 * bR + bC];

				*(rBuffer + ((Row + bR) * WIDTH) + (Col + (3 - bC))) = Colour;
			}
		}
	}

	return (void*)rBuffer;
}

void* XVTF_NS::Codec::DecompressDXT5(void* buffer, const unsigned int& offset, const unsigned int& width, const unsigned int& height)
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
	RGBA8888* rBuffer = new RGBA8888[WIDTH * HEIGHT];
	unsigned char* bPtr = (unsigned char*)(buffer)+offset;

	// For Each Block
	for (unsigned int b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		// Get Alphas
		unsigned char A[8];
		A[0] = *bPtr; ++bPtr;
		A[1] = *bPtr; ++bPtr;
		float af_0 = static_cast<float>(A[0]);
		float af_1 = static_cast<float>(A[1]);

		if (A[0] > A[1])
		{
			A[2] = static_cast<unsigned char>(std::round((6 * af_0 + af_1) / 7));
			A[3] = static_cast<unsigned char>(std::round((5 * af_0 + 2 * af_1) / 7));
			A[4] = static_cast<unsigned char>(std::round((4 * af_0 + 3 * af_1) / 7));
			A[5] = static_cast<unsigned char>(std::round((3 * af_0 + 4 * af_1) / 7));
			A[6] = static_cast<unsigned char>(std::round((2 * af_0 + 5 * af_1) / 7));
			A[7] = static_cast<unsigned char>(std::round((af_0 + 6 * af_1) / 7));
		}
		else
		{
			A[2] = static_cast<unsigned char>(std::round((4 * af_0 + af_1) / 5));
			A[3] = static_cast<unsigned char>(std::round((3 * af_0 + 2 * af_1) / 5));
			A[4] = static_cast<unsigned char>(std::round((2 * af_0 + 3 * af_1) / 5));
			A[5] = static_cast<unsigned char>(std::round((af_0 + 4 * af_1) / 5));
			A[6] = 0x0;
			A[7] = 0xFF;
		}

		// Get Alphas
		unsigned char PixA[16];
		
		bPtr += 5;
		PixA[15] = A[(*bPtr & 0b11100000) >> 5];
		PixA[14] = A[(*bPtr & 0b00011100) >> 2];
		PixA[13] = (*bPtr & 0b00000011) << 1; --bPtr;
		PixA[13] += (*bPtr & 0b10000000) >> 7;
		PixA[13] = A[PixA[13]];
		PixA[12] = A[(*bPtr & 0b01110000) >> 4];
		PixA[11] = A[(*bPtr & 0b00001110) >> 1];
		PixA[10] = (*bPtr & 0b00000001) << 2; --bPtr;
		PixA[10] += (*bPtr & 0b11000000) >> 6;
		PixA[10] = A[PixA[10]];
		PixA[9] = A[(*bPtr & 0b00111000) >> 3];
		PixA[8] = A[(*bPtr & 0b00000111)]; --bPtr;
		PixA[7] = A[(*bPtr & 0b11100000) >> 5];
		PixA[6] = A[(*bPtr & 0b00011100) >> 2];
		PixA[5] = (*bPtr & 0b00000011) << 1; --bPtr;
		PixA[5] += (*bPtr & 0b10000000) >> 7;
		PixA[5] = A[PixA[5]];
		PixA[4] = A[(*bPtr & 0b01110000) >> 4];
		PixA[3] = A[(*bPtr & 0b00001110) >> 1];
		PixA[2] = (*bPtr & 0b00000001) << 2; --bPtr;
		PixA[2] += (*bPtr & 0b11000000) >> 6;
		PixA[2] = A[PixA[2]];
		PixA[1] = A[(*bPtr & 0b00111000) >> 3];
		PixA[0] = A[(*bPtr & 0b00000111)];
		bPtr += 6;

		unsigned short* c_0 = (unsigned short*)bPtr; bPtr += 2;
		unsigned short* c_1 = (unsigned short*)bPtr; bPtr += 2;

		unsigned char c_0_r = (*c_0 >> 11) & 0x1F;
		unsigned char c_0_g = (*c_0 >> 5) & 0x3F;
		unsigned char c_0_b = *c_0 & 0x1F;

		unsigned char c_1_r = (*c_1 >> 11) & 0x1F;
		unsigned char c_1_g = (*c_1 >> 5) & 0x3F;
		unsigned char c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT5[c_0_r];
		Colour0.G = LUT6[c_0_g];
		Colour0.B = LUT5[c_0_b];

		RGBA8888 Colour1;
		Colour1.R = LUT5[c_1_r];
		Colour1.G = LUT6[c_1_g];
		Colour1.B = LUT5[c_1_b];

		RGBA8888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGBA8888 Colour3 = Mix(Colour0, Colour1, 1, 2);

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

				RGBA8888 Colour;
				if (Pix == 0b00)
					Colour = Colour0;
				else if (Pix == 0b01)
					Colour = Colour1;
				else if (Pix == 0b10)
					Colour = Colour2;
				else
					Colour = Colour3;

				Colour.A = PixA[bR * 4 + (3 - bC)];

				*(rBuffer + ((Row + bR) * WIDTH) + (Col + (3 - bC))) = Colour;
			}
		}
	}

	return (void*)rBuffer;
}