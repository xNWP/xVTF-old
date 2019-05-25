#include "xVTF/half_precision_float.h"

float xvtf::half::toFloat() const
{
	unsigned char rexp = this->exp + 112;
	unsigned short int man = this->mantissa;
	unsigned char s = this->sign;

	unsigned char dat[4];
	dat[3] = s << 7;
	dat[3] += ((rexp & 0xFE) >> 1);
	dat[2] = ((rexp & 0x1) << 7);
	dat[2] += ((man & 0xFE) >> 1);
	dat[1] = ((man & 0x1) << 7);
	dat[1] += ((man & 0xFE00) >> 9);
	dat[0] = 0;
	dat[0] += ((man & 0x100) >> 1);

	float out = *(float*)&dat;

	return out;
}