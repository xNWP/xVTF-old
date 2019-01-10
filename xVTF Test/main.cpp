#include <iostream>
#include "xVTF/xVTF.h"

int main()
{
#ifdef _DEBUG
	xvtf::SayHello(std::cout);
#else
	std::cout << "Please compile in Debug Mode in order to test xVTF.\n";
#endif
	return 0;
}