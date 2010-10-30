
// TileMapEditorView.cpp : CTileMapEditorView 类的实现
//

#include "stdafx.h"
#include "TileMapEditor.h"

#include "TileMapEditorDoc.h"
#include "TileMapEditorView.h"

#include "ToolManager.h"
#include "ToolBase.h"

#include "MemDC.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define M_Margin	50

// CTileMapEditorView

IMPLEMENT_DYNCREATE(CTileMapEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CTileMapEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CTileMapEditorView 构造/析构

CTileMapEditorView::CTileMapEditorView()
{
}

CTileMapEditorView::~CTileMapEditorView()
{
	//if( _bmpBackup.GetSafeHandle() )
	//{
	//	_memDC.SelectObject(_pOldBmp);
	//	_memDC.DeleteDC();
	//	_bmpBackup.DeleteObject();
	//}
}

BOOL CTileMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CScrollView::PreCreateWindow(cs);
}

// CTileMapEditorView 绘制

void CTileMapEditorView::OnDraw(CDC* pDC)
{
	GetClientRect(_rcClient);

	CTileMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDC->OffsetViewportOrg(M_Margin, M_Margin);

	//if( !_bmpBackup.GetSafeHandle() )
	//{
	//	_bmpBackup.CreateCompatibleBitmap(pDC, _rcClient.Width(), _rcClient.Height());

	//	_memDC.CreateCompatibleDC(pDC);
	//	_pOldBmp = _memDC.SelectObject(&_bmpBackup);
	//}

	{
		MemDC memDC(pDC);

		CPoint ptLP = GetScrollPosition();
		ptLP.Offset(-M_Margin, -M_Margin);

		CRect rc = _rcClient;
		rc.OffsetRect(ptLP);

		pDoc->GetMap().Draw(&memDC, rc);
		ToolManager::getSingleton().GetCurrentTool()->Draw(&memDC);

		//memDC->PeakBuffer(&_memDC);
	}


	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		MapThumbnailView* pMapThumbView = pMainFrame->GetMapThumbnailView(); 
		if (pMapThumbView->GetSafeHwnd())
			pMapThumbView->Invalidate();
	}

}

//CDC* CTileMapEditorView::GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll)
//{
//	if( !_bmpBackup.GetSafeHandle() )
//		return 0;
//
//	rc	= _rcClient;
//	ptScroll = GetScrollPosition();
//
//	szDoc.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
//	szDoc.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
//
//	return &_memDC;
//}
void CTileMapEditorView::GetDrawingContent(CRect& rc, CSize& szDoc, CPoint& ptScroll)
{
	rc	= _rcClient;
	ptScroll = GetScrollPosition();

	szDoc.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
	szDoc.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
}


void CTileMapEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
	sizeTotal.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CTileMapEditorView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTileMapEditorView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CTileMapEditorView 诊断

#ifdef _DEBUG
void CTileMapEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTileMapEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTileMapEditorDoc* CTileMapEditorView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTileMapEditorDoc)));
	return (CTileMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTileMapEditorView 消息处理程序

void CTileMapEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CSize sizeTotal;
	sizeTotal.cx	= GetDocument()->GetMap().GetPixelWidth() + M_Margin * 2;
	sizeTotal.cy	= GetDocument()->GetMap().GetPixelHeight() + M_Margin * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CTileMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnLButtonDown(nFlags, ptLP);

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTileMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnLButtonUp(nFlags, ptLP);

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTileMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);
	ToolManager::getSingleton().GetCurrentTool()->OnMouseMove(nFlags, ptLP);

	CScrollView::OnMouseMove(nFlags, point);
}

void CTileMapEditorView::LogicInvalidate(CRect rc)
{
	CPoint pt = GetScrollPosition();
	rc.OffsetRect(-pt);
	rc.OffsetRect(M_Margin, M_Margin);

	InvalidateRect(rc);
}

BOOL CTileMapEditorView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CTileMapEditorView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	//if( _bmpBackup.GetSafeHandle() )
	//{
	//	_memDC.SelectObject(_pOldBmp);
	//	_memDC.DeleteDC();
	//	_bmpBackup.DeleteObject();
	//}
}

void CTileMapEditorView::OffsetScrollPos(CPoint pt)
{
	CPoint ptScroll = GetScrollPosition();
	ptScroll.Offset(pt);

	ScrollToPosition(ptScroll);

	CMainFrame* pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, AfxGetMainWnd() );
	if (pMainFrame)
	{
		MapThumbnailView* pMapThumbView = pMainFrame->GetMapThumbnailView(); 
		if (pMapThumbView->GetSafeHwnd())
			pMapThumbView->Invalidate();
	}
}

void CTileMapEditorView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	//VK_ESCAPE VK_DELETE VK_SHIFT

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}
