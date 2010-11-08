#include "stdafx.h"
#include "GameObjectEditorView.h"
#include "GameObjectEditorObjects.h"
#include "MemDC.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(GameObjectEditorView, CScrollView)

BEGIN_MESSAGE_MAP(GameObjectEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()

// GameObjectEditorView 构造/析构

GameObjectEditorView::GameObjectEditorView()
{
}

GameObjectEditorView::~GameObjectEditorView()
{
}

BOOL GameObjectEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// GameObjectEditorView 绘制

void GameObjectEditorView::OnDraw(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	MemDC memDC(pDC);
	memDC.FillSolidRect(rcClient, RGB(255, 255, 255));

	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		CDialogGameObject* pGODlg = GetGODlg();
		pGODlg->DrawEditingObject(&memDC, CPoint(0, 0));

		_DrawGrid(&memDC);
		_DrawCenterGrid(&memDC);
		_tool.Draw(&memDC);
	}
}

void GameObjectEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx	= 500;
	sizeTotal.cy	= 500;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// GameObjectEditorView 消息处理程序

void GameObjectEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void GameObjectEditorView::LogicInvalidate(CRect rc)
{
	CPoint pt = GetScrollPosition();
	rc.OffsetRect(-pt);

	InvalidateRect(rc);
}

void GameObjectEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );

	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnLButtonDown(nFlags, ptLP);
	}

	CScrollView::OnLButtonDown(nFlags, ptLP);
}

void GameObjectEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );

	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnLButtonUp(nFlags, ptLP);
	}

	CScrollView::OnLButtonUp(nFlags, ptLP);
}

void GameObjectEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );

	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnMouseMove(nFlags, ptLP);
	}

	CScrollView::OnMouseMove(nFlags, ptLP);
}

BOOL GameObjectEditorView::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);

	return TRUE;
}

int GameObjectEditorView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void GameObjectEditorView::_DrawGrid(CDC* pDC)
{
	CDialogGameObject* pDlg = GetGODlg();

	CPen pen(PS_SOLID, 1, RGB(0, 255, 0));
	CPen* pOldPen = pDC->SelectObject(&pen);

	int iMapW = pDlg->_iTileW * pDlg->_iTileCount;
	int iMapH = pDlg->_iTileH * pDlg->_iTileCount;

	if (pDlg->_iMapType == 0)
	{
		//画横线
		for (int i = 0; i <= pDlg->_iTileCount; i++)
		{
			pDC->MoveTo(0, i * pDlg->_iTileH);
			pDC->LineTo(iMapW, i * pDlg->_iTileH);
		}

		//画竖线
		for (int i = 0; i <= pDlg->_iTileCount; i++)
		{
			pDC->MoveTo(i * pDlg->_iTileW, 0);
			pDC->LineTo(i * pDlg->_iTileW, iMapH);
		}
	}
	else
	{
		//画斜横线
		for (int i = 0; i <= pDlg->_iTileCount; i++)
		{
			pDC->MoveTo(iMapW/2 - i * pDlg->_iTileW / 2
				, i * pDlg->_iTileH / 2);

			pDC->LineTo(iMapW - i * pDlg->_iTileW / 2
				, iMapH/2 + i * pDlg->_iTileH / 2);
		}

		//画斜竖线
		for (int i = 0; i <= pDlg->_iTileCount; i++)
		{
			pDC->MoveTo(i * pDlg->_iTileW / 2
				, iMapH/2 + i * pDlg->_iTileH / 2);

			pDC->LineTo(iMapW/2 + i * pDlg->_iTileW / 2
				, i * pDlg->_iTileH / 2);
		}
	}

	pDC->SelectObject(pOldPen);
}

void GameObjectEditorView::_DrawCenterGrid(CDC* pDC)
{
	CDialogGameObject* pGODlg = GetGODlg();
	CPoint ptCenter(pGODlg->_iTileCount/2, pGODlg->_iTileCount/2);
	CRect rcPixel = pGODlg->GetPixelCoordRect(ptCenter);

	CPen pen(PS_SOLID, 2, RGB(0, 0, 255));

	pDC->SelectStockObject(NULL_BRUSH);
	CPen* pOldPen = pDC->SelectObject(&pen);


	CRect rc = rcPixel;

	if (pGODlg->_iMapType == 0)
	{
		rc.DeflateRect(1, 1, 0, 0);

		pDC->Rectangle(rc);
	}
	else
	{
		rc.DeflateRect(1, 1, 1, 1);

		CPoint pts[4];
		pts[0] = CPoint(rc.left, rc.top + rc.Height()/2);
		pts[1] = CPoint(rc.left + rc.Width()/2, rc.top);
		pts[2] = CPoint(rc.right, rc.top + rc.Height()/2);
		pts[3] = CPoint(rc.left + rc.Width()/2, rc.bottom);

		pDC->Polygon(pts, 4);
	}

	pDC->SelectObject(pOldPen);
}
