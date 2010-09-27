
// TileMapEditorView.cpp : CTileMapEditorView ���ʵ��
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

// CTileMapEditorView ����/����

CTileMapEditorView::CTileMapEditorView()
{
	// TODO: �ڴ˴���ӹ������

}

CTileMapEditorView::~CTileMapEditorView()
{
}

BOOL CTileMapEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CTileMapEditorView ����

void CTileMapEditorView::OnDraw(CDC* /*pDC*/)
{
	CTileMapEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
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


// CTileMapEditorView ���

#ifdef _DEBUG
void CTileMapEditorView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTileMapEditorView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CTileMapEditorDoc* CTileMapEditorView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTileMapEditorDoc)));
	return (CTileMapEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CTileMapEditorView ��Ϣ�������

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
