//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file error_macro.h
/// @brief provides a simple macro for setting an error code.
//==============================================================================

#ifndef XVTF_ERROR_MACRO_H__
#define XVTF_ERROR_MACRO_H__

#define XVTF_SETERROR( CONTAINER, CODE) \
if (CONTAINER != nullptr) *CONTAINER = CODE

#endif // !XVTF_ERROR_MACRO_H__