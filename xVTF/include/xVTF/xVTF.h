#ifndef X_VTF_H__
#define X_VTF_H__

#ifdef _DEBUG
	#include <iostream>
#endif

#ifdef X_VTF_STATIC
	#define XVTFAPI
#else
	#pragma message ( "Using xVTF in DLL Mode. If you are trying to use xVTF as a Static Library, define 'X_VTF_STATIC' before including xVTF.h" )
	#ifdef X_VTF_EXPORTS
		#define XVTFAPI extern "C" __declspec(dllexport)
	#else
		#define XVTFAPI extern "C" __declspec(dllimport)
	#endif
#endif

namespace xvtf
{
	#ifdef _DEBUG
	//---------------------------------------------------------------------------------------------------
	/// Provides a convience function to test that the library is linking. Library must be compiled in
	/// Debug mode in order for this to work (or atleast have _DEBUG defined).
	/// @param[out] ostr			The ostream object to print an output to, typically std::cout
	//---------------------------------------------------------------------------------------------------
	XVTFAPI void SayHello(std::ostream& ostr);
	#endif
}

#endif // !X_VTF_H__
