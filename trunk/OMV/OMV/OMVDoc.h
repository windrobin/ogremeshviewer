/*
-----------------------------------------------------------------------------
This source file is part of Tiger Viewer(An Ogre Mesh Viewer)
For the latest info, see http://code.google.com/p/ogremeshviewer/

Copyright (c) 2010 Zhang Kun(zhk.tiger@gmail.com)

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
This software also uses Microsoft Fluent UI.
License terms to copy, use or distribute the Fluent UI are available separately.  
To learn more about our Fluent UI licensing program, please visit 
http://msdn.microsoft.com/officeui.

Generally speaking, Fluent UI is free, if you do not use it make a contest software
like Office.
-----------------------------------------------------------------------------
*/

// OMVDoc.h : interface of the COMVDoc class
//


#pragma once


class COMVDoc : public CDocument
	, public Ogre::MeshSerializerListener
{
protected: // create from serialization only
	COMVDoc();
	DECLARE_DYNCREATE(COMVDoc)

// Attributes
public:
	virtual ~COMVDoc();

	bool	LoadMeshFile(Ogre::MeshPtr& outMesh, const Ogre::String& strPathName, const Ogre::String& strName);

	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void Serialize(CArchive& ar);

	Ogre::MeshPtr	GetMesh(const Ogre::String& meshName);


protected:
	Ogre::MeshSerializer*	_meshSerializer;

	typedef Ogre::map<Ogre::String, Ogre::MeshPtr>::type	NameMeshMapType;
	NameMeshMapType			_Meshes;

	void processMaterialName(Ogre::Mesh *mesh, Ogre::String *name);
	void processSkeletonName(Ogre::Mesh *mesh, Ogre::String *name){};

	void	_ResetDoc();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

