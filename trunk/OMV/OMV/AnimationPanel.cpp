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

#include "stdafx.h"
#include "MainFrm.h"
#include "AnimationPanel.h"
#include "Resource.h"
#include "OMV.h"
#include "OMVDoc.h"

#include "OgreFramework.h"

using namespace Ogre;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AnimationPanel::AnimationPanel()
{
	//m_nCurrSort = ID_SORTING_GROUPBYTYPE;
}

AnimationPanel::~AnimationPanel()
{
}

BEGIN_MESSAGE_MAP(AnimationPanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClassView message handlers

int AnimationPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	_SkeletonDlg.Create(SkeletonControlPanel::IDD, this);

	return 0;
}

void AnimationPanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rcClient;
	GetClientRect(rcClient);
	//rcClient.DeflateRect(1, 1, 1, 1);

	_SkeletonDlg.MoveWindow(&rcClient);
}

BOOL AnimationPanel::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}


void AnimationPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void AnimationPanel::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}


BOOL AnimationPanel::OnEraseBkgnd(CDC* pDC)
{

	return CWnd::OnEraseBkgnd(pDC);
}


void AnimationPanel::OnReset()
{
	_SkeletonDlg.OnReset();
}
