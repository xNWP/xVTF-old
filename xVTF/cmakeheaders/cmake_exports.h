//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file exports.h
/// @brief Provides the linking modes for the library.
//==============================================================================

#ifndef XVTF_EXPORTS_H__
#define XVTF_EXPORTS_H__

#ifdef _MSC_VER
# ifdef XVTF_STATIC
#	  define XVTFAPI
#	  define _CRT_SECURE_NO_WARNINGS
# else
#		ifdef XVTF_EXPORTS
#			define XVTFAPI __declspec(dllexport)
#			define _CRT_SECURE_NO_WARNINGS
#		else
#			define XVTFAPI __declspec(dllimport)
#		endif
#	endif
#endif

#ifdef __GNUC__
#	ifdef XVTF_EXPORTS
#		define XVTFAPI __attribute__((visibility("default")))
#	else
#		define XVTFAPI
#	endif
#endif

#cmakedefine XVTF_VERSION @XVTF_VERSION@

#endif // !XVTF_EXPORTS_H__
