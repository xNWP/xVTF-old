//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file cpp_linkage.cpp
/// @brief verifies that the library links without error.
//==============================================================================

#include "xVTF/xVTF.h"

int main()
{
	if (xvtf::WhatVersion() == XVTF_VERSION)
		return 0;
	else
		return -1;
}