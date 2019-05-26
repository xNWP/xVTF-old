#include "xVTF/xVTF.h"

int main(unsigned int argc, char** argv)
{
	// WIP
	using namespace xvtf;
	half h(12.5);
	auto t = h.toFloat();

	h = 22.5;
	t = h.toFloat();

	return 0;
}