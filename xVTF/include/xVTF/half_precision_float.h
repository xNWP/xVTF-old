#ifndef XVTF_HALF_PRECISION_FLOAT_H__
#define XVTF_HALF_PRECISION_FLOAT_H__

#include "xVTF/exports.h"

#pragma pack(push, 1)

namespace xvtf
{
	//----------------------------------------------------------------------------------------------------
	/// Represents a half precision floating-point number.
	//----------------------------------------------------------------------------------------------------
	struct half
	{
	private:
		unsigned short int mantissa : 10;
		unsigned short int exp : 5;
		unsigned short int sign : 1;

	public:
		//----------------------------------------------------------------------------------------------------
		/// Converts the half precision float to a single precision float.
		/// @return float			The converted half precision float as a single precision float.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI float toFloat() const;

		/////////////////////////////////////////////////////////////////
	   //        ____ _   __ ______ ______ ____   _   __ ___     __     /
	   //		/  _// | / //_  __// ____// __ \ / | / //   |   / /     /
	   //       / / /  |/ /  / /  / __/  / /_/ //  |/ // /| |  / /      /
	   //     _/ / / /|  /  / /  / /___ / _, _// /|  // ___ | / /___    /
	   //    /___//_/ |_/  /_/  /_____//_/ |_|/_/ |_//_/  |_|/_____/    /
	   //																/
		/////////////////////////////////////////////////////////////////

		~half() = default;
		half() = default;
	private:
		half(const half&) = delete;
		half(const half&&) = delete;
		half& operator=(const half&) = delete;
		half& operator=(const half&&) = delete;
	};
}

#pragma pack(pop)

#endif // !XVTF_HALF_PRECISION_FLOAT_H__
