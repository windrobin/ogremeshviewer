// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#if defined(WIN32)
#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	define _WIN32_WINNT 0x0400
#	include <windows.h>
#endif

#pragma warning(disable : 4251 4661)

