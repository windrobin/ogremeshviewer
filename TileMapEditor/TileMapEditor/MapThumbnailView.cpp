#include "stdafx.h"
#include "MainFrm.h"
#include "MapThumbnailView.h"
#include "Resource.h"
#include "TileMapEditor.h"

#include "TileMapEditorView.h"

#include "MemDC.h"

using namespace Cactus;


//////////////////////////////////////////////////////////////////////
// 构造/析构
//////////////////////////////////////////////////////////////////////

MapThumbnailView::MapThumbnailView()
{
}

MapThumbnailView::~MapThumbnailView()
{
}

BEGIN_MESSAGE_MAP(MapThumbnailView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MapThumbnailView 消息处理程序

int MapThumbnailView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void MapThumbnailView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	Invalidate();
}

BOOL MapThumbnailView::PreTranslateMessage(MSG* pMsg)
{
	return CDockablePane::PreTranslateMessage(pMsg);
}

void MapThumbnailView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文


	MemDC memDC(&dc);

	CRect rcClient;
	GetClientRect(rcClient);

	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	CTileMapEditorView* pView = (CTileMapEditorView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView(); 
	if (pView)
	{
		CRect rcView;
		CSize szDoc;
		CPoint ptScroll;
		CDC* pDC = pView->GetDrawingContent(rcView, szDoc, ptScroll);
		if (pDC)
		{
			float fRatio1 = 1.0f * rcClient.Width() / szDoc.cx;
			float fRatio2 = 1.0f * rcClient.Height() / szDoc.cy;
			float fRatio = fRatio1;
			if (fRatio1 > fRatio2)
				fRatio = fRatio2;

			CRect rcDoc = CRect( CPoint( (rcClient.Width() - fRatio * szDoc.cx)/2
				, (rcClient.Height() - fRatio * szDoc.cy)/2 )
				, CSize( szDoc.cx * fRatio, szDoc.cy * fRatio )
				);

			CRect rcThumbView = CRect(CPoint(rcDoc.TopLeft().x + fRatio * ptScroll.x, rcDoc.TopLeft().y + fRatio * ptScroll.y)
				, CSize(rcView.Width() * fRatio, rcView.Height() * fRatio)
				);

			//绘制地图内容
			//memDC.SetStretchBltMode(HALFTONE);
			//memDC.StretchBlt( rcDoc.TopLeft().x + fRatio * ptScroll.x + 1, rcDoc.TopLeft().y + fRatio * ptScroll.y + 1
			//	, rcView.Width() * fRatio - 1, rcView.Height() * fRatio - 1
			//	, pDC, 0, 0, rcView.Width(), rcView.Height(), SRCCOPY
			//	);

			//绘制文档范围
			CPen pen(PS_SOLID, 1, RGB(0, 0, 255));
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.SelectStockObject(NULL_BRUSH);
			memDC.Rectangle(rcDoc);
			memDC.SelectObject(pOldPen);

			//绘制试图范围
			CPen pen2(PS_SOLID, 1, RGB(0, 255, 0));
			CPen* pOldPen2 = memDC.SelectObject(&pen2);
			memDC.SelectStockObject(LTGRAY_BRUSH);
			memDC.Rectangle(rcThumbView);
			memDC.SelectObject(pOldPen2);
		}
	}
}

void MapThumbnailView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

}

void MapThumbnailView::OnContextMenu(CWnd* pWnd, CPoint point)
{
}

void MapThumbnailView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnLButtonDown(nFlags, point);
}

void MapThumbnailView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnLButtonUp(nFlags, point);
}

void MapThumbnailView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDockablePane::OnMouseMove(nFlags, point);
}
