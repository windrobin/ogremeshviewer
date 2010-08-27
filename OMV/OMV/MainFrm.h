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

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
//#include "FileView.h"
#include "MeshPanel.h"
#include "AnimationPanel.h"

#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "ActorPanel.h"
#include "Resource.h"

//class COutlookBar : public CMFCOutlookBar
//{
//	virtual BOOL AllowShowOnPaneMenu() const { return TRUE; }
//	virtual void GetPaneName(CString& strName) const { BOOL bNameValid = strName.LoadString(IDS_OUTLOOKBAR); ASSERT(bNameValid); }
//};

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	SkeletonControlPanel&	GetSkeletonControlPanel(){ return _AnimationPanel._SkeletonDlg; }
	AnimationPanel&			GetAnimationPanel(){ return _AnimationPanel; }

	ActorPanel&				GetActorPanel(){ return _ActorPanel; }
	MeshPanel&				GetMeshPanel(){ return _MeshPanel; }

	void					OnResetAllViews();

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar		m_wndRibbonBar;				// RibbonBar
	CMFCRibbonApplicationButton m_MainButton;		// Top - Left MFC button
	CMFCToolBarImages	m_PanelImages;
	
	CMFCRibbonStatusBar  m_wndStatusBar;

	MeshPanel			_MeshPanel;
	AnimationPanel		_AnimationPanel;
	ActorPanel			_ActorPanel;
	//CFileView         m_wndFileView;
	CPropertiesWnd		_wndProperties;

	COutputWnd			_LogPanel;

	//COutlookBar       m_wndNavigationBar;
	//CMFCShellTreeCtrl m_wndTree;
	//CCalendarBar      m_wndCalendar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnViewPanels(UINT id);
	afx_msg void OnUpdateViewPanels(CCmdUI* pCmdUI);
	afx_msg void OnSceneOptions(UINT id);
	afx_msg void OnUpdateSceneOptions(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()


	void InitializeRibbon();
	BOOL CreateDockingWindows();

	//BOOL CreateOutlookBar(CMFCOutlookBar& bar, UINT uiID, int nInitialWidth);
	//int FindFocusedOutlookWnd(CMFCOutlookBarTabCtrl** ppOutlookWnd);
	//CMFCOutlookBarTabCtrl* FindOutlookParent(CWnd* pWnd);
	//CMFCOutlookBarTabCtrl* m_pCurrOutlookWnd;
	//CMFCOutlookBarPane*    m_pCurrOutlookPage;

public:
	afx_msg void OnClose();
};


