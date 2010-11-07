#include "stdafx.h"
#include "GameObjectEditorView.h"

#include "MemDC.h"

#define M_Margin	50


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(GameObjectEditorView, CScrollView)

BEGIN_MESSAGE_MAP(GameObjectEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
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
	pDC->FillSolidRect(rcClient, RGB(255, 255, 255));

	pDC->OffsetViewportOrg(M_Margin, M_Margin);

	CPoint ptLP = GetScrollPosition();
	ptLP.Offset(-M_Margin, -M_Margin);

	CRect rc = rcClient;
	rc.OffsetRect(ptLP);


	//MemDC memDC(pDC);
}

void GameObjectEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx	= 500 + M_Margin * 2;
	sizeTotal.cy	= 500 + M_Margin * 2;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

// GameObjectEditorView 消息处理程序

void GameObjectEditorView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void GameObjectEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);

	CScrollView::OnLButtonDown(nFlags, point);
}

void GameObjectEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);

	CScrollView::OnLButtonUp(nFlags, point);
}

void GameObjectEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint ptLP = point;
	ptLP.Offset( GetScrollPosition() );
	ptLP.Offset(-M_Margin, -M_Margin);

	CScrollView::OnMouseMove(nFlags, point);
}

void GameObjectEditorView::LogicInvalidate(CRect rc)
{
	CPoint pt = GetScrollPosition();
	rc.OffsetRect(-pt);
	rc.OffsetRect(M_Margin, M_Margin);

	InvalidateRect(rc);
}

BOOL GameObjectEditorView::OnEraseBkgnd(CDC* pDC)
{
	//return CScrollView::OnEraseBkgnd(pDC);

	return TRUE;
}
