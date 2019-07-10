#include "codecs.h"
#include "luts.h"

#include <cmath>

xvtf::PixelFormats::RGB888 xvtf::Codecs::Mix(const xvtf::PixelFormats::RGB888& _A, const xvtf::PixelFormats::RGB888& _B, uint32 PartsA, uint32 PartsB)
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

	xvtf::PixelFormats::RGB888 RVAL;
	RVAL.R = static_cast<uchar>(R_R);
	RVAL.G = static_cast<uchar>(R_G);
	RVAL.B = static_cast<uchar>(R_B);

	return RVAL;
}

xvtf::PixelFormats::RGBA8888 xvtf::Codecs::Mix(const xvtf::PixelFormats::RGBA8888& _A, const xvtf::PixelFormats::RGBA8888& _B, uint32 PartsA, uint32 PartsB)
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

	xvtf::PixelFormats::RGBA8888 RVAL;
	RVAL.R = static_cast<uchar>(R_R);
	RVAL.G = static_cast<uchar>(R_G);
	RVAL.B = static_cast<uchar>(R_B);
	RVAL.A = static_cast<uchar>(R_A);

	return RVAL;
}

void* xvtf::Codecs::DecompressDXT1(const void* buffer, addressable offset, uint16 width, uint16 height)
{
	using namespace xvtf::PixelFormats;

	uint32 WIDTH = width < 4 ? 4 : width;
	uint32 HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGB888* rBuffer = (RGB888*)malloc(sizeof(RGB888) * WIDTH * HEIGHT);
	uchar* bPtr = (uchar*)(buffer) + offset;

	// For Each Block
	for (uint32 b = 0; b < (WIDTH * HEIGHT/ 16); b++)
	{
		uint16* c_0 = (uint16*)bPtr; bPtr += 2;
		uint16* c_1 = (uint16*)bPtr; bPtr += 2;

		uchar c_0_r = (*c_0 >> 11) & 0x1F;
		uchar c_0_g = (*c_0 >> 5) & 0x3F;
		uchar c_0_b = *c_0 & 0x1F;

		uchar c_1_r = (*c_1 >> 11) & 0x1F;
		uchar c_1_g = (*c_1 >> 5) & 0x3F;
		uchar c_1_b = *c_1 & 0x1F;

		RGB888 Colour0;
		Colour0.R = LUT::LUT5[c_0_r];
		Colour0.G = LUT::LUT6[c_0_g];
		Colour0.B = LUT::LUT5[c_0_b];

		RGB888 Colour1;
		Colour1.R = LUT::LUT5[c_1_r];
		Colour1.G = LUT::LUT6[c_1_g];
		Colour1.B = LUT::LUT5[c_1_b];

		RGB888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGB888 Colour3 = Mix(Colour0, Colour1, 1, 2);

		uint32 Row = (b / (WIDTH / 4)) * 4;
		uint32 Col = (b % (WIDTH / 4)) * 4;

		// For Each Row (in block)
		for (uchar bR = 0; bR < 4; bR++)
		{
			uchar* cData = (uchar*)bPtr; bPtr += 1;

			// For Each Column (in block row)
			for (uchar bC = 0; bC < 4; bC++)
			{
				uchar Pix = (*cData & (0b11000000 >> (bC * 2))) >> ((3 - bC) * 2);
				
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

void* xvtf::Codecs::DecompressDXT1_ONEBITALPHA(const void* buffer, addressable offset, uint16 width, uint16 height)
{
	using namespace xvtf::PixelFormats;

	uint32 WIDTH = width < 4 ? 4 : width;
	uint32 HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGBA8888* rBuffer = (RGBA8888*)malloc(sizeof(RGBA8888) * WIDTH * HEIGHT);
	uchar* bPtr = (uchar*)(buffer)+offset;

	// For Each Block
	for (uint32 b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		uint16* c_0 = (uint16*)bPtr; bPtr += 2;
		uint16* c_1 = (uint16*)bPtr; bPtr += 2;

		uchar c_0_r = (*c_0 >> 11) & 0x1F;
		uchar c_0_g = (*c_0 >> 5) & 0x3F;
		uchar c_0_b = *c_0 & 0x1F;

		uchar c_1_r = (*c_1 >> 11) & 0x1F;
		uchar c_1_g = (*c_1 >> 5) & 0x3F;
		uchar c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT::LUT5[c_0_r];
		Colour0.G = LUT::LUT6[c_0_g];
		Colour0.B = LUT::LUT5[c_0_b];
		Colour0.A = 0xFF;

		RGBA8888 Colour1;
		Colour1.R = LUT::LUT5[c_1_r];
		Colour1.G = LUT::LUT6[c_1_g];
		Colour1.B = LUT::LUT5[c_1_b];
		Colour1.A = 0xFF;

		RGBA8888 Colour2, Colour3;

		// 4 Colour Mode (No Transparency)
		if (*c_0 > *c_1)
		{
			Colour2 = Mix(Colour0, Colour1, 2, 1);
			Colour3 = Mix(Colour0, Colour1, 1, 2);
		}
		else // 3 Colour Mode (1-bit Alpha)
		{
			Colour2 = Mix(Colour0, Colour1, 1, 1);
			Colour3.R = 0; Colour3.G = 0; Colour3.B = 0; Colour3.A = 0;
		}

		uint32 Row = (b / (WIDTH / 4)) * 4;
		uint32 Col = (b % (WIDTH / 4)) * 4;

		// For Each Row (in block)
		for (uchar bR = 0; bR < 4; bR++)
		{
			uchar* cData = (uchar*)bPtr; bPtr += 1;

			// For Each Column (in block row)
			for (uchar bC = 0; bC < 4; bC++)
			{
				uchar Pix = (*cData & (0b11000000 >> (bC * 2))) >> ((3 - bC) * 2);

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

void* xvtf::Codecs::DecompressDXT3(const void* buffer, addressable offset, uint16 width, uint16 height)
{
	using namespace xvtf::PixelFormats;

	uint32 WIDTH = width < 4 ? 4 : width;
	uint32 HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGBA8888* rBuffer = (RGBA8888*)malloc(sizeof(RGBA8888) * WIDTH * HEIGHT);
	uchar* bPtr = (uchar*)(buffer)+offset;

	// For Each Block
	for (uint32 b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		uchar A[16];
		// Read in Alpha Data
		for (uchar i = 0; i < 4; ++i)
		{
			uint16* VAL = (uint16*)bPtr; bPtr += 2;
			A[i * 4] = LUT::LUT4[(*VAL >> 12) & 0xF];
			A[i * 4 + 1] = LUT::LUT4[(*VAL >> 8) & 0xF];
			A[i * 4 + 2] = LUT::LUT4[(*VAL >> 4) & 0xF];
			A[i * 4 + 3] = LUT::LUT4[*VAL & 0xF];
		}

		uint16* c_0 = (uint16*)bPtr; bPtr += 2;
		uint16* c_1 = (uint16*)bPtr; bPtr += 2;

		uchar c_0_r = (*c_0 >> 11) & 0x1F;
		uchar c_0_g = (*c_0 >> 5) & 0x3F;
		uchar c_0_b = *c_0 & 0x1F;

		uchar c_1_r = (*c_1 >> 11) & 0x1F;
		uchar c_1_g = (*c_1 >> 5) & 0x3F;
		uchar c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT::LUT5[c_0_r];
		Colour0.G = LUT::LUT6[c_0_g];
		Colour0.B = LUT::LUT5[c_0_b];

		RGBA8888 Colour1;
		Colour1.R = LUT::LUT5[c_1_r];
		Colour1.G = LUT::LUT6[c_1_g];
		Colour1.B = LUT::LUT5[c_1_b];

		RGBA8888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGBA8888 Colour3 = Mix(Colour0, Colour1, 1, 2);

		uint32 Row = (b / (WIDTH / 4)) * 4;
		uint32 Col = (b % (WIDTH / 4)) * 4;

		// For Each Row (in block)
		for (uchar bR = 0; bR < 4; bR++)
		{
			uchar* cData = (uchar*)bPtr; bPtr += 1;

			// For Each Column (in block row)
			for (uchar bC = 0; bC < 4; bC++)
			{
				uchar Pix = (*cData & (0b11000000 >> (bC * 2))) >> ((3 - bC) * 2);

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

void* xvtf::Codecs::DecompressDXT5(const void* buffer, addressable offset, uint16 width, uint16 height)
{
	using namespace xvtf::PixelFormats;

	uint32 WIDTH = width < 4 ? 4 : width;
	uint32 HEIGHT = height < 4 ? 4 : height;

	// Create Buffer
	RGBA8888* rBuffer = (RGBA8888*)malloc(sizeof(RGBA8888) * WIDTH * HEIGHT);
	uchar* bPtr = (uchar*)(buffer)+offset;

	// For Each Block
	for (uint32 b = 0; b < (WIDTH * HEIGHT / 16); b++)
	{
		// Get Alphas
		uchar A[8];
		A[0] = *bPtr; ++bPtr;
		A[1] = *bPtr; ++bPtr;
		float af_0 = static_cast<float>(A[0]);
		float af_1 = static_cast<float>(A[1]);

		if (A[0] > A[1])
		{
			A[2] = static_cast<uchar>(std::round((6 * af_0 + af_1) / 7));
			A[3] = static_cast<uchar>(std::round((5 * af_0 + 2 * af_1) / 7));
			A[4] = static_cast<uchar>(std::round((4 * af_0 + 3 * af_1) / 7));
			A[5] = static_cast<uchar>(std::round((3 * af_0 + 4 * af_1) / 7));
			A[6] = static_cast<uchar>(std::round((2 * af_0 + 5 * af_1) / 7));
			A[7] = static_cast<uchar>(std::round((af_0 + 6 * af_1) / 7));
		}
		else
		{
			A[2] = static_cast<uchar>(std::round((4 * af_0 + af_1) / 5));
			A[3] = static_cast<uchar>(std::round((3 * af_0 + 2 * af_1) / 5));
			A[4] = static_cast<uchar>(std::round((2 * af_0 + 3 * af_1) / 5));
			A[5] = static_cast<uchar>(std::round((af_0 + 4 * af_1) / 5));
			A[6] = 0x0;
			A[7] = 0xFF;
		}

		// Get Alphas
		uchar PixA[16];
		
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

		uint16* c_0 = (uint16*)bPtr; bPtr += 2;
		uint16* c_1 = (uint16*)bPtr; bPtr += 2;

		uchar c_0_r = (*c_0 >> 11) & 0x1F;
		uchar c_0_g = (*c_0 >> 5) & 0x3F;
		uchar c_0_b = *c_0 & 0x1F;

		uchar c_1_r = (*c_1 >> 11) & 0x1F;
		uchar c_1_g = (*c_1 >> 5) & 0x3F;
		uchar c_1_b = *c_1 & 0x1F;

		RGBA8888 Colour0;
		Colour0.R = LUT::LUT5[c_0_r];
		Colour0.G = LUT::LUT6[c_0_g];
		Colour0.B = LUT::LUT5[c_0_b];

		RGBA8888 Colour1;
		Colour1.R = LUT::LUT5[c_1_r];
		Colour1.G = LUT::LUT6[c_1_g];
		Colour1.B = LUT::LUT5[c_1_b];

		RGBA8888 Colour2 = Mix(Colour0, Colour1, 2, 1);
		RGBA8888 Colour3 = Mix(Colour0, Colour1, 1, 2);

		uint32 Row = (b / (WIDTH / 4)) * 4;
		uint32 Col = (b % (WIDTH / 4)) * 4;

		// For Each Row (in block)
		for (uchar bR = 0; bR < 4; bR++)
		{
			uchar* cData = (uchar*)bPtr; bPtr += 1;

			// For Each Column (in block row)
			for (uchar bC = 0; bC < 4; bC++)
			{
				uchar Pix = (*cData & (0b11000000 >> (bC * 2))) >> ((3 - bC) * 2);

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