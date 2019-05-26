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
		union
		{
			unsigned short rawBits;
			struct
			{
				unsigned short int mantissa : 10;
				unsigned short int exp : 5;
				unsigned short int sign : 1;
			};
		};

	public:
		//----------------------------------------------------------------------------------------------------
		/// Initializes a 16-bit half precision floating-point number with default value 0.0.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI half() : rawBits(0) { };

		half(const half& h) { this->rawBits = h.rawBits; }
		half(const half&& h) { this->rawBits = h.rawBits; }
		half& operator=(const half& h) { this->rawBits = h.rawBits; }
		half& operator=(const half&& h) { this->rawBits = h.rawBits; }

		//----------------------------------------------------------------------------------------------------
		/// Creates the half from a float.
		/// If any of the following special cases occur the following values will be given.
		/// You can instead use the function setFloat to also get an error code for these cases.
		/// +-Inf	=> +-65504 (Max/Min 16 bit floating point number)
		/// NaN		=> 0
		/// f > 65504	=> 65504
		/// f < -65504	=> -65504
		//----------------------------------------------------------------------------------------------------
		XVTFAPI half(float f);

		//----------------------------------------------------------------------------------------------------
		/// Converts the half precision float to a single precision float.
		/// If any of the following special cases occur the error code will be set to reflect this.
		/// +-Inf	=> +-65504 (Max/Min 16 bit floating point number)
		/// NaN		=> 0
		/// @param[out] xvtferrno	Will signal +/- infinity as well as NaN. Can be nullptr
		/// @return float			The converted half precision float as a single precision float.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI float toFloat(unsigned int * const & xvtferrno = nullptr) const;

		//----------------------------------------------------------------------------------------------------
		/// Set's the value from a float.
		/// If any of the following special cases occur the following values will be given.
		/// You can also set xvtferrno to something other than nullptr to signal any of these special cases.
		/// +-Inf		=> +-65504 (Max/Min 16 bit floating point number)
		/// NaN			=> 0
		/// f > 65504	=> 65504
		/// f < -65504	=> -65504
		/// @param[in] f			The float to copy.
		/// @param[out] xvtferrno	Will signal +/- infinity, NaN, and any clamping of the value.
		/// @return bool			True if no special cases occured.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI bool setFloat(float f, unsigned int * const & xvtferrno = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Set's the value from a float.
		/// If any of the following special cases occur the following values will be given.
		/// You can instead use the function setFloat to also get an error code for these cases.
		/// +-Inf	=> +-65504 (Max/Min 16 bit floating point number)
		/// NaN		=> 0
		/// f > 65504	=> 65504
		/// f < -65504	=> -65504
		/// @return half			The half float.
		//----------------------------------------------------------------------------------------------------
		XVTFAPI half& operator=(float f);
	};
}

#pragma pack(pop)

#endif // !XVTF_HALF_PRECISION_FLOAT_H__
