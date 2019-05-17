#include "xVTF/xVTF.h"
#include <stdint.h>

int main(unsigned int argc, char** argv)
{
	// WIP

	xvtf::half* h;
	float* f;

	uint16_t val = 0b0011010101010101;
	h = (xvtf::half*)&val;

	uint32_t fval = 0b00111110101010101010101010101011;
	f = (float*)&fval;

	auto hf = h->toFloat();

	return 0;
}