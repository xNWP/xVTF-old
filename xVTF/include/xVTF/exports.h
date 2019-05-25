#ifndef XVTF_EXPORTS_H__
#define XVTF_EXPORTS_H__

/* Linking Mode */
#ifdef XVTF_STATIC
#	define XVTFAPI
#	define _CRT_SECURE_NO_WARNINGS
#else
#	ifdef X_VTF_EXPORTS
#		define XVTFAPI __declspec(dllexport)
#		define _CRT_SECURE_NO_WARNINGS
#	else
#		define XVTFAPI __declspec(dllimport)
#		undef _CRT_SECURE_NO_WARNINGS
#	endif
#endif

#endif // !XVTF_EXPORTS_H__
