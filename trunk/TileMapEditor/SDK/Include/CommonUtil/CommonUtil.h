/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			CommonUtil.h
*	Description:
*			The only one interface file to other depended packages
*			of package CommonUtil.
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma	once

#include "PreInclude.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

//Add all exports here...
#	include "GString.h"
#	include "FileConfig.h"
#	include "Mutex.h"
#	include "Logger.h"
#	include "StringHelper.h"
#	include "Clock.h"
#	include "IClockObserver.h"
#	include "TimeSource.h"
#	include "Profiler.h"
#	include "SigSlot.h"
#	include "TGXML.h"
#	include "XMLDocHandler.h"
#	include "FileSystem.h"
#	include "ThreadWin32.h"
#	include "Path.h"
#	include "FileSystem.h"
#	include "PkgFileSystem.h"
#	include "OSFileSystem.h"
#	include "XMLOutStream.h"
#	include "CommonTemplate.h"
#	include "SharedPtr.h"
#	include "ResourceProvider.h"
#	include "Packet.h"
#	include "StaticMemPool.h"
#	include "NatureTime.h"
#	include "Updatable.h"

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif