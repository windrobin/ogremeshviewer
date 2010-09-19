/*=============================================================================
*	Copyright (C) 2006-2008, Zhang Kun. All Rights Reserved.
*
*	File name:
*			Path.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
=============================================================================*/

#pragma once

#include "PreInclude.h"

namespace Cactus
{
	/**
	A string represents a directory.
	*/
	class COMMONUTIL_API Path : public Object
	{
	public:
		/**
		Default constructor get a Path represents the current working directory.
		*/
		Path();

		/**
		Construct a Path use a file's path.
		*/
		Path( const Cactus::String& pathname );

		/**
		Catch the Path with more string.
		*/
		void					Catch( const Cactus::String& pathname );

		/**
		Get the string for the Path.
		*/
		Cactus::String			GetFullString() const;

		/**
		Get the Path/full path for file with filename.
		*/
		Cactus::String			Fixup( const Cactus::String& filename, bool include_filename = true ) const;

		/**
		Get the relative path string for filename to Path.
		*/
		Cactus::String			Relative( const Cactus::String& filename ) const;

		/**
		Get the relative Path to the current working directory.
		*/
		Cactus::String			RelativeToWorkingDir() const;

		/**
		Get the relative Path to the a directory.
		*/
		static Cactus::String	RelativeToDir( const Cactus::String& path, const Cactus::String& base_directory);

		/**
		Fix the '\' to '/'.
		*/
		static Cactus::String	FixSeparator( const Cactus::String& s );

		/**
		Split Path into sections.
		*/
		static void		Split(
							Cactus::vector<Cactus::String>::type& out,
							const Cactus::String& path,
							bool include_last = true
							);

	private:
		Cactus::vector<Cactus::String>::type	_strPath;
	};
}
