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

// OMVView.cpp : implementation of the COMVView class
//

#include "stdafx.h"
#include "OMV.h"

#include "OMVDoc.h"
#include "OMVView.h"

#include "OgreFramework.h"

using namespace Ogre;



// COMVView diagnostics

#ifdef _DEBUG
void COMVView::AssertValid() const
{
	CView::AssertValid();
}

void COMVView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COMVDoc* COMVView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COMVDoc)));
	return (COMVDoc*)m_pDocument;
}
#endif //_DEBUG



// COMVView

IMPLEMENT_DYNCREATE(COMVView, CView)

BEGIN_MESSAGE_MAP(COMVView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_MOUSELEAVE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// COMVView construction/destruction

COMVView::COMVView()
{
	_bCaptured = false;
}

COMVView::~COMVView()
{
}

BOOL COMVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COMVView drawing

void COMVView::OnDraw(CDC* /*pDC*/)
{
	COMVDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void COMVView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void COMVView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// COMVView message handlers
BOOL COMVView::OnEraseBkgnd(CDC* pDC)
{
	//return CDockablePane::OnEraseBkgnd(pDC);

	return TRUE;
}


void COMVView::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetCursorPos(&_cursorPos);
	_bCaptured = true;

	SetCapture();
	ShowCursor(FALSE);

	CView::OnLButtonDown(nFlags, point);
}

void COMVView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (_bCaptured)
	{
		_bCaptured = false;

		SetCursorPos(_cursorPos.x, _cursorPos.y);
		ReleaseCapture();
		ShowCursor(TRUE);
	}

	CView::OnLButtonUp(nFlags, point);
}

void COMVView::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	BOOL bResult = rcClient.PtInRect(point);

	if(bResult && !OgreFramework::getSingletonPtr()->_bRenderWndHasMouse)
	{
		TRACKMOUSEEVENT		csTME;
		csTME.cbSize = sizeof(csTME);
		csTME.dwFlags = TME_LEAVE;
		csTME.hwndTrack = this->GetSafeHwnd();
		::_TrackMouseEvent(&csTME);

		OgreFramework::getSingletonPtr()->_bRenderWndHasMouse = true;
		Invalidate();
	}

	if (_bCaptured)
	{
		//SetCursorPos(_cursorPos.x, _cursorPos.y);
	}

	CView::OnMouseMove(nFlags, point);
}

void COMVView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (OgreFramework::getSingletonPtr() && OgreFramework::getSingletonPtr()->m_pRenderWnd)
	{
		OgreFramework::getSingletonPtr()->m_pRenderWnd->windowMovedOrResized(); 

		// Adjust camera's aspect ratio, too 
		if (cx != 0 && cy != 0) 
			OgreFramework::getSingletonPtr()->m_pCamera->setAspectRatio((Ogre::Real)cx / (Ogre::Real)cy); 

		OgreFramework::getSingletonPtr()->m_pCamera->yaw(Radian(0));
	}
}


void COMVView::OnMouseLeave()
{
	OgreFramework::getSingletonPtr()->_bRenderWndHasMouse = false;

	CView::OnMouseLeave();
}

int COMVView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}
