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

#pragma once

#include "ViewTree.h"

class CMainFrame;

class CClassToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class MeshPanel : public CDockablePane
{
public:
	MeshPanel();
	virtual ~MeshPanel();

	void	OnLoadMeshFile(Ogre::MeshPtr& mesh, const Ogre::String& strName);
	void	OnReset();

protected:
	CClassToolBar	m_wndToolBar;
	CViewTree		m_wndItemTree;
	CImageList		m_ClassViewImages;
	
	//UINT m_nCurrSort;

	void OnChangeVisualStyle();

	void	BuildSharedGeometryInfo(HTREEITEM hMesh, const Ogre::MeshPtr& mesh);
	void	BuildVertexDeclarationInfo(HTREEITEM hVertexDecl, Ogre::VertexDeclaration* vDecl);
	void	BuildGeometryInfo(HTREEITEM hMesh, const Ogre::MeshPtr& mesh);
	void	BuildMaterialInfo(HTREEITEM hMaterial, const Ogre::MaterialPtr& material);

	void			SaveCurrentDir();
	void			RestoreCurrentDir();
	Ogre::String	GetDlgDir(const CString& strDlgPathName);

	HTREEITEM		_hRoot;
	HTREEITEM		_hMaterialGroup;
	Ogre::String	_strOldCurrentDir;

	CMainFrame*		_pMainFrame;

// Overrides
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);

	//afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//afx_msg void OnClassAddMemberFunction();
	//afx_msg void OnClassAddMemberVariable();
	//afx_msg void OnClassDefinition();
	//afx_msg void OnClassProperties();

	//afx_msg void OnSort(UINT id);
	//afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

	afx_msg void OnNewFolder();
	afx_msg void OnOpenMeshFile();

	DECLARE_MESSAGE_MAP()
};

