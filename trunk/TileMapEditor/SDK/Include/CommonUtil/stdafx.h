/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			CommonUtil.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
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