#include "xVTF/xHalfPrecisionFloat.h"

float xvtf::half::toFloat() const
{
	unsigned char rexp = this->exp + 112;
	unsigned int man = this->mantissa;
	unsigned char s = 1;

	char dat[4];
	dat[3] = s << 7;
	dat[3] += ((rexp & 0xFE) >> 1);
	dat[2] = (rexp & 0x1) << 7;
	dat[2] += (man & 0xFE000000) >> 25;
	dat[1] = (man & 0x1FE0000) >> 17;
	dat[0] = (man & 0x1FE00) >> 9;

	float test = 0.33333333333333;

	return float();
}