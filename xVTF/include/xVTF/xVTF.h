//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file xVTF.h
/// @brief Main header for the entire library, includes all public headers.
//==============================================================================

#ifndef XVTF_H__
#define XVTF_H__

#include "xVTF/bitmapimage.h"
#include "xVTF/error.h"
#include "xVTF/exports.h"
#include "xVTF/flags.h"
#include "xVTF/pixelformats.h"
#include "xVTF/types.h"
#include "xVTF/vtf_file.h"

#ifndef XVTF_STATIC
#	ifdef _WIN32
#		pragma message("Using xVTF in DLL Mode. If you are trying to use xVTF as a Static Library, define 'XVTF_STATIC' before including xVTF.h")
#	endif
#endif

#endif // !XVTF_H__