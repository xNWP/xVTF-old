#include "xVTF/half_precision_float.h"

#include "xVTF/error.h"

xvtf::half::half(float f)
{
	this->setFloat(f);
}

float xvtf::half::toFloat(unsigned int * const & xvtferrno) const
{
	// Assume no error
	XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);

	// Is Infinite?
	if (this->exp == 0x1F && this->mantissa == 0)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_INF);
		// Return the maximum/minimum size half float
		return 65504.0f * (this->sign == 1 ? -1.0f : 1.0f);
	}
	// Is NaN?
	else if (this->exp == 0x1F && this->mantissa != 0)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_NAN);
		return 0.0f;
	}
	// Is Zero?
	else if (this->exp == 0 && this->mantissa == 0)
	{
		return this->sign == 1 ? -0.0f : 0.0f;
	}
	// Is Subnormal?
	else if (this->exp == 0 && this->mantissa != 0)
	{
		float dec = this->mantissa / 1024.0f;
		return (this->sign == 1 ? -1.0f : 1.0f) * dec / 16384.0f;
	}
	// Normal Number
	else
	{
		unsigned int exp = ((unsigned int)this->exp + 112) << 23;
		unsigned int s = ((unsigned int)this->sign) << 31;
		unsigned int mantissa = ((unsigned int)this->mantissa) << 13;
		unsigned int out = s | exp | mantissa;
		return *(float*)&out;
	}

	XVTF_SETERROR(xvtferrno, ERRORCODE::UNKNOWN);
	return 0;
}

bool xvtf::half::setFloat(float f, unsigned int * const & xvtferrno)
{
	unsigned int fUint = *(unsigned int*)&f;
	unsigned int sign = (fUint & 0x80000000) >> 31;
	unsigned int exp = (fUint & 0x7F800000) >> 23;
	unsigned int mantissa = (fUint & 0x7FFFFF);

	// Zero / Subnormal?
	if (exp == 0)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);
		this->rawBits = (sign << 15) | 0x0;
		return true;
	}
	// +/- Infinity?
	else if (exp == 0xFF && mantissa == 0)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_INF);
		this->rawBits = sign == 1 ? 0xFBFF : 0x7BFF;
		return false;
	}
	// NaN?
	else if (exp == 0xFF && mantissa != 0)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_NAN);
		this->rawBits = 0x0;
		return false;
	}
	// Is too big for us to hold?
	else if (f > 65504)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_CLAMPED);
		this->rawBits = 0x7BFF;
		return false;
	}
	else if (f < -65504)
	{
		XVTF_SETERROR(xvtferrno, ERRORCODE::NUMBER_WAS_CLAMPED);
		this->rawBits = 0xFBFF;
		return false;
	}
	// Normal Numbers
	else
	{
		unsigned short rExp = (exp - 112) << 10;
		unsigned short rSign = sign << 15;
		unsigned short rMantissa = (mantissa & 0x7FE000) >> 13;

		this->rawBits = rSign | rExp | rMantissa;

		XVTF_SETERROR(xvtferrno, ERRORCODE::NONE);
		return true;
	}
}

xvtf::half& xvtf::half::operator=(float f)
{
	this->setFloat(f);
	return *this;
}