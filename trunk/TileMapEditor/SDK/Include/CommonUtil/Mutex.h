/*
* Copyright (C) 2004-2008 Wu Yongwei <adah at users dot sourceforge dot net>
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any
* damages arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must
*    not claim that you wrote the original software.  If you use this
*    software in a product, an acknowledgement in the product
*    documentation would be appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must
*    not be misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* This file is part of Stones of Nvwa:
*      http://sourceforge.net/projects/nvwa
*
*/

#pragma once

#include "CommonUtilExports.h"

#ifdef _DEBUG
#   include <stdio.h>
#   include <stdlib.h>
#   define _FAST_MUTEX_ASSERT(_Expr, _Msg) \
	if (!(_Expr)) \
	{ \
		fprintf(stderr, "ThreadMutex::%s\n", _Msg); \
		abort(); \
	}
#else
#   define _FAST_MUTEX_ASSERT(_Expr, _Msg) ((void)0)
#endif

namespace Cactus
{

	class COMMONUTIL_API NullMutex
	{
	public:
		NullMutex(){}
		~NullMutex(){}

		void	Lock(){}
		bool	Trylock(){ return true; }
		void	Unlock(){}
	};

	//------------------------------------------------------------------------------

#ifdef WIN32

	class COMMONUTIL_API ThreadMutex
	{
	public:
		ThreadMutex();
		~ThreadMutex();

		void	Lock();
		bool	Trylock();
		void	Unlock();

	private:
		CRITICAL_SECTION _cs;

		ThreadMutex(const ThreadMutex&);
		ThreadMutex& operator=(const ThreadMutex&);
#ifdef _DEBUG
		bool _M_locked;
#endif
	};

#else

#	include <pthread.h>

	class ThreadMutex
	{
		pthread_mutex_t _M_mtx_impl;

#ifdef _DEBUG
		bool _M_locked;
#endif

	public:
		ThreadMutex()
#ifdef _DEBUG
			: _M_locked(false)
#endif
		{
			::pthread_mutex_init(&_M_mtx_impl, NULL);
		}
		~ThreadMutex()
		{
			_Mutex_ASSERT(!_M_locked, "~ThreadMutex(): still locked");

			::pthread_mutex_destroy(&_M_mtx_impl);
		}
		void Lock()
		{

			::pthread_mutex_lock(&_M_mtx_impl);
#ifdef _DEBUG
			_Mutex_ASSERT(!_M_locked, "lock(): already locked");
			_M_locked = true;
#endif
		}
		void Unlock()
		{

#ifdef _DEBUG
			_Mutex_ASSERT(_M_locked, "unlock(): not locked");
			_M_locked = false;
#endif
			::pthread_mutex_unlock(&_M_mtx_impl);
		}
	private:
		ThreadMutex(const Mutex&);
		ThreadMutex& operator=(const Mutex&);
	};


#endif//WIN32


	//------------------------------------------------------------------------------
	template<class LockType = ThreadMutex>
	class MutexAutoLock
	{
	public:
		explicit MutexAutoLock(LockType& mutex)
			: _mutex(mutex)
		{
			_mutex.Lock();
		}

		~MutexAutoLock()
		{
			_mutex.Unlock();
		}

	private:
		LockType& _mutex;
		MutexAutoLock(const MutexAutoLock&);
		MutexAutoLock& operator=(const MutexAutoLock&);
	};

	//------------------------------------------------------------------------------
	// Usage:
	// class SomeClass : public ObjectLevelLock<SomeClass>
	// typedef ObjectLevelLock<SomeClass>::lock lock;
	// lock __guard(*this);
	template <class _Host>
	class ObjectLevelLock
	{
		mutable ThreadMutex _M_mtx;
	public:
		class lock;
		friend class lock;

		/** Type that provides locking/unlocking semantics. */
		class lock
		{
			const ObjectLevelLock& _M_host;

			lock(const lock&);
			lock& operator=(const lock&);
		public:
			explicit lock(const ObjectLevelLock& __host) : _M_host(__host)
			{
				_M_host._M_mtx.Lock();
			}
			~lock()
			{
				_M_host._M_mtx.Unlock();
			}
#ifndef NDEBUG
			// The purpose of this method is allow one to write code
			// like "assert(guard.GetLockedObject() == this)" to
			// ensure that the locked object is exactly the object being
			// accessed.
			const ObjectLevelLock* GetLockedObject() const
			{
				return &_M_host;
			}
#   endif
		};
	};


	//------------------------------------------------------------------------------
	// Usage:
	// typedef ObjectLevelLock<Host>::lock lock;
	// lock __guard;
	template <class _Host, bool _bRealLock = true>
	class ClassLevelLock
	{
		static ThreadMutex _S_mtx;
	public:
		class lock;
		friend class lock;

		/** Type that provides locking/unlocking semantics. */
		class lock
		{
			lock(const lock&);
			lock& operator=(const lock&);
		public:
			lock()
			{
				if(_bRealLock)
					_S_mtx.Lock();
			}
			~lock()
			{
				if(_bRealLock)
					_S_mtx.Unlock();
			}
		};
	};

	template <class _Host, bool _bRealLock>
	ThreadMutex ClassLevelLock<_Host, _bRealLock>::_S_mtx;

}
