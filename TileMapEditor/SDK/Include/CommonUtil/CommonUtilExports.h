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
#pragma	once

#if defined(USE_COMMONUTIL_STATIC_LIB) || defined(_LIB)
#	define COMMONUTIL_API
#else
#	ifdef COMMONUTIL_EXPORTS
#		define COMMONUTIL_API __declspec(dllexport)
#	else
#		define COMMONUTIL_API __declspec(dllimport)
#	endif
#endif
