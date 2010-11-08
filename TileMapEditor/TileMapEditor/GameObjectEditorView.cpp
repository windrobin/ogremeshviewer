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
		_DrawGrid(&memDC);

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

void GameObjectEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnLButtonDown(nFlags, point);
	}

	CScrollView::OnLButtonDown(nFlags, point);
}

void GameObjectEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnLButtonUp(nFlags, point);
	}

	CScrollView::OnLButtonUp(nFlags, point);
}

void GameObjectEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	ResourceGameObject* pGO = GetGOEditor()->_pResGO;
	if (pGO)
	{
		_tool.OnMouseMove(nFlags, point);
	}

	CScrollView::OnMouseMove(nFlags, point);
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

	int iMapW = pDlg->_iTileW * pDlg->_iTileCountX;
	int iMapH = pDlg->_iTileH * pDlg->_iTileCountY;

	if (pDlg->_iMapType == 0)
	{
		//画横线
		for (int i = 0; i <= pDlg->_iTileCountY; i++)
		{
			pDC->MoveTo(0, i * pDlg->_iTileH);
			pDC->LineTo(iMapW, i * pDlg->_iTileH);
		}

		//画竖线
		for (int i = 0; i <= pDlg->_iTileCountX; i++)
		{
			pDC->MoveTo(i * pDlg->_iTileW, 0);
			pDC->LineTo(i * pDlg->_iTileW, iMapH);
		}
	}
	else
	{
		//画斜横线
		for (int i = 0; i <= pDlg->_iTileCountY; i++)
		{
			pDC->MoveTo(iMapW/2 - i * pDlg->_iTileW / 2
				, i * pDlg->_iTileH / 2);

			pDC->LineTo(iMapW - i * pDlg->_iTileW / 2
				, iMapH/2 + i * pDlg->_iTileH / 2);
		}

		//画斜竖线
		for (int i = 0; i <= pDlg->_iTileCountX; i++)
		{
			pDC->MoveTo(i * pDlg->_iTileW / 2
				, iMapH/2 + i * pDlg->_iTileH / 2);

			pDC->LineTo(iMapW/2 + i * pDlg->_iTileW / 2
				, i * pDlg->_iTileH / 2);
		}
	}

	pDC->SelectObject(pOldPen);
}
