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

#include "stdafx.h"
#include "MainFrm.h"
#include "ActorPanel.h"
#include "Resource.h"
#include "OMV.h"
#include "OMVDoc.h"

#include "OgreFramework.h"
#include "Actor.h"

using namespace Ogre;

#define M_ID_CheckListBox	(WM_USER + 10)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ActorPanel::ActorPanel()
{
}

ActorPanel::~ActorPanel()
{
}

BEGIN_MESSAGE_MAP(ActorPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_LBN_SELCHANGE(M_ID_CheckListBox, OnCheckListSelectChange)
	ON_CLBN_CHKCHANGE(M_ID_CheckListBox, OnCheckListCheckChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int ActorPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateStockObject(DEFAULT_GUI_FONT);

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	_actorsList.Create(WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_NOTIFY | LBS_SORT, rectDummy, this, M_ID_CheckListBox);
	_actorsList.SetFont(&m_Font);

	return 0;
}

void ActorPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	//rcClient.DeflateRect(1, 1, 1, 1);

	_actorsList.MoveWindow(&rcClient);
}

BOOL ActorPanel::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}


void ActorPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void ActorPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

BOOL ActorPanel::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void ActorPanel::AddActorName(const CString& str)
{
	int iIndex = _actorsList.InsertString(0, str);
	if (LB_ERR != iIndex)
	{
		_actorsList.SetCheck(iIndex, 1);
	}
}

void ActorPanel::OnCheckListSelectChange()
{
	int iIndex = _actorsList.GetCurSel();
	if (iIndex != -1)
	{
		int iCheck = _actorsList.GetCheck(iIndex);
		if (iCheck == 0)
			return;

		CString strName;
		_actorsList.GetText(iIndex, strName);

		OgreFramework::getSingleton().SetCurrentActor((LPCTSTR)strName);

		CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
		pMainFrame->GetAnimationPanel().ShowWindow(SW_SHOW);
		pMainFrame->GetSkeletonControlPanel().BuildSkeletonAnimInfo((LPCTSTR)strName);
	}

}

void ActorPanel::OnCheckListCheckChange()
{
	int iIndex = _actorsList.GetCurSel();
	if (iIndex != -1)
	{
		CString strName;
		_actorsList.GetText(iIndex, strName);

		Actor* pActor = OgreFramework::getSingletonPtr()->GetActor((LPCTSTR)strName);
		if (!pActor)
			return;

		int iCheck = _actorsList.GetCheck(iIndex);
		if (iCheck == 0)
		{
			pActor->SetVisible(false);
		}
		else
		{
			OgreFramework::getSingleton().SetCurrentActor((LPCTSTR)strName);

			CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
			pMainFrame->GetAnimationPanel().ShowWindow(SW_SHOW);
			pMainFrame->GetSkeletonControlPanel().BuildSkeletonAnimInfo((LPCTSTR)strName);

			pActor->SetVisible(true);
		}
	}
}

void ActorPanel::OnReset()
{
	_actorsList.ResetContent();
}
