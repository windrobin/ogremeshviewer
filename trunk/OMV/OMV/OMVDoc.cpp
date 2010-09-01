/*
-----------------------------------------------------------------------------
This source file is part of OMV(Ogre Mesh Viewer)
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

// OMVDoc.cpp : implementation of the COMVDoc class
//

#include "stdafx.h"
#include "OMV.h"
#include "OMVDoc.h"
#include "MainFrm.h"

#include "OgreFramework.h"

using namespace Ogre;

// COMVDoc

IMPLEMENT_DYNCREATE(COMVDoc, CDocument)

BEGIN_MESSAGE_MAP(COMVDoc, CDocument)
END_MESSAGE_MAP()


// COMVDoc construction/destruction

COMVDoc::COMVDoc()
{
	_meshSerializer = new MeshSerializer();
	_meshSerializer->setListener(this);
}

COMVDoc::~COMVDoc()
{
	delete _meshSerializer;
}

void COMVDoc::processMaterialName(Ogre::Mesh *mesh, Ogre::String *name)
{
	Ogre::MaterialPtr material = MaterialManager::getSingleton().getByName(*name);
	if (!material.getPointer())
	{
		Ogre::String	strMeshName = mesh->getName();
		Ogre::String	strMaterialName = strMeshName.substr(0, strMeshName.rfind(".") + 1);

		if (strMaterialName.length() == 0)
		{
			return;
		}
		strMaterialName += "material";

		try
		{
			DataStreamPtr data = Ogre::ResourceGroupManager::getSingleton().openResource(strMaterialName, "Material");
			MaterialManager::getSingleton().parseScript(data, "Material");
		}
		catch (Ogre::Exception& e)
		{
			CFileDialog dlg(TRUE, "material", 0
				, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
				, "Ogre Material Files (*.material)|*.material|All Files (*.*)|*.*||"
				);

			char curDir[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, curDir);

			if( dlg.DoModal() == IDOK )
			{
				::SetCurrentDirectory(curDir);

				Ogre::String strDlgPathName = dlg.GetPathName();
				Ogre::String strDlgPath = strDlgPathName.substr(0, strDlgPathName.rfind("\\") + 1);

				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(strDlgPath, "FileSystem", "Material");

				try
				{
					DataStreamPtr data = Ogre::ResourceGroupManager::getSingleton().openResource(strDlgPathName, "Material");
					MaterialManager::getSingleton().parseScript(data, "Material");
				}
				catch (...)
				{
				}
			}
		}
	}
}


BOOL COMVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// (SDI documents will reuse this document)
	_ResetDoc();

	return TRUE;
}

BOOL COMVDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);
	std::string strModulePath = buffer;

	strModulePath.erase(strModulePath.find_last_of('\\') + 1);
	SetCurrentDirectory(strModulePath.c_str());


	std::string strPathName = lpszPathName;

	std::string strPath = strPathName.substr(0, strPathName.rfind("\\") + 1);
	std::string strName = strPathName.substr(strPathName.rfind("\\") + 1);

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(strPath, "FileSystem", "MeshData");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(strPath, "FileSystem", "Material");

	Ogre::MeshPtr outMesh;
	if (LoadMeshFile(outMesh, strPathName, strName))
	{
		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		pMainFrame->GetMeshPanel().OnLoadMeshFile(outMesh, strName);
	}

	return TRUE;
}

BOOL COMVDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// need not it, but have to restore working dir

	char buffer[MAX_PATH];
	::GetModuleFileName(0, buffer, MAX_PATH);
	std::string strModulePath = buffer;

	strModulePath.erase(strModulePath.find_last_of('\\') + 1);
	SetCurrentDirectory(strModulePath.c_str());


	return TRUE;
}

// COMVDoc serialization

void COMVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

// COMVDoc commands

bool COMVDoc::LoadMeshFile(Ogre::MeshPtr& outMesh, const Ogre::String& strPathName, const Ogre::String& strName)
{
	if (_Meshes.find(strName) != _Meshes.end())
	{
		AfxMessageBox("Mesh already exist!", MB_OK | MB_ICONERROR);
		return false;
	}

	std::ifstream ifs;
	ifs.open(strPathName.c_str(), std::ios_base::in | std::ios_base::binary);

	if (ifs.bad())
	{
		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Unable to load mesh file:");
		OgreFramework::getSingletonPtr()->m_pLog->logMessage(strPathName);
		return false;
	}

	// pass false for freeOnClose to FileStreamDataStream since ifs is created on stack
	DataStreamPtr stream(new FileStreamDataStream(strPathName, &ifs, false));

	outMesh = MeshManager::getSingleton().create(strName, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	_meshSerializer->importMesh(stream, outMesh.getPointer());

	_Meshes[strName] = outMesh;

	return true;
}

Ogre::MeshPtr COMVDoc::GetMesh(const Ogre::String& meshName)
{
	return _Meshes[meshName];
}

void COMVDoc::_ResetDoc()
{
	for (NameMeshMapType::iterator it = _Meshes.begin(); it != _Meshes.end(); ++it)
	{
		it->second.setNull();
	}
	_Meshes.clear();

	//Notify all views
	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	pMainFrame->OnResetAllViews();
}



