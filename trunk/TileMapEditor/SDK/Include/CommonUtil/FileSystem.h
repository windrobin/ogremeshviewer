/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			FileSystem.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "FileSystemBase.h"
#include "OSFileSystem.h"

namespace Cactus
{
	//The OSFileSystem is always there, you can use it to load a disk file explicitly.
	//GetDefaultFileSystem() will return a customize file system, usually a package file system.
	//If SetDefaultFileSystem() is not set, the OSFileSystem is default.
	class COMMONUTIL_API FileManager
	{
	public:
		static FileManager&		GetInstance();
		
		IFileSystem*			GetDefaultFileSystem();
		void					SetDefaultFileSystem(IFileSystem* fs);

		OSFileSystem&			GetOSFileSystem();
		
		const Cactus::String&	GetWorkingDir() { return _strWorkingDir; }

		const Cactus::String&	GetDataDir() { return _strDataDir; }
		void					SetDataDir(const Cactus::String& dataDir) { _strDataDir = dataDir; }

	protected:

		FileManager();

		IFileSystem*		_fs;
		OSFileSystem		_osFS;

		Cactus::String		_strWorkingDir;		//execute file directory
		Cactus::String		_strDataDir;		//data root directory
	};
	
}
