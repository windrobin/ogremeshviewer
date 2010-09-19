/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			Object.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/
#pragma once

#include "CommonUtilConfig.h"
#include "CommonUtilBasicType.h"
#include "CommonUtilExports.h"
#include "OgreMemAlloc.h"

#ifndef WIN32
#	include "Mutex.h"
#endif

namespace Cactus
{
	class COMMONUTIL_API Object : public AllocatedObject<GeneralAllocPolicy>
	{
	};

	class COMMONUTIL_API IRefObject
	{
	public:
		virtual void AddRef() = 0;
		virtual void Release() = 0;
	};


	class COMMONUTIL_API RefObject : public IRefObject, public Object
	{

#ifndef WIN32
	protected:
		ThreadMutex _objMutex;
#endif

	public:
		RefObject();
		virtual ~RefObject() {}

		void AddRef()
		{
#ifdef WIN32
			InterlockedIncrement(&_lRefCount);
#else
			MutexAutoLock<><> lock(_objMutex);
			++_lRefCount;
#endif
		}

		void Release()
		{
#ifdef WIN32
			long result = InterlockedDecrement(&_lRefCount);
#else
			MutexAutoLock<><> lock(_objMutex);
			long result = --_lRefCount;
#endif
			if(result == 0)
			{
				Mem_Delete this;
			}
		}

		Cactus::String GetObjectName() const
		{
			return _strName;
		}

		long GetReferenceCount() const
		{
			return _lRefCount; 
		}

	protected:
		long			_lRefCount;
		Cactus::String	_strName;
	};

}

#define ImplementIObject \
	public: \
		virtual void AddRef() \
		{ \
			Cactus::RefObject::AddRef(); \
		} \
		virtual void Release() \
		{ \
			Cactus::RefObject::Release(); \
		}


