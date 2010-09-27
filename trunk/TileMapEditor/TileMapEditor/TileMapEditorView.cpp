
// TileMapEditorView.cpp : CTileMapEditorView 类的实现
//

#include "stdafx.h"
#include "TileMapEditor.h"

#include "TileMapEditorDoc.h"
#include "TileMapEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTileMapEditorView

IMPLEMENT_DYNCREATE(CTileMapEditorView, CScrollView)

BEGIN_MESSAGE_MAP(CTileMapEditorView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CTileMapEditorView 构造/析构

CTileMapEditorView::CTileMapEditorView()
{
	// TODO: 在此处添加构造代码

}

CTileMapEditorView::~CTileMapEditorView()
{
}

BOOL CTileMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CTileMapEditorView 绘制

void CTileMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CTileMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}

void CTileMapEditorView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	sizeTotal.cx	= GetDocument()->GetMap()._iWidth;
	sizeTotal.cy	= GetDocument()->GetMap()._iHeight;
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
	sizeTotal.cx	= GetDocument()->GetMap()._iWidth;
	sizeTotal.cy	= GetDocument()->GetMap()._iHeight;
	SetScrollSizes(MM_TEXT, sizeTotal);

	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CTileMapEditorView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnLButtonDown(nFlags, point);
}

void CTileMapEditorView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnLButtonUp(nFlags, point);
}

void CTileMapEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CScrollView::OnMouseMove(nFlags, point);
}
